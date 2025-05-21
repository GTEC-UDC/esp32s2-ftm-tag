/**
 * @file main.c
 * @brief ESP32-S2 FTM Tag with Zenoh pub/sub (experimental replacement for MQTT)
 * Requires: https://github.com/eclipse-zenoh/zenoh-pico (for embedded pub/sub)
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <inttypes.h> // For PRIu32 format macros
#include <unistd.h>

// Fix for undefined identifiers in linting
#ifndef CONFIG_LOG_MAXIMUM_LEVEL
#define CONFIG_LOG_MAXIMUM_LEVEL 3
#endif

#ifndef CONFIG_FREERTOS_HZ
#define CONFIG_FREERTOS_HZ 100
#endif

#ifndef CONFIG_ESP_WIFI_STATIC_RX_BUFFER_NUM
#define CONFIG_ESP_WIFI_STATIC_RX_BUFFER_NUM 10
#endif

#ifndef CONFIG_ESP_WIFI_DYNAMIC_RX_BUFFER_NUM
#define CONFIG_ESP_WIFI_DYNAMIC_RX_BUFFER_NUM 32
#endif

#ifndef CONFIG_ESP_WIFI_TX_BUFFER_TYPE
#define CONFIG_ESP_WIFI_TX_BUFFER_TYPE 1
#endif

#ifndef CONFIG_ESP_WIFI_DYNAMIC_RX_MGMT_BUF
#define CONFIG_ESP_WIFI_DYNAMIC_RX_MGMT_BUF 32
#endif

#ifndef CONFIG_ESP_WIFI_ESPNOW_MAX_ENCRYPT_NUM
#define CONFIG_ESP_WIFI_ESPNOW_MAX_ENCRYPT_NUM 7
#endif

#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_wifi.h"
#include "esp_task_wdt.h"
#include "esp_mac.h"
#include "predict.h"
#include "predict_emxAPI.h"
#include "predict_terminate.h"
#include "predict_types.h"
#include "rt_nonfinite.h"

// Incluir solo el archivo principal de Zenoh
#include "zenoh-pico.h"

#define WIFI_SSID "Bunker_208_arriba"
#define WIFI_PASSWORD "brigantio"
#define DEFAULT_SCAN_LIST_SIZE 20 

// Prefix to identify FTM access points
#define FTM_AP_PREFIX "ftm_"

// FTM channel and bandwidth configuration -
// MUST MATCH THE ANCHOR
#define FTM_TARGET_CHANNEL 6  // Channel 6 like the anchor
#define FTM_SECOND_CHANNEL WIFI_SECOND_CHAN_ABOVE  // 40MHz same as anchor

// AP password (if configured with security)
#define FTM_AP_PASSWORD "ftmftmftmftm"  // Same password as in the anchor

// Treat all access points as potential FTM responders
// This is useful for debugging
#define TREAT_ALL_AP_AS_FTM 1

#define MAX_ANCHORS 8
#define MIN_RTT 0.0
#define MAX_RTT 220.0
#define MIN_RSS -90.0
#define MAX_RSS -50.0

// Use static allocation for WiFi records to avoid heap fragmentation
static wifi_ap_record_t s_ap_records[DEFAULT_SCAN_LIST_SIZE];
static wifi_ap_record_t anchors[MAX_ANCHORS];
static uint8_t num_anchors = 0, current_anchor = 0;

static const char *TAG = "FTM_TAG";
static const char *TAG_STA = "FTM_STA"; // Tag for WiFi STA events

static EventGroupHandle_t wifi_event_group;
static EventGroupHandle_t ftm_event_group;
const int CONNECTED_BIT = BIT0;
const int FTM_REPORT_BIT = BIT1;
const int FTM_FAILURE_BIT = BIT2;
const int SCAN_DONE_BIT = BIT3;

static wifi_ftm_report_entry_t *g_ftm_report = NULL;
static uint8_t g_ftm_report_num_entries = 0;
static uint8_t just_reboot = 0;

// NEW DATA STRUCTURES FOR ROS2-COMPATIBLE JSON
#define MAX_FTM_FRAMES_TO_STORE 16 // Should match or exceed ftmi_cfg.frm_count

typedef struct {
    int32_t rssi;
    uint32_t rtt;      // picoseconds
    uint64_t t1;       // picoseconds, from FTM report
    uint64_t t2;       // picoseconds, from FTM report
    uint64_t t3;       // picoseconds, from FTM report
    uint64_t t4;       // picoseconds, from FTM report
} ftm_frame_entry_data_t;

typedef struct {
    char anchor_id[18];          // MAC address string "XX:XX:XX:XX:XX:XX"
    uint32_t rtt_est_ps;         // from event->rtt_est (picoseconds)
    uint32_t rtt_raw_ps;         // from event->rtt_raw (picoseconds)
    float dist_est_m;            // from event->dist_est (cm) converted to meters
    float own_est_m;             // from resultCm (cm) converted to meters
    int32_t num_frames;
    ftm_frame_entry_data_t frames[MAX_FTM_FRAMES_TO_STORE];
    bool valid;
} ftm_composite_data_t;

static ftm_composite_data_t g_ftm_data_buffer = {0}; // Global buffer to hold the processed FTM data

emxArray_real_T *X;
emxArray_real_T *result;

// Zenoh Configurations
#define ZENOH_FTM_TOPIC "ftm"

#define CLIENT_OR_PEER 0  // 0: Client mode; 1: Peer mode
#if CLIENT_OR_PEER == 0
#define MODE "client"
#define LOCATOR ""  // If empty, it will scout
#elif CLIENT_OR_PEER == 1
#define MODE "peer"
#define LOCATOR "udp/224.0.0.224:7447#iface=en0"
#else
#error "Unknown Zenoh operation mode. Check CLIENT_OR_PEER value."
#endif

// Variables for Zenoh
static z_owned_session_t zenoh_session = {0};
static z_owned_publisher_t zenoh_publisher = {0};

// Add error code definitions for Zenoh
#define Z_ERROR_GENERIC -1      // Generic error
#define Z_ERROR_INVALID_LOCATOR -2
#define Z_ERROR_INVALID_SESSION -87  // EREMOTEIO - Remote I/O error
#define Z_ERROR_CONNECTION_FAILED -111 // ECONNREFUSED

// Function to convert Zenoh error code to string
const char* zenoh_error_to_str(int error_code) {
    switch (error_code) {
        case Z_ERROR_GENERIC:
            return "Generic Zenoh error";
        case Z_ERROR_INVALID_LOCATOR:
            return "Invalid locator";
        case Z_ERROR_INVALID_SESSION:
            return "Invalid session or remote I/O error (EREMOTEIO)";
        case Z_ERROR_CONNECTION_FAILED:
            return "Connection refused";
        default:
            return "Unknown error";
    }
}

// Configuración e inicialización de Zenoh
static bool zenoh_init(void) {
    ESP_LOGI(TAG, "Initializing Zenoh...");
    
    // Configure Zenoh
    z_owned_config_t config;
    z_config_default(&config);
    
    // Add basic configuration
    zp_config_insert(z_loan_mut(config), Z_CONFIG_MODE_KEY, MODE);
    ESP_LOGI(TAG, "Zenoh mode set to: %s", MODE);
    
    if (strcmp(LOCATOR, "") != 0) {
        if (strcmp(MODE, "client") == 0) {
            zp_config_insert(z_loan_mut(config), Z_CONFIG_CONNECT_KEY, LOCATOR);
            ESP_LOGI(TAG, "Zenoh connect endpoint: %s", LOCATOR);
        } else {
            zp_config_insert(z_loan_mut(config), Z_CONFIG_LISTEN_KEY, LOCATOR);
            ESP_LOGI(TAG, "Zenoh listen endpoint: %s", LOCATOR);
        }
    } else {
        ESP_LOGI(TAG, "Zenoh will use scouting to discover peers/routers");
    }
    
    // Set a specific timeout for scout
    zp_config_insert(z_loan_mut(config), Z_CONFIG_SCOUTING_TIMEOUT_KEY, "3000");
    ESP_LOGI(TAG, "Zenoh scout timeout set to 3000ms");
    
    // Enable multicast if in client mode with empty locator
    // if (strcmp(MODE, "client") == 0 && strcmp(LOCATOR, "") == 0) {
    //     zp_config_insert(z_loan_mut(config), Z_CONFIG_MULTICAST_SCOUTING_KEY, "true");
    //     ESP_LOGI(TAG, "Zenoh multicast scouting enabled");
    // }

    // Open session with multiple retries
    int max_retries = 3;
    int retry_count = 0;
    int return_value = 0;
    
    while (retry_count < max_retries) {
        ESP_LOGI(TAG, "Attempt %d/%d to open Zenoh session", retry_count + 1, max_retries);
        
        return_value = z_open(&zenoh_session, z_move(config), NULL);
        if (return_value < 0) {
            ESP_LOGE(TAG, "Error %d (%s) opening Zenoh session. Retrying in 1 second...", 
                    return_value, zenoh_error_to_str(return_value));
            retry_count++;
            
            // Recreate config for next attempt
            z_config_default(&config);
            zp_config_insert(z_loan_mut(config), Z_CONFIG_MODE_KEY, MODE);
            
            if (strcmp(LOCATOR, "") != 0) {
                if (strcmp(MODE, "client") == 0) {
                    zp_config_insert(z_loan_mut(config), Z_CONFIG_CONNECT_KEY, LOCATOR);
                } else {
                    zp_config_insert(z_loan_mut(config), Z_CONFIG_LISTEN_KEY, LOCATOR);
                }
            }
            
            // Add scout timeout for next attempt
            zp_config_insert(z_loan_mut(config), Z_CONFIG_SCOUTING_TIMEOUT_KEY, "3000");
            
            // Wait before retrying
            vTaskDelay(pdMS_TO_TICKS(1000));
        } else {
            break; // Success!
        }
    }
    
    if (return_value < 0) {
        ESP_LOGE(TAG, "All attempts to open Zenoh session failed. Last error: %d (%s)", 
                return_value, zenoh_error_to_str(return_value));
        return false;
    }
    
    ESP_LOGI(TAG, "Zenoh session opened successfully");
    
    // For error -87 (EREMOTEIO), we'll skip starting tasks and just try to declare the publisher
    if (return_value == Z_ERROR_INVALID_SESSION) {
        ESP_LOGW(TAG, "Due to error code -87, skipping read/lease tasks");
    } else {
        // Start Zenoh tasks
        ESP_LOGI(TAG, "Starting Zenoh read task");
        zp_start_read_task(z_loan_mut(zenoh_session), NULL);
        
        ESP_LOGI(TAG, "Starting Zenoh lease task");
        zp_start_lease_task(z_loan_mut(zenoh_session), NULL);
    }

    // Declare publisher
    ESP_LOGI(TAG, "Declaring publisher for topic: %s", ZENOH_FTM_TOPIC);
    z_view_keyexpr_t ke;
    z_view_keyexpr_from_str_unchecked(&ke, ZENOH_FTM_TOPIC);
    
    int pub_result = z_declare_publisher(z_loan(zenoh_session), &zenoh_publisher, z_loan(ke), NULL);
    if (pub_result < 0) {
        ESP_LOGE(TAG, "Error %d (%s) declaring Zenoh publisher", 
                pub_result, zenoh_error_to_str(pub_result));
        return false;
    }
    
    ESP_LOGI(TAG, "Zenoh initialized successfully. Publisher declared on %s", ZENOH_FTM_TOPIC);
    return true;
}

static float normalize(float value, float min, float max) {
    float range = max - min;
    return 2 * ((value - min) / range) - 1;
}

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "WiFi started");
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) {
        ESP_LOGI(TAG, "Connected to AP");
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGW(TAG, "Disconnected from AP");
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE) {
        ESP_LOGI(TAG_STA, "Scan completed");
        xEventGroupSetBits(wifi_event_group, SCAN_DONE_BIT);
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
    }
}

static void print_auth_mode(int authmode) {
    switch (authmode) {
    case WIFI_AUTH_OPEN:
        ESP_LOGI(TAG_STA, "Auth mode: WIFI_AUTH_OPEN");
        break;
    case WIFI_AUTH_WEP:
        ESP_LOGI(TAG_STA, "Auth mode: WIFI_AUTH_WEP");
        break;
    case WIFI_AUTH_WPA_PSK:
        ESP_LOGI(TAG_STA, "Auth mode: WIFI_AUTH_WPA_PSK");
        break;
    case WIFI_AUTH_WPA2_PSK:
        ESP_LOGI(TAG_STA, "Auth mode: WIFI_AUTH_WPA2_PSK");
        break;
    case WIFI_AUTH_WPA_WPA2_PSK:
        ESP_LOGI(TAG_STA, "Auth mode: WIFI_AUTH_WPA_WPA2_PSK");
        break;
    default:
        ESP_LOGI(TAG_STA, "Auth mode: %d", authmode);
        break;
    }
}

// Función para imprimir BSSID como string
static void print_mac(uint8_t* mac, char* buffer) {
    sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

static void dump_ap_record(wifi_ap_record_t* ap, int index) {
    char bssid_str[18];
    print_mac(ap->bssid, bssid_str);
    
    ESP_LOGI(TAG_STA, "----------------------------------");
    ESP_LOGI(TAG_STA, "AP %d Details:", index);
    ESP_LOGI(TAG_STA, "SSID: %s", ap->ssid);
    ESP_LOGI(TAG_STA, "BSSID: %s", bssid_str);
    ESP_LOGI(TAG_STA, "Channel: %d (2nd: %d)", ap->primary, ap->second);
    ESP_LOGI(TAG_STA, "RSSI: %d", ap->rssi);
    ESP_LOGI(TAG_STA, "FTM Responder: %s", ap->ftm_responder ? "Yes" : "No");
    print_auth_mode(ap->authmode);
    ESP_LOGI(TAG_STA, "WPS: %s", ap->wps ? "Yes" : "No");
    ESP_LOGI(TAG_STA, "Country: %.3s", ap->country.cc);
    ESP_LOGI(TAG_STA, "----------------------------------");
}

static void wifi_init_sta(void) {
    ESP_ERROR_CHECK(esp_netif_init());
    wifi_event_group = xEventGroupCreate();
    ftm_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    // Register event handlers
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));
    
    // Set WiFi mode and start WiFi (but don't connect yet)
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    
    // Configure power and mode for 40MHz - exactly like the anchor
    ESP_ERROR_CHECK(esp_wifi_set_max_tx_power(84)); // 84 = max power (20.5 dBm)
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
    ESP_ERROR_CHECK(esp_wifi_set_bandwidth(WIFI_IF_STA, WIFI_BW_HT40));
    ESP_ERROR_CHECK(esp_wifi_set_channel(FTM_TARGET_CHANNEL, FTM_SECOND_CHANNEL));
    
    // Set protocol to ensure compatibility - include all protocols
    ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_STA, 
        WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N | WIFI_PROTOCOL_LR));
}

static void disconnect_from_wifi(void) {
    ESP_LOGI(TAG, "Disconnecting from WiFi");
    ESP_ERROR_CHECK(esp_wifi_disconnect());
    vTaskDelay(pdMS_TO_TICKS(500)); // Wait to ensure disconnection
}

// Modified FTM report handler to store data instead of publishing immediately
static void ftm_report_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    wifi_event_ftm_report_t *event = (wifi_event_ftm_report_t *) event_data;
    
    if (event->status != FTM_STATUS_SUCCESS) {
        ESP_LOGW(TAG, "FTM failed with status %d", event->status);
        xEventGroupSetBits(ftm_event_group, FTM_FAILURE_BIT);
        return;
    }

    // Skip first measurement after reboot, as it might be unstable
    if (just_reboot == 1) {
        just_reboot = 0;
        xEventGroupSetBits(ftm_event_group, FTM_FAILURE_BIT);
        return;
    }

    // Create a safe copy of the report data
    if (g_ftm_report != NULL) {
        free(g_ftm_report);
        g_ftm_report = NULL;
    }

    g_ftm_report = malloc(sizeof(wifi_ftm_report_entry_t) * event->ftm_report_num_entries);
    if (g_ftm_report == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for FTM report");
        xEventGroupSetBits(ftm_event_group, FTM_FAILURE_BIT);
        return;
    }

    // Safely copy the data
    memcpy(g_ftm_report, event->ftm_report_data, sizeof(wifi_ftm_report_entry_t) * event->ftm_report_num_entries);
    g_ftm_report_num_entries = event->ftm_report_num_entries;

    // Prepare MAC en formato string
    char mac_str[18];
    sprintf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x",
            event->peer_mac[0], event->peer_mac[1], event->peer_mac[2],
            event->peer_mac[3], event->peer_mac[4], event->peer_mac[5]);
    
    // Get SSID of current anchor (for local logging, not for JSON)
    char ssid[33] = "unknown";
    if (current_anchor < num_anchors) {
        strncpy(ssid, (char*)anchors[current_anchor].ssid, sizeof(ssid) - 1);
        ssid[sizeof(ssid) - 1] = '\0';
    }
    
    ESP_LOGI(TAG, "FTM report from %s with %d entries", mac_str, g_ftm_report_num_entries);
    ESP_LOGI(TAG, "RTT: %lu, RAW: %lu, DIST: %lu", event->rtt_est, event->rtt_raw, event->dist_est);

    // Calculate average RSSI
    int totalRss = 0;
    for (int i = 0; i < g_ftm_report_num_entries; i++) {
        totalRss += g_ftm_report[i].rssi;
        ESP_LOGI(TAG, "[%d] RTT: %lu, RSSI: %d, T1: %lu, T2: %lu, T3: %lu, T4: %lu", 
                i, g_ftm_report[i].rtt, g_ftm_report[i].rssi, 
                g_ftm_report[i].t1, g_ftm_report[i].t2, g_ftm_report[i].t3, g_ftm_report[i].t4);
    }
    
    float meanRss = g_ftm_report_num_entries > 0 ? totalRss / (float)g_ftm_report_num_entries : -100.0f;

    // Create and initialize ML inputs/outputs
    X = emxCreate_real_T(1, 2);
    if (X == NULL) {
        ESP_LOGE(TAG, "Failed to allocate X array");
        goto cleanup;
    }
    
    X->data[0] = normalize(event->rtt_raw, MIN_RTT, MAX_RTT);
    X->data[1] = normalize(meanRss, MIN_RSS, MAX_RSS);
    
    emxInitArray_real_T(&result, 1);
    if (result == NULL) {
        ESP_LOGE(TAG, "Failed to initialize result array");
        goto cleanup;
    }
    
    // Run prediction
    predict(X, result);
    int resultCm = (int)(result->data[0] * 100.0);

    // Populate the global buffer with processed data
    memset(&g_ftm_data_buffer, 0, sizeof(ftm_composite_data_t)); // Clear previous data

    strncpy(g_ftm_data_buffer.anchor_id, mac_str, sizeof(g_ftm_data_buffer.anchor_id) - 1);
    g_ftm_data_buffer.rtt_est_ps = event->rtt_est;
    g_ftm_data_buffer.rtt_raw_ps = event->rtt_raw;
    g_ftm_data_buffer.dist_est_m = (float)event->dist_est / 100.0f; // Convert cm to m
    g_ftm_data_buffer.own_est_m = (float)resultCm / 100.0f;      // Convert cm to m
    
    g_ftm_data_buffer.num_frames = 0;
    if (g_ftm_report_num_entries > 0 && g_ftm_report != NULL) {
        int frames_to_copy = (g_ftm_report_num_entries < MAX_FTM_FRAMES_TO_STORE) ? g_ftm_report_num_entries : MAX_FTM_FRAMES_TO_STORE;
        g_ftm_data_buffer.num_frames = frames_to_copy;
        for (int i = 0; i < frames_to_copy; i++) {
            g_ftm_data_buffer.frames[i].rssi = g_ftm_report[i].rssi;
            g_ftm_data_buffer.frames[i].rtt = g_ftm_report[i].rtt;
            g_ftm_data_buffer.frames[i].t1 = g_ftm_report[i].t1;
            g_ftm_data_buffer.frames[i].t2 = g_ftm_report[i].t2;
            g_ftm_data_buffer.frames[i].t3 = g_ftm_report[i].t3;
            g_ftm_data_buffer.frames[i].t4 = g_ftm_report[i].t4;
        }
    }
    g_ftm_data_buffer.valid = true;

    ESP_LOGI(TAG, "Processed FTM report for %s: rtt_raw_ps=%lu, own_est_m=%.2f, num_frames=%ld",
             g_ftm_data_buffer.anchor_id, g_ftm_data_buffer.rtt_raw_ps, g_ftm_data_buffer.own_est_m, (long)g_ftm_data_buffer.num_frames);

    // Simplificar la salida para evitar errores de formato
    printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\\n", 
           event->peer_mac[0], event->peer_mac[1], event->peer_mac[2],
           event->peer_mac[3], event->peer_mac[4], event->peer_mac[5]);
    printf("SSID: %s\\n", ssid);
    printf("RTT_EST: %lu\\n", event->rtt_est);
    printf("RTT_RAW: %lu\\n", event->rtt_raw);
    printf("DIST_EST: %lu\\n", event->dist_est);
    printf("OWN_EST_CM: %d\\n", resultCm);
    printf("RSSI: %d\\n", (int)meanRss);

    // Store data for later transmission
    // store_ftm_measurement(mac_str, ssid, event->rtt_raw, event->rtt_est, 
    //                       event->dist_est, resultCm, (int8_t)meanRss);
    // No longer calling store_ftm_measurement, data is in g_ftm_data_buffer

cleanup:
    // Cleanup memory
    if (result != NULL) {
        emxDestroyArray_real_T(result);
    }
    
    if (X != NULL) {
        emxDestroyArray_real_T(X);
    }
    
    // Signal completion
    xEventGroupSetBits(ftm_event_group, FTM_REPORT_BIT);
}

// Implement wifi_perform_scan from original code
static bool wifi_perform_scan(void) {
    // Configure scan parameters for FTM AP detection on channel 6
    wifi_scan_config_t scan_config = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = FTM_TARGET_CHANNEL,  // Scan specifically on channel 6 like the anchor
        .show_hidden = true,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE
    };
    
    ESP_LOGI(TAG_STA, "Starting WiFi scan on channel %d with 40MHz bandwidth...", FTM_TARGET_CHANNEL);
    
    // Ensure we use channel 6 and 40MHz before scanning - same as anchor
    ESP_ERROR_CHECK(esp_wifi_set_bandwidth(WIFI_IF_STA, WIFI_BW_HT40));
    ESP_ERROR_CHECK(esp_wifi_set_channel(FTM_TARGET_CHANNEL, FTM_SECOND_CHANNEL));
    xEventGroupClearBits(wifi_event_group, SCAN_DONE_BIT);
    
    // Start scan
    ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));
    
    uint16_t ap_count = 0;
    esp_wifi_scan_get_ap_num(&ap_count);
    
    if (ap_count == 0) {
        ESP_LOGI(TAG_STA, "No matching AP found on channel %d", FTM_TARGET_CHANNEL);
        return false;
    }
    
    // Clear existing AP records
    memset(s_ap_records, 0, sizeof(s_ap_records));
    
    uint16_t number = DEFAULT_SCAN_LIST_SIZE;
    if (esp_wifi_scan_get_ap_records(&number, s_ap_records) == ESP_OK) {
        num_anchors = 0;
        for (int i = 0; i < number; i++) {
            dump_ap_record(&s_ap_records[i], i);
            
            bool is_ftm_ap = false;
            
            // Check if SSID starts with FTM prefix (like configured in the anchor)
            if (strncmp((char*)s_ap_records[i].ssid, FTM_AP_PREFIX, strlen(FTM_AP_PREFIX)) == 0) {
                ESP_LOGI(TAG_STA, "AP has FTM prefix in SSID: %s", s_ap_records[i].ssid);
                is_ftm_ap = true;
            }
            
            // Check FTM responder flag - must be true like in the anchor
            if (s_ap_records[i].ftm_responder) {
                ESP_LOGI(TAG_STA, "AP has FTM responder flag");
                is_ftm_ap = true;
            } else {
                ESP_LOGW(TAG_STA, "AP does not have FTM responder flag. Verify that ftm_responder=true on the anchor");
            }
            
            // Check if it's on channel 6 like the anchor
            if (s_ap_records[i].primary != FTM_TARGET_CHANNEL) {
                ESP_LOGW(TAG_STA, "AP is on channel %d instead of %d", 
                         s_ap_records[i].primary, FTM_TARGET_CHANNEL);
            }
            
            #if TREAT_ALL_AP_AS_FTM
            // Debug mode: treat all APs as FTM - useful for tests
            ESP_LOGI(TAG_STA, "Debug mode: treating all APs as FTM responders");
            is_ftm_ap = true;
            #endif
                
            if (is_ftm_ap && num_anchors < MAX_ANCHORS) {
                memcpy(&anchors[num_anchors], &s_ap_records[i], sizeof(wifi_ap_record_t));
                ESP_LOGI(TAG_STA, "Added AP %d as anchor %d", i, num_anchors);
                num_anchors++;
            }
        }
    }
    
    ESP_LOGI(TAG_STA, "Scan completed, found %d anchors", num_anchors);
    return num_anchors > 0;
}

// Start FTM session with a specific AP - simpler version based on original code
static void start_ftm_session(int anchor_idx) {
    if (anchor_idx < 0 || anchor_idx >= num_anchors) {
        ESP_LOGE(TAG, "Invalid anchor index");
        return;
    }
    
    // Reset event group bits before starting new session
    xEventGroupClearBits(ftm_event_group, FTM_REPORT_BIT | FTM_FAILURE_BIT);
    
    // FTM configuration that matches the anchor
    wifi_ftm_initiator_cfg_t ftmi_cfg = {
        .frm_count = 16,
        .burst_period = 2
    };
    
    memcpy(ftmi_cfg.resp_mac, anchors[anchor_idx].bssid, 6);
    ftmi_cfg.channel = FTM_TARGET_CHANNEL;
    
    char bssid_str[18];
    sprintf(bssid_str, "%02x:%02x:%02x:%02x:%02x:%02x",
            anchors[anchor_idx].bssid[0], anchors[anchor_idx].bssid[1],
            anchors[anchor_idx].bssid[2], anchors[anchor_idx].bssid[3],
            anchors[anchor_idx].bssid[4], anchors[anchor_idx].bssid[5]);
            
    ESP_LOGI(TAG, "Starting FTM with %s (SSID: %s) on channel %d with 40MHz bandwidth",
             bssid_str, anchors[anchor_idx].ssid, FTM_TARGET_CHANNEL);
             
    // Explicitly check in which channel the AP is and warn if it doesn't match
    if (anchors[anchor_idx].primary != FTM_TARGET_CHANNEL) {
        ESP_LOGW(TAG, "WARNING: AP is on channel %d but we are trying on channel %d", 
                 anchors[anchor_idx].primary, FTM_TARGET_CHANNEL);
    }
    
    // Explicitly check if the AP has FTM responder enabled
    if (!anchors[anchor_idx].ftm_responder) {
        ESP_LOGW(TAG, "WARNING: AP does not have FTM Responder enabled according to scan");
        ESP_LOGW(TAG, "Verify that the anchor has ftm_responder = true configured");
    }
             
    // Explicitly configure the channel and bandwidth before starting FTM
    ESP_ERROR_CHECK(esp_wifi_set_channel(FTM_TARGET_CHANNEL, FTM_SECOND_CHANNEL));
    ESP_ERROR_CHECK(esp_wifi_set_bandwidth(WIFI_IF_STA, WIFI_BW_HT40));
    
    // Short pause to ensure channel is stable before starting FTM
    vTaskDelay(pdMS_TO_TICKS(100));
    
    // Start the FTM session
    esp_err_t err = esp_wifi_ftm_initiate_session(&ftmi_cfg);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start FTM session: %d", err);
        
        switch (err) {
            case ESP_ERR_WIFI_NOT_INIT:
                ESP_LOGE(TAG, "WiFi not initialized");
                break;
            case ESP_ERR_WIFI_NOT_STARTED:
                ESP_LOGE(TAG, "WiFi not started");
                break;
            case ESP_ERR_INVALID_ARG:
                ESP_LOGE(TAG, "Invalid argument in FTM config");
                break;
            case ESP_ERR_NO_MEM:
                ESP_LOGE(TAG, "Out of memory");
                break;
            case ESP_FAIL:
                ESP_LOGE(TAG, "FTM failed - AP may not support FTM or may be disconnected");
                break;
            default:
                ESP_LOGE(TAG, "Other error: %d", err);
        }
        
        xEventGroupSetBits(ftm_event_group, FTM_FAILURE_BIT);
    } else {
        ESP_LOGI(TAG, "FTM session initiated successfully");
    }
}

// Connect to WiFi, initialize Zenoh, send data, and disconnect
static void connect_send_disconnect(void) {
    static char json_message_buffer[3072]; // Moved back inside the function
    if (!g_ftm_data_buffer.valid) {
        ESP_LOGI(TAG, "No valid measurement to send, skipping WiFi connection");
        return;
    }
    
    ESP_LOGI(TAG, "Connecting to WiFi to send measurement data...");
    
    // Connect to WiFi
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
        },
    };
    
    ESP_LOGI(TAG, "Connecting to WiFi '%s'...", WIFI_SSID);
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_connect());
    
    // Wait for connection
    EventBits_t bits = xEventGroupWaitBits(
        wifi_event_group,
        CONNECTED_BIT,
        pdFALSE,
        pdFALSE,
        pdMS_TO_TICKS(10000)); // 10 seconds of timeout
        
    if (!(bits & CONNECTED_BIT)) {
        ESP_LOGW(TAG, "Failed to connect to WiFi within timeout");
        return;
    }
    
    ESP_LOGI(TAG, "Connected to WiFi, initializing Zenoh...");
    
    // Initialize Zenoh with multiple attempts
    bool zenoh_initialized = false;
    for (int attempt = 0; attempt < 3; attempt++) {
        ESP_LOGI(TAG, "Zenoh initialization attempt %d/3", attempt + 1);
        
        if (zenoh_init()) {
            zenoh_initialized = true;
            break;
        }
        
        ESP_LOGW(TAG, "Failed to initialize Zenoh, retrying in 1 second");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    if (!zenoh_initialized) {
        ESP_LOGE(TAG, "Failed to initialize Zenoh after multiple attempts");
        disconnect_from_wifi();
        return;
    }
    
    // Give Zenoh some time to initialize
    vTaskDelay(pdMS_TO_TICKS(500));
    
    // Publish data
    ESP_LOGI(TAG, "Publishing measurement data via Zenoh...");
    
    // Check for valid session before publishing
    if (z_session_is_closed(z_loan(zenoh_session))) {
        ESP_LOGE(TAG, "Zenoh session is invalid or closed, cannot publish");
    } else {
        // Try to publish with error handling
        bool published = false;
        for (int pub_attempt = 0; pub_attempt < 3; pub_attempt++) {
            ESP_LOGI(TAG, "Publishing attempt %d/3", pub_attempt + 1);
            
            // Create JSON message
            memset(json_message_buffer, 0, sizeof(json_message_buffer));
            char* p = json_message_buffer;
            char* end = json_message_buffer + sizeof(json_message_buffer);

            // Construct the new JSON
            p += snprintf(p, end - p, "{\"anchor_id\":\"%s\",\"rtt_est\":%lu,\"rtt_raw\":%lu,\"dist_est\":%.2f,\"own_est\":%.2f,\"num_frames\":%ld,\"frames\":[",
                          g_ftm_data_buffer.anchor_id,
                          (unsigned long)g_ftm_data_buffer.rtt_est_ps,
                          (unsigned long)g_ftm_data_buffer.rtt_raw_ps,
                          g_ftm_data_buffer.dist_est_m,
                          g_ftm_data_buffer.own_est_m,
                          (long)g_ftm_data_buffer.num_frames);

            for (int i = 0; i < g_ftm_data_buffer.num_frames; i++) {
                if (p >= end - 200) { // Check buffer space before adding a frame, 200 is a conservative estimate
                    ESP_LOGE(TAG, "JSON buffer too small for all frames");
                    break; 
                }
                p += snprintf(p, end - p, "%s{\"rssi\":%ld,\"rtt\":%lu,\"t1\":%llu,\"t2\":%llu,\"t3\":%llu,\"t4\":%llu}",
                              (i > 0) ? "," : "",
                              (long)g_ftm_data_buffer.frames[i].rssi,
                              (unsigned long)g_ftm_data_buffer.frames[i].rtt,
                              (unsigned long long)g_ftm_data_buffer.frames[i].t1,
                              (unsigned long long)g_ftm_data_buffer.frames[i].t2,
                              (unsigned long long)g_ftm_data_buffer.frames[i].t3,
                              (unsigned long long)g_ftm_data_buffer.frames[i].t4);
            }
            if (p < end -1 ) p += snprintf(p, end - p, "]}"); else { /* handle error or truncate */ *(end-1) = 0;}


            ESP_LOGI(TAG, "Publishing: %s", json_message_buffer);
            
            // Create payload and publish
            z_owned_bytes_t payload;
            z_bytes_copy_from_str(&payload, json_message_buffer);
            
            int pub_result = z_publisher_put(z_loan(zenoh_publisher), z_move(payload), NULL);
            if (pub_result < 0) {
                ESP_LOGE(TAG, "Failed to publish, error: %d (%s)", 
                         pub_result, zenoh_error_to_str(pub_result));
                
                if (pub_attempt < 2) {
                    ESP_LOGI(TAG, "Retrying publication in 500ms");
                    vTaskDelay(pdMS_TO_TICKS(500));
                }
            } else {
                ESP_LOGI(TAG, "Publication successful");
                published = true;
                break;
            }
        }
        
        if (!published) {
            ESP_LOGE(TAG, "Failed to publish measurement data after multiple attempts");
        }
    }
    
    // Give some time for the message to be sent
    vTaskDelay(pdMS_TO_TICKS(500));
    
    // Cleanup Zenoh
    if (!z_session_is_closed(z_loan(zenoh_session))) {
        ESP_LOGI(TAG, "Closing Zenoh session...");
        z_drop(z_move(zenoh_publisher));
        z_drop(z_move(zenoh_session));
    }
    
    // Disconnect from WiFi
    ESP_LOGI(TAG, "Disconnecting from WiFi...");
    disconnect_from_wifi();
    
    // Mark measurement as sent (even if it failed, to avoid retrying forever)
    g_ftm_data_buffer.valid = false;
    
    ESP_LOGI(TAG, "Measurement sent and WiFi disconnected, returning to FTM scanning");
}

