#include QMK_KEYBOARD_H
#include "debug.h"

// use outbound scroll lock to activate mouse mode
#define MOUSE_LOCK_TIMEOUT 1200
#define SCROLL_MODE_TIMEOUT 1200

#define DELTA_X_THRESHOLD 120
#define DELTA_Y_THRESHOLD 30

enum custom_keycodes {
    MOUSE_LOCK_ON = SAFE_RANGE,
    MOUSE_LOCK_OFF
};

// Dummy
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {{{ KC_NO }}};

static uint8_t  caps_lock_state;
static uint8_t  num_lock_state;
static uint8_t  scroll_lock_state;
static bool     mouse_active = false;
static bool     scroll_mode = false;
static bool     scroll_engaged = false;
//static int8_t   delta_x;
//static int8_t   delta_y;
static uint16_t last_mouse_activity = 0;

keyboard_config_t keyboard_config;

void keyboard_post_init_user(void) {
    debug_enable      = true;
    debug_mouse       = true;
    caps_lock_state   = host_keyboard_led_state().caps_lock;
    num_lock_state    = host_keyboard_led_state().num_lock;
    scroll_lock_state = host_keyboard_led_state().scroll_lock;
}

bool led_update_user(led_t led_state) {
    if (led_state.caps_lock != caps_lock_state) {
        print("CAPS LOCK signal received: cycle DPI\n");
        cycle_dpi();
    }

    if (led_state.num_lock != num_lock_state) {
        print("NUM LOCK signal received: reset to bootloader\n");
        reset_keyboard();
    }

    if (led_state.scroll_lock != scroll_lock_state) {
        scroll_mode = led_state.scroll_lock;
        scroll_engaged = false;
        toggle_drag_scroll();
        if (scroll_mode) {
            print("SCROLL LOCK ON signal received: activate scroll mode\n");
        } else {
            print("SCROLL LOCK OFF signal received: deactivate scroll mode\n");
        }
    }

    return true;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    bool moved = (mouse_report.x || mouse_report.y || mouse_report.h || mouse_report.v);

    // Movement detection logic
    if (moved) {
        if (!mouse_active) {
            print("sending mouse lock - activate\n");
            tap_code16(MOUSE_LOCK_ON);
            mouse_active = true;
        }

        last_mouse_activity = timer_read();

        if (scroll_mode) {
            toggle_drag_scroll();
            scroll_engaged = true;
            /**
            delta_x += mouse_report.x;
            delta_y += mouse_report.y;

            if (delta_x > DELTA_X_THRESHOLD) {
                mouse_report.h = 1;
                delta_x        = 0;
            } else if (delta_x < -DELTA_X_THRESHOLD) {
                mouse_report.h = -1;
                delta_x        = 0;
            }

            if (delta_y > DELTA_Y_THRESHOLD) {
                mouse_report.v = -1;
                delta_y        = 0;
            } else if (delta_y < -DELTA_Y_THRESHOLD) {
                mouse_report.v = 1;
                delta_y        = 0;
            }

            mouse_report.x = 0;
            mouse_report.y = 0;
            */
        }
    } else {
        if (mouse_active && timer_elapsed(last_mouse_activity) > MOUSE_LOCK_TIMEOUT) {
            print("sending mouse lock - deactivate\n");
            tap_code16(MOUSE_LOCK_OFF);
            mouse_active = false;
        }

        if (scroll_mode && scroll_engaged && timer_elapsed(last_mouse_activity) > SCROLL_MODE_TIMEOUT) {
            print("deactivating scroll mode - timeout\n");
            toggle_drag_scroll();
            scroll_mode = false;
            scroll_engaged = false;
        }

    }

    return mouse_report;
}
