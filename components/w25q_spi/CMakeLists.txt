idf_component_register(
    SRCS "w25q_spi.c" 
    INCLUDE_DIRS "include" "${CMAKE_SOURCE_DIR}/main/include"
    REQUIRES driver esp_driver_spi esp_driver_gpio SPI)