void app_main(void) {
    ESP_LOGI(TAG, "\n\n==== ESP32-S2 FTM TAG STARTING ====\n");
    
    // Check if this is a SW reboot
    just_reboot = (esp_reset_reason() == ESP_RST_SW) ? 1 : 0;
    
    // Initialize NVS flash
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "NVS needs to be erased, retrying...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    } else {
        ESP_ERROR_CHECK(ret);
    }

    // Log device info
    uint8_t mac[6];
    ESP_ERROR_CHECK(esp_base_mac_addr_get(&mac[0]));
    ESP_LOGI(TAG, "MAC Address: %02X:%02X:%02X:%02X:%02X:%02X",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    // Initialize WiFi in STA mode
    wifi_init_sta();
    
    // Register FTM handler
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_FTM_REPORT, &ftm_report_handler, NULL));
    
    // Allow system to stabilize
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    ESP_LOGI(TAG, "Searching for FTM responders on channel %d with 40MHz bandwidth", FTM_TARGET_CHANNEL);
    ESP_LOGI(TAG, "Ensure anchor has FTM configured and is on channel %d", FTM_TARGET_CHANNEL);
    
    // Initial scan to find FTM responders
    bool found_anchors = wifi_perform_scan();
    
    // If we don't find anchors on the first try, try a few more times
    int scan_attempts = 1;
    while (!found_anchors && scan_attempts < 5) {
        ESP_LOGI(TAG, "Attempt %d to scan for FTM responders", scan_attempts + 1);
        vTaskDelay(pdMS_TO_TICKS(1000));
        found_anchors = wifi_perform_scan();
        scan_attempts++;
    }
    
    if (num_anchors == 0) {
        ESP_LOGE(TAG, "No FTM anchors found after %d attempts", scan_attempts);
        ESP_LOGI(TAG, "Ensure AP has FTM Responder enabled and is on channel %d", FTM_TARGET_CHANNEL);
        ESP_LOGI(TAG, "Verify that SSID starts with '%s'", FTM_AP_PREFIX);
        return;
    }

    // Main loop for FTM measurements
    current_anchor = 0;
    const TickType_t xTicksToWaitFTM = 500 / portTICK_PERIOD_MS;
    
    ESP_LOGI(TAG, "Starting FTM measurements with %d anchors", num_anchors);
    
    // Clear measurement data
    // memset(&last_measurement, 0, sizeof(ftm_measurement_t));
    memset(&g_ftm_data_buffer, 0, sizeof(ftm_composite_data_t));
    
    // Counter to track measurements
    int measurement_count = 0;
    const int MEASUREMENTS_BEFORE_SEND = 5; // Send data every X measurements
    
    for (;;) {
        // Periodically rescan to ensure we have an updated list
        if (current_anchor == 0) {
            ESP_LOGI(TAG, "Refreshing FTM AP responders list...");
            wifi_perform_scan();
            if (num_anchors == 0) {
                ESP_LOGW(TAG, "No anchors found in rescan. Waiting...");
                vTaskDelay(pdMS_TO_TICKS(2000));
                continue;
            }
        }
        
        // Start FTM session with current anchor
        start_ftm_session(current_anchor);
        
        EventBits_t bits = xEventGroupWaitBits(
            ftm_event_group, 
            FTM_REPORT_BIT | FTM_FAILURE_BIT,
            pdFALSE, pdFALSE, 
            xTicksToWaitFTM);
        
        if (bits & FTM_REPORT_BIT) {
            xEventGroupClearBits(ftm_event_group, FTM_REPORT_BIT);
            if (g_ftm_report != NULL) {
                free(g_ftm_report);
                g_ftm_report = NULL;
                g_ftm_report_num_entries = 0;
            }
            
            measurement_count++;
            
            // If we have a valid measurement and reached the count threshold, 
            // connect to WiFi and send data
            if (g_ftm_data_buffer.valid && measurement_count >= MEASUREMENTS_BEFORE_SEND) {
                measurement_count = 0;
                connect_send_disconnect();
            }
            
            vTaskDelay(20 / portTICK_PERIOD_MS);
        } else if (bits & FTM_FAILURE_BIT) {
            xEventGroupClearBits(ftm_event_group, FTM_FAILURE_BIT);
            
            // Clean up memory
            if (result != NULL) {
                emxDestroyArray_real_T(result);
            }
            if (X != NULL) {
                emxDestroyArray_real_T(X);
            }
            if (g_ftm_report != NULL) {
                free(g_ftm_report);
                g_ftm_report = NULL;
                g_ftm_report_num_entries = 0;
            }
            
            ESP_LOGW(TAG, "FTM session failed with anchor %d, trying next one", current_anchor);
            
            // It might be better to rescan than to restart
            if (++current_anchor >= num_anchors) {
                current_anchor = 0;
                wifi_perform_scan(); // Rescan to refresh AP list
            }
            
            vTaskDelay(100 / portTICK_PERIOD_MS);
        } else {
            // No event received within timeout, move to next anchor
            ESP_LOGW(TAG, "FTM session timeout with anchor %d, trying next one", current_anchor);
            if (++current_anchor >= num_anchors) {
                current_anchor = 0;
            }
            vTaskDelay(20 / portTICK_PERIOD_MS);
        }
        
        // Reset watchdog to prevent timeouts
        esp_task_wdt_reset();
    }
}
