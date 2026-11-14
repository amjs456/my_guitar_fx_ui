#include "my_guitar_fx_ui.h"

#include "display_manager.h"

#include "esp_err.h"
#include "esp_log.h"
#include "misc/lv_types.h"
#include "my_ili9341_lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "portmacro.h"
#include "sys/lock.h"

const char *TAG = "my_guitar_fx_display";

lv_display_t *display = NULL;

esp_err_t my_guitar_fx_display_init(const fx_t fx_array[], size_t fx_array_length){

    esp_err_t err = my_ili9341_lvgl_init(&display);
    if(err != ESP_OK){
        ESP_LOGE(TAG, "my_ili9341_lvgl_init failed", esp_err_to_name(err));
        return err;
    }

    BaseType_t res = xTaskCreate(lvgl_port_task, "LVGL", 8 * 1024, NULL, 2, NULL);
    if(res != pdTRUE){
        ESP_LOGE(TAG, "lvgl_port_task creation failed");
    }

    _lock_acquire(&lvgl_api_lock);
    display_manager(display, fx_array, fx_array_length);
    _lock_release(&lvgl_api_lock);
    return ESP_OK;
}