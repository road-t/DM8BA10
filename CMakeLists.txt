idf_component_register(SRCS "src/DM8BA10.cpp"
                    INCLUDE_DIRS "src"
                    REQUIRES driver freertos esp_common esp_timer)

target_compile_definitions(${COMPONENT_LIB} PUBLIC ESP_IDF=true)
