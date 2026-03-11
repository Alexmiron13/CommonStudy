idf_component_register(
    SRCS "SPI.c" 
    INCLUDE_DIRS "include" "${CMAKE_SOURCE_DIR}/main/include"
    REQUIRES driver)
