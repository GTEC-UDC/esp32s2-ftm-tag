#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "cmd_system.h"
#include "argtable3/argtable3.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_wifi.h"
#include "esp_console.h"
#include "esp_task_wdt.h"
#include "predict.h"
#include "predict_emxAPI.h"
#include "predict_terminate.h"
#include "predict_types.h"
#include "rt_nonfinite.h"

#define MAX_ANCHORS 8
#define USE_CSV 1

#define MIN_RTT 0.0
#define MAX_RTT 220.0

#define MIN_RSS -9.0
#define MAX_RSS -50.0


typedef struct {
    struct arg_str *ssid;
    struct arg_str *password;
    struct arg_end *end;
} wifi_args_t;

typedef struct {
    struct arg_str *ssid;
    struct arg_end *end;
} wifi_scan_arg_t;

typedef struct {
    struct arg_lit *mode;
    struct arg_int *frm_count;
    struct arg_int *burst_period;
    struct arg_str *ssid;
    struct arg_end *end;
} wifi_ftm_args_t;


static bool s_reconnect = true;
static const char *TAG_STA = "ftm_tag";


static EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT = BIT0;
const int DISCONNECTED_BIT = BIT1;

static EventGroupHandle_t ftm_event_group;
const int FTM_REPORT_BIT = BIT0;
const int FTM_FAILURE_BIT = BIT1;
wifi_ftm_report_entry_t *g_ftm_report;
uint8_t g_ftm_report_num_entries;

uint16_t g_scan_ap_num;
wifi_ap_record_t *g_ap_list_buffer;


wifi_ap_record_t anchors[MAX_ANCHORS];
uint8_t num_anchors, current_anchor;

uint8_t just_reboot= 0;

emxArray_real_T *X;
emxArray_real_T *result;

ESP_EVENT_DEFINE_BASE(END_SCAN_OR_FTM_EVENT);


static float normalize(float value, float min, float max){
    float range, norm;

    range = max -min;
    norm = (value -min) / range;
    return 2*norm -1;
}

// static void hard_restart() {
//   esp_task_wdt_init(1,true);
//   esp_task_wdt_add(NULL);
//   while(true);
// }

static void wifi_connected_handler(void *arg, esp_event_base_t event_base,
                                   int32_t event_id, void *event_data)
{
    //wifi_event_sta_connected_t *event = (wifi_event_sta_connected_t *)event_data;

    xEventGroupClearBits(wifi_event_group, DISCONNECTED_BIT);
    xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
}

static void disconnect_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if (s_reconnect) {
        //ESP_LOGI(TAG_STA, "sta disconnect, s_reconnect...");
        esp_wifi_connect();
    } else {
        //ESP_LOGI(TAG_STA, "sta disconnect");
    }
    xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
    xEventGroupSetBits(wifi_event_group, DISCONNECTED_BIT);
}

