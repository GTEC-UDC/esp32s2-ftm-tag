# Install script for directory: /home/valba/esp/esp-idf

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/home/valba/.espressif/tools/xtensa-esp-elf/esp-14.2.0_20241119/xtensa-esp-elf/bin/xtensa-esp32s2-elf-objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/xtensa/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_pm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/mbedtls/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/bootloader/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esptool_py/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/partition_table/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_app_format/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_bootloader_format/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/app_update/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_partition/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/efuse/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/bootloader_support/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_mm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/spi_flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_system/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_common/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_rom/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/hal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/log/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/heap/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/soc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_security/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_hw_support/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/freertos/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/newlib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/pthread/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/cxx/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_timer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_gptimer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_ringbuf/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_uart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/app_trace/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_event/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/nvs_flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_phy/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_usb_serial_jtag/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_vfs_console/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/vfs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/lwip/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_netif_stack/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_netif/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/wpa_supplicant/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_coex/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_wifi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_spi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_gdbstub/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/bt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/unity/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/cmock/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/console/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_pcnt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_mcpwm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_ana_cmpr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_i2s/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/sdmmc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_sdmmc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_sdspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_sdio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_dac/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_bitscrambler/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_rmt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_tsens/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_sdm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_ledc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_parlio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/driver/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/http_parser/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp-tls/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_adc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_isp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_cam/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_psram/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_jpeg/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_ppa/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_touch_sens/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_driver_twai/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_eth/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_hid/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/tcp_transport/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_http_client/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_http_server/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_https_ota/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_https_server/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_lcd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/protobuf-c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/protocomm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/esp_local_ctrl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/espcoredump/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/wear_levelling/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/fatfs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/idf_test/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/ieee802154/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/json/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/mqtt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/nvs_sec_provider/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/openthread/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/perfmon/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/rt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/spiffs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/touch_element/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/ulp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/usb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/wifi_provisioning/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/zenoh_pico_wrapper/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/main/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/cmd_nvs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/cmd_system/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/valba/Trabajo/TrabajoGIT/WIFIFTM/esp32s2-ftm-tag/build/esp-idf/cmd_wifi/cmake_install.cmake")
endif()

