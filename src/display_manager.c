#include "display_manager.h"

#include "display/lv_display.h"
#include "misc/lv_types.h"
#include "widgets/button/lv_button.h"
#include "widgets/label/lv_label.h"

void display_manager(lv_display_t *disp){
    lv_obj_t *scr = lv_display_get_screen_active(disp);
    
    lv_obj_t *lbl = lv_label_create(scr);
    lv_label_set_text(lbl, "MY GUITAR FX");
}