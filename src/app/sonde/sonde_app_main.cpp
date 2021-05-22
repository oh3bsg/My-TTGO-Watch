/****************************************************************************
 *   Aug 3 12:17:11 2020
 *   Copyright  2020  Dirk Brosswick
 *   Email: dirk.brosswick@googlemail.com
 ****************************************************************************/
 
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include "config.h"
#include <TTGO.h>

#include "sonde_app.h"
#include "sonde_app_main.h"

#include "gui/mainbar/app_tile/app_tile.h"
#include "gui/mainbar/main_tile/main_tile.h"
#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"
#include "gui/widget_factory.h"
#include "gui/widget_styles.h"

lv_obj_t *sonde_app_main_tile = NULL;
lv_obj_t * label_lat = NULL;
lv_obj_t * label_lon = NULL;
lv_obj_t * label_alt = NULL;
lv_obj_t * label_rssi = NULL;
lv_obj_t * arc_direction = NULL;

lv_style_t sonde_app_main_style;

lv_task_t * _sonde_app_task;

uint16_t distance = 0;
uint16_t dir = 260;

LV_IMG_DECLARE(refresh_32px);
LV_FONT_DECLARE(Ubuntu_72px);

static void exit_sonde_app_main_event_cb( lv_obj_t * obj, lv_event_t event );
static void enter_sonde_app_setup_event_cb( lv_obj_t * obj, lv_event_t event );
void sonde_app_task( lv_task_t * task );

void sonde_app_main_setup( uint32_t tile_num ) {

    sonde_app_main_tile = mainbar_get_tile_obj( tile_num );
    lv_style_copy( &sonde_app_main_style, ws_get_mainbar_style() );

    label_lat = lv_label_create(sonde_app_main_tile, NULL);
    lv_label_set_text(label_lat, "Lat : 61.000000");
    lv_obj_align(label_lat, NULL, LV_ALIGN_IN_TOP_MID, SONDE_DISTANCE_LABEL_X_OFFSET, SONDE_DISTANCE_LABEL_Y_OFFSET);

    label_lon = lv_label_create(sonde_app_main_tile, NULL);
    lv_label_set_text(label_lon, "Lon : 23.000000");
    lv_obj_align(label_lon, NULL, LV_ALIGN_IN_TOP_MID, SONDE_DIRECTION_LABEL_X_OFFSET, SONDE_DIRECTION_LABEL_Y_OFFSET);

    label_alt = lv_label_create(sonde_app_main_tile, NULL);
    lv_label_set_text(label_alt, "Alt : 32000 m");
    lv_obj_align(label_alt, NULL, LV_ALIGN_IN_TOP_MID, SONDE_ALT_LABEL_X_OFFSET, SONDE_ALT_LABEL_Y_OFFSET);

    label_rssi = lv_label_create(sonde_app_main_tile, NULL);
    lv_label_set_text(label_rssi, "RSSI: -100 dBm");
    lv_obj_align(label_rssi, NULL, LV_ALIGN_IN_TOP_MID, SONDE_RSSI_LABEL_X_OFFSET, SONDE_RSSI_LABEL_Y_OFFSET);

    arc_direction = lv_arc_create(sonde_app_main_tile, NULL);
    lv_arc_set_angles(arc_direction, 270, 275);
    lv_obj_align(arc_direction, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -10);

    lv_obj_t * exit_btn = wf_add_exit_button( sonde_app_main_tile, exit_sonde_app_main_event_cb, &sonde_app_main_style );
    lv_obj_align(exit_btn, sonde_app_main_tile, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10 );

    lv_obj_t * setup_btn = wf_add_setup_button( sonde_app_main_tile, enter_sonde_app_setup_event_cb, &sonde_app_main_style );
    lv_obj_align(setup_btn, sonde_app_main_tile, LV_ALIGN_IN_BOTTOM_RIGHT, -10, -10 );

    // uncomment the following block of code to remove the "myapp" label in background
    /*
    lv_style_set_text_opa( &sonde_app_main_style, LV_OBJ_PART_MAIN, LV_OPA_70);
    lv_style_set_text_font( &sonde_app_main_style, LV_STATE_DEFAULT, &Ubuntu_72px);
    lv_obj_t *app_label = lv_label_create( sonde_app_main_tile, NULL);
    lv_label_set_text( app_label, sonde_APP_LABEL);
    lv_obj_reset_style_list( app_label, LV_OBJ_PART_MAIN );
    lv_obj_add_style( app_label, LV_OBJ_PART_MAIN, &sonde_app_main_style );
    lv_obj_align( app_label, sonde_app_main_tile, LV_ALIGN_CENTER, 0, 0);
    */

    // create an task that runs every second
    _sonde_app_task = lv_task_create( sonde_app_task, 5*1000, LV_TASK_PRIO_MID, NULL );
}

static void enter_sonde_app_setup_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):
            statusbar_hide( true );
            mainbar_jump_to_tilenumber( sonde_app_get_app_setup_tile_num(), LV_ANIM_ON );
            break;
    }
}

static void exit_sonde_app_main_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):
            mainbar_jump_to_maintile( LV_ANIM_OFF );
            break;
    }
}

void sonde_app_task( lv_task_t * task ) {
    // put your code her
    lv_arc_set_angles(arc_direction, dir-2, dir+2);
 }