static void ftm_report_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    int i;  
    int totalRss;
    float meanRss;
    int resultCm;
    wifi_event_ftm_report_t *event = (wifi_event_ftm_report_t *) event_data;

    ESP_LOGI(TAG_STA, "FTM HANDLER");
    if (event->status == FTM_STATUS_SUCCESS) {

        if (just_reboot==1){
            just_reboot = 0;
            xEventGroupSetBits(ftm_event_group, FTM_FAILURE_BIT);
            return;
        }
        g_ftm_report = event->ftm_report_data;
        g_ftm_report_num_entries = event->ftm_report_num_entries;

        totalRss = 0;
        for (i = 0; i < g_ftm_report_num_entries; i++) {
            totalRss = totalRss +g_ftm_report[i].rssi;
        }

        meanRss = totalRss/g_ftm_report_num_entries;

        X = emxCreate_real_T(1, 2);
        X->data[0] = normalize(event->rtt_raw, MIN_RTT, MAX_RTT);
        X->data[1] = normalize(meanRss, MIN_RSS, MAX_RSS);
        emxInitArray_real_T(&result, 1);
        predict(X, result);

        resultCm = (int) (result->data[0]*100.0);
        //fprintf(stdout,"(%f , %f)", X->data[0], X->data[1]);

        if (USE_CSV==1){
            fprintf(stdout, "%02x:%02x:%02x:%02x:%02x:%02x,%ld,%ld,%ld,%d,%d,", 
                event->peer_mac[0], event->peer_mac[1], event->peer_mac[2],
                event->peer_mac[3], event->peer_mac[4], event->peer_mac[5],
                event->rtt_est, event->rtt_raw, event->dist_est, resultCm, event->ftm_report_num_entries);
        } else {
            fprintf(stdout, "{\"id\":\"%02x:%02x:%02x:%02x:%02x:%02x\", \"rtt_est\":%ld, \"rtt_raw\":%ld , \"dist_est\":%ld, \"own_est\":%d,\"num_frames\":%d, \"frames\":[", 
                event->peer_mac[0], event->peer_mac[1], event->peer_mac[2],
                event->peer_mac[3], event->peer_mac[4], event->peer_mac[5],
                event->rtt_est, event->rtt_raw, event->dist_est, resultCm, event->ftm_report_num_entries);
        }
        
        for (i = 0; i < g_ftm_report_num_entries; i++) {
            if (USE_CSV==1){
                fprintf(stdout,"%ld,%d,%lld,%lld,%lld,%lld",g_ftm_report[i].rtt, g_ftm_report[i].rssi, g_ftm_report[i].t1,g_ftm_report[i].t2,g_ftm_report[i].t3,g_ftm_report[i].t4);
            } else {
                fprintf(stdout,"{\"rtt\":%ld, \"rssi\":%d, \"t1\":%lld, \"t2\":%lld, \"t3\":%lld, \"t4\":%lld}",g_ftm_report[i].rtt, g_ftm_report[i].rssi, g_ftm_report[i].t1,g_ftm_report[i].t2,g_ftm_report[i].t3,g_ftm_report[i].t4); 
            }

            if (USE_CSV==1 && i<g_ftm_report_num_entries-1){
                fprintf(stdout,",");
            }
        }

        if (USE_CSV==0){
            fprintf(stdout,"]");
        }

        fprintf(stdout,"\n");
        fflush(stdout);
        
        xEventGroupSetBits(ftm_event_group, FTM_REPORT_BIT);
    } else {

        if (event->status == FTM_STATUS_UNSUPPORTED){
            ESP_LOGI(TAG_STA, "FTM procedure with Peer(%02x:%02x:%02x:%02x:%02x:%02x) failed! (Status - FTM_STATUS_UNSUPPORTED)",
                event->peer_mac[0], event->peer_mac[1], event->peer_mac[2],
                event->peer_mac[3], event->peer_mac[4], event->peer_mac[5]);
        } else if (event->status == FTM_STATUS_CONF_REJECTED){
            ESP_LOGI(TAG_STA, "FTM procedure with Peer(%02x:%02x:%02x:%02x:%02x:%02x) failed! (Status - FTM_STATUS_CONF_REJECTED)",
                event->peer_mac[0], event->peer_mac[1], event->peer_mac[2],
                event->peer_mac[3], event->peer_mac[4], event->peer_mac[5]);
        } else if (event->status == FTM_STATUS_NO_RESPONSE){
            ESP_LOGI(TAG_STA, "FTM procedure with Peer(%02x:%02x:%02x:%02x:%02x:%02x) failed! (Status - FTM_STATUS_NO_RESPONSE)",
                event->peer_mac[0], event->peer_mac[1], event->peer_mac[2],
                event->peer_mac[3], event->peer_mac[4], event->peer_mac[5]);
        } else if (event->status == FTM_STATUS_FAIL){
            ESP_LOGI(TAG_STA, "FTM procedure with Peer(%02x:%02x:%02x:%02x:%02x:%02x) failed! (Status - FTM_STATUS_FAIL)",
                event->peer_mac[0], event->peer_mac[1], event->peer_mac[2],
                event->peer_mac[3], event->peer_mac[4], event->peer_mac[5]);
        }


        xEventGroupSetBits(ftm_event_group, FTM_FAILURE_BIT);
    }

}


static bool wifi_perform_scan(const char *ssid, bool internal)
{
    wifi_scan_config_t scan_config = { 0 };
    scan_config.ssid = (uint8_t *) ssid;
    uint8_t i;

    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_scan_start(&scan_config, true) );

    esp_wifi_scan_get_ap_num(&g_scan_ap_num);
    if (g_scan_ap_num == 0) {
        ESP_LOGI(TAG_STA, "No matching AP found");
        return false;
    }

    if (g_ap_list_buffer) {
        free(g_ap_list_buffer);
    }
    g_ap_list_buffer = malloc(g_scan_ap_num * sizeof(wifi_ap_record_t));
    if (g_ap_list_buffer == NULL) {
        ESP_LOGE(TAG_STA, "Failed to malloc buffer to print scan results");
        return false;
    }

    if (esp_wifi_scan_get_ap_records(&g_scan_ap_num, (wifi_ap_record_t *)g_ap_list_buffer) == ESP_OK) {
        if (!internal) {
            num_anchors = 0;
            for (i = 0; i < g_scan_ap_num; i++) {
                if (num_anchors<MAX_ANCHORS && g_ap_list_buffer[i].ftm_responder){
                    anchors[num_anchors] = g_ap_list_buffer[i];
                    num_anchors+=1;
                }
                //ESP_LOGI(TAG_STA, "[%s][rssi=%d]""%s", g_ap_list_buffer[i].ssid, g_ap_list_buffer[i].rssi, g_ap_list_buffer[i].ftm_responder ? "[FTM Responder]" : "");
            }
        }
    }
    ESP_LOGI(TAG_STA, "Scan done %d anchors found", num_anchors);
    
    return true;
}


