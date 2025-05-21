set(BATCH_UNICAST_SIZE 8192 CACHE STRING "Override unicast batch size" FORCE)
set(BATCH_MULTICAST_SIZE 8192 CACHE STRING "Override multicast batch size" FORCE)
set(FRAG_MAX_SIZE 8192 CACHE STRING "Override fragmentation max size" FORCE)

# Incluir todos los archivos fuente necesarios (agregado net y core)
file(GLOB_RECURSE ZENOH_COMMON_SRC
    zenoh-pico/src/api/*.c
    zenoh-pico/src/collections/*.c
    zenoh-pico/src/collections/internal/*.c
    zenoh-pico/src/buffers/*.c
    zenoh-pico/src/link/*.c
    zenoh-pico/src/net/*.c
    zenoh-pico/src/protocol/*.c
    zenoh-pico/src/protocol/**/*.c
    zenoh-pico/src/session/*.c
    zenoh-pico/src/transport/*.c
    zenoh-pico/src/transport/common/*.c
    zenoh-pico/src/transport/unicast/*.c
    zenoh-pico/src/transport/multicast/*.c
    zenoh-pico/src/utils/*.c
    zenoh-pico/src/intmap/*.c
    zenoh-pico/src/core/*.c
    zenoh-pico/src/system/common/*.c
    )

file(GLOB ZENOH_ESPIDF_SRC
    zenoh-pico/src/system/espidf/*.c
)

set(ZENOH_SRC_FILES
    ${ZENOH_COMMON_SRC}
    ${ZENOH_ESPIDF_SRC}
)

idf_component_register(
    SRCS ${ZENOH_SRC_FILES}
    INCLUDE_DIRS
        zenoh-pico/include
        zenoh-pico/src
        zenoh-pico/src/system/espidf
    REQUIRES
        esp_driver_uart
        lwip
)

# Activar soporte completo de Zenoh-Pico
target_compile_definitions(${COMPONENT_LIB} PRIVATE
    ZENOH_ESPIDF
    ZENOH_PICO_USE_HIGH_LEVEL_API
    ZENOH_PICO_WITH_TASK=1
    ZENOH_PICO_WITH_THREAD=0
    ZENOH_PICO_WITH_TEST=0
    ZENOH_PICO_WITH_ESP_IDF=1
    ZENOH_PICO_WITH_SCOUTING=1
    BATCH_UNICAST_SIZE=8192
    BATCH_MULTICAST_SIZE=8192
    FRAG_MAX_SIZE=8192
)
