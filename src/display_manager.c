#include "display_manager.h"

#include "core/lv_group.h"
#include "core/lv_obj.h"
#include "core/lv_obj_event.h"
#include "core/lv_obj_pos.h"
#include "core/lv_obj_tree.h"
#include "display/lv_display.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "misc/lv_event.h"
#include "misc/lv_types.h"
#include "widgets/button/lv_button.h"
#include "widgets/label/lv_label.h"
#include <stdint.h>

#include "tuner.h"

#define BUTTON_WIDTH 80
#define BUTTON_HEIGHT 50

QueueHandle_t tuner_result_queue_handle;
lv_obj_t *tuner_container;
lv_obj_t *target_note_lbl;
lv_obj_t *diff_cent_lbl;


static void tuner_button_event_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED){
        lv_obj_remove_flag(tuner_container, LV_OBJ_FLAG_HIDDEN);
    }
}

static void tuner_update_cb(lv_timer_t *timer){
    (void)timer;
    TuningResult tuning_result;

    if(xQueueReceive(tuner_result_queue_handle, &tuning_result, 0) == pdTRUE){
        char note[3];
        sprintf(note, "%d", tuning_result.target_midi_note);
        lv_label_set_text(target_note_lbl, note);

        //char diff_cent[32];
        //sprintf(diff_cent, "%.2f", tuning_result.diff_cent); 
        //lv_label_set_text(diff_cent_lbl, diff_cent);
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
    lv_obj_t *scr = lv_display_get_screen_active(disp);
    int32_t scr_width = (int32_t)lv_obj_get_width(scr);

    lv_obj_t *navbar_container = lv_obj_create(scr);
    lv_obj_set_style_pad_all(navbar_container, 0, 0);
    lv_obj_set_size(navbar_container, scr_width, 30);

    lv_obj_t *title_lbl = lv_label_create(navbar_container);
    lv_label_set_text(title_lbl, "MY GUITAR FX");


    lv_obj_t *main_container = lv_obj_create(scr);
    lv_obj_set_size(main_container, scr_width, 230);
    lv_obj_set_style_pad_all(main_container, 0, 0);
    

    lv_obj_t *btn_container = lv_obj_create(main_container);
    int32_t btn_container_width = scr_width * 0.9f;
    lv_obj_set_size(btn_container, btn_container_width, 100);
    lv_obj_set_pos(btn_container, (scr_width - btn_container_width) / 2, 40);
    

    lv_obj_t *tuner_btn = lv_button_create(btn_container);
    lv_obj_t *tuner_lbl = lv_label_create(tuner_btn);
    lv_obj_center(tuner_lbl);
    lv_label_set_text(tuner_lbl, "Tuner");
    lv_obj_set_size(tuner_btn, BUTTON_WIDTH, BUTTON_HEIGHT);
    lv_obj_add_event_cb(tuner_btn, tuner_button_event_cb, LV_EVENT_CLICKED, NULL);

    tuner_container = lv_obj_create(main_container);
    lv_obj_add_flag(tuner_container, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_pad_all(tuner_container, 0, 0);
    lv_obj_set_size(tuner_container, scr_width, 200);
    lv_obj_t *back_btn  = lv_button_create(tuner_container);
    lv_obj_t *back_lbl = lv_label_create(back_btn);
    lv_obj_center(back_lbl);
    lv_label_set_text(back_lbl, "Back");
    lv_obj_add_event_cb(back_btn, back_button_event_cb, LV_EVENT_CLICKED, NULL);


    
    tuner_arg_t *args = (tuner_arg_t *)fx_array[0].args;
    tuner_result_queue_handle = args->queue_handle;
    //lv_obj_t *tuner_mode_lbl = lv_label_create(tuner_container);
    //lv_label_set_text(tuner_mode_lbl, args->tuning);
    
    target_note_lbl = lv_label_create(tuner_container);
    lv_label_set_text(target_note_lbl, "---");
    //diff_cent_lbl = lv_label_create(tuner_container);
    //lv_label_set_text(diff_cent_lbl, "---");

    lv_timer_create(tuner_update_cb, 50, NULL);
}