idf_component_register(
    SRCS "wifi_sta.c" 
    INCLUDE_DIRS "include" "${CMAKE_SOURCE_DIR}/main/include"
    REQUIRES time_sync esp_wifi)
