idf_component_register(
    SRCS "at24c.c" 
    INCLUDE_DIRS "include" "${CMAKE_SOURCE_DIR}/main/include"
    REQUIRES driver)