static int do_ftm(wifi_ap_record_t *ap_record){

    ESP_LOGI(TAG_STA, "Doing FTM");
     //esp_task_wdt_reset();

    wifi_ftm_initiator_cfg_t ftmi_cfg = {
        .frm_count = 16,
        .burst_period = 1,
    };

    if (ap_record) {
        ESP_LOGI(TAG_STA, "Starting FTM with %02x:%02x:%02x:%02x:%02x:%02x on channel %d\n", 
            ap_record->bssid[0], ap_record->bssid[1], ap_record->bssid[2],
            ap_record->bssid[3], ap_record->bssid[4], ap_record->bssid[5],
            ap_record->primary);
        memcpy(ftmi_cfg.resp_mac, ap_record->bssid, 6);
        ftmi_cfg.channel = ap_record->primary;
    } else {
        ESP_LOGE(TAG_STA, "NOT AP RECORD");
        ESP_ERROR_CHECK(esp_event_post( END_SCAN_OR_FTM_EVENT, 0, NULL, 0,pdMS_TO_TICKS(100)));
        return 0;
    }

    if (ESP_OK != esp_wifi_ftm_initiate_session(&ftmi_cfg)) {
        ESP_LOGE(TAG_STA, "Failed to start FTM session");
        ESP_ERROR_CHECK(esp_event_post( END_SCAN_OR_FTM_EVENT, 0, NULL, 0,pdMS_TO_TICKS(100)));
        return 0;
    }

    return 0;
}

static int proccess_next_anchor(){

    current_anchor+=1;
    if (current_anchor>=num_anchors){
        current_anchor = 0;
    }

    ESP_LOGI(TAG_STA, "Proccess_next_anchor %d", current_anchor);
    return do_ftm(&anchors[current_anchor]);

}

void initialise_wifi(void)
{
    static bool initialized = false;

    if (initialized) {
        return;
    }

    ESP_ERROR_CHECK(esp_netif_init());
    wifi_event_group = xEventGroupCreate();
    ftm_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_create_default() );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                    WIFI_EVENT_STA_CONNECTED,
                    &wifi_connected_handler,
                    NULL,
                    NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                    WIFI_EVENT_STA_DISCONNECTED,
                    &disconnect_handler,
                    NULL,
                    NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                    WIFI_EVENT_FTM_REPORT,
                    &ftm_report_handler,
                    NULL,
                    NULL));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL) );
    ESP_ERROR_CHECK(esp_wifi_start() );
    initialized = true;
}


void app_main(void)
{


if (esp_reset_reason()==ESP_RST_SW) { 
    just_reboot = 1;
}
 

    const TickType_t xTicksToWaitFTM = 500 / portTICK_PERIOD_MS;
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    initialise_wifi();

    num_anchors = 0;
    current_anchor = -1;
    wifi_perform_scan(NULL, false);

    for (;;){
        proccess_next_anchor();
        EventBits_t bits = xEventGroupWaitBits(ftm_event_group, FTM_REPORT_BIT | FTM_FAILURE_BIT,
                                          pdFALSE, pdFALSE, xTicksToWaitFTM);
        
        if (bits & FTM_REPORT_BIT){  
            free(g_ftm_report);
            g_ftm_report = NULL;
            g_ftm_report_num_entries = 0;
            vTaskDelay(20);
        } else if (bits & FTM_FAILURE_BIT){  
            xEventGroupClearBits(ftm_event_group, FTM_FAILURE_BIT);
            emxDestroyArray_real_T(result);
            emxDestroyArray_real_T(X);
            free(g_ftm_report);
            g_ftm_report = NULL;
            g_ftm_report_num_entries = 0;
            //hard_restart();
            esp_restart();
        } else {
            //esp_restart();
            //wifi_perform_scan(NULL, false);
            vTaskDelay(20);

        }

        xEventGroupClearBits(ftm_event_group, FTM_REPORT_BIT);
        //xEventGroupClearBits(ftm_event_group, FTM_FAILURE_BIT);
        //vTaskDelay(20);
        esp_task_wdt_reset();
    }
}
