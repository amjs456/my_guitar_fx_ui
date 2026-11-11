idf_component_register(SRCS "my_guitar_fx_ui.c" "src/display_manager.c"
                    INCLUDE_DIRS "include"
                    PRIV_INCLUDE_DIRS "private_include"
                    PRIV_REQUIRES my_ili9341_lvgl)
