#include "display_manager.h"

#include "core/lv_group.h"
#include "core/lv_obj.h"
#include "core/lv_obj_event.h"
#include "core/lv_obj_pos.h"
#include "core/lv_obj_tree.h"
#include "display/lv_display.h"
#include "font/lv_font.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "layouts/flex/lv_flex.h"
#include "lv_conf_internal.h"
#include "misc/lv_event.h"
#include "misc/lv_types.h"
#include "portmacro.h"
#include "sdkconfig.h"
#include "widgets/button/lv_button.h"
#include "widgets/label/lv_label.h"
#include <stdint.h>
#include <string.h>

#include "tuner.h"

#define BUTTON_WIDTH 80
#define BUTTON_HEIGHT 50


lv_obj_t *tuner_container;
size_t g_fx_array_length;
lv_obj_t *g_fx_ui_container[CONFIG_MAX_FX_COUNT];
lv_obj_t *fx_btn[CONFIG_MAX_FX_COUNT];

static void fx_button_event_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED){
        lv_obj_t *clicked_btn = lv_event_get_target_obj(e);
        for(int i = 0; i < g_fx_array_length; i++){
            if(strcmp(lv_obj_get_name(g_fx_ui_container[i]) ,lv_obj_get_name(clicked_btn)) == 0){
                lv_obj_remove_flag(g_fx_ui_container[i], LV_OBJ_FLAG_HIDDEN);
                continue;
            }
        }
    }
}


static void back_button_event_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target_obj(e);
    lv_obj_t *parent = lv_obj_get_parent(btn);
    if(code == LV_EVENT_CLICKED){
        lv_obj_add_flag(parent, LV_OBJ_FLAG_HIDDEN);
    }
}

void display_manager(lv_display_t *disp, const fx_t fx_array[], size_t fx_array_length){
    g_fx_array_length = fx_array_length; 
    lv_obj_t *fx_btn_lbl[fx_array_length];

    lv_obj_t *scr = lv_display_get_screen_active(disp);
    lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_COLUMN);
    int32_t scr_width = (int32_t)lv_obj_get_width(scr);

    lv_obj_t *navbar_container = lv_obj_create(scr);
    lv_obj_set_style_pad_all(navbar_container, 0, 0);
    lv_obj_set_size(navbar_container, scr_width, 20);

    lv_obj_t *title_lbl = lv_label_create(navbar_container);
    lv_label_set_text(title_lbl, "MY GUITAR FX");


    lv_obj_t *main_container = lv_obj_create(scr);
    lv_obj_set_size(main_container, scr_width, 230);
    lv_obj_set_style_pad_all(main_container, 0, 0);
    

    lv_obj_t *btn_container = lv_obj_create(main_container);
    lv_obj_set_flex_flow(btn_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_all(btn_container, 0, 0);
    int32_t btn_container_width = scr_width * 0.9f;
    lv_obj_set_size(btn_container, btn_container_width, 100);
    lv_obj_set_pos(btn_container, (scr_width - btn_container_width) / 2, 40);

    for(int i = 0; i < fx_array_length; i++){
        g_fx_ui_container[i] = fx_array[i].fx_ui_func_ptr(main_container, fx_array[i].args);
        lv_obj_add_flag(g_fx_ui_container[i], LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_pad_all(g_fx_ui_container[i], 0, 0);
        lv_obj_set_size(g_fx_ui_container[i], 320, 200);
        fx_btn[i] = lv_obj_create(btn_container);
        fx_btn_lbl[i] = lv_label_create(fx_btn[i]);
        lv_label_set_text(fx_btn_lbl[i], lv_obj_get_name(g_fx_ui_container[i]));
        lv_obj_set_name(fx_btn[i], lv_obj_get_name(g_fx_ui_container[i]));
        lv_obj_set_size(fx_btn[i], BUTTON_WIDTH, BUTTON_HEIGHT);
        lv_obj_add_event_cb(fx_btn[i], fx_button_event_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_t *back_btn  = lv_button_create(g_fx_ui_container[i]);
        lv_obj_t *back_lbl = lv_label_create(back_btn);
        lv_obj_center(back_lbl);
        lv_label_set_text(back_lbl, "Back");
        lv_obj_add_event_cb(back_btn, back_button_event_cb, LV_EVENT_CLICKED, NULL);
    }
}