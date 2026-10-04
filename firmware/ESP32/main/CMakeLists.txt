idf_component_register(SRCS "protocol.c" "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp_adc driver esp_timer
                )