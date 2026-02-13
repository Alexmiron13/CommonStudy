idf_component_register(
    SRCS "uart.c" 
    INCLUDE_DIRS "include" "${CMAKE_SOURCE_DIR}/main/include"
    REQUIRES driver)
