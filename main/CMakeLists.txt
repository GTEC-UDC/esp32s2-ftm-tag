idf_component_register(
  SRCS 
    "main.c"
    "CompactRegressionTree.c"
    "predict_data.c"
    "predict_emxutil.c"
    "predict_terminate.c"
    "rtGetNaN.c"
    "initialize.c"
    "predict.c"
    "predict_emxAPI.c"
    "predict_initialize.c"
    "rtGetInf.c"
    "rt_nonfinite.c"
  INCLUDE_DIRS "."
  REQUIRES mqtt esp_wifi nvs_flash zenoh_pico_wrapper
)

target_compile_definitions(${COMPONENT_LIB} PRIVATE
    ZENOH_ESPIDF
    ZENOH_PICO_USE_HIGH_LEVEL_API
)
