#include QMK_KEYBOARD_H
#include "config.h"
#include "quantum.h"

// Define a custom keycode for cycling layers
enum custom_keycodes {
    LAYER_CYCLE = SAFE_RANGE,
};

const uint16_t PROGMEM dpi_combo[] = {KC_BTN4, KC_BTN5, COMBO_END};
const uint16_t PROGMEM layer_combo[] = {KC_BTN1, KC_BTN2, COMBO_END};
const uint16_t PROGMEM bootloader_combo[] = {KC_BTN1, KC_BTN2, KC_BTN3, COMBO_END};

combo_t key_combos[] = {
    COMBO(dpi_combo, DPI_CONFIG),
    COMBO(layer_combo, LAYER_CYCLE), // Use the custom keycode
    COMBO(bootloader_combo, QK_BOOT),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( // Base
//      |-------------|---------------|---------------|-------------|
//      |   KC_BTN4   |    KC_BTN5    |    KC_BTN3    |   KC_BTN2   |
//      |-------------|---------------|---------------|-------------|
//      |   KC_BTN1   |               |               | DRAG_SCROLL |
//      |-------------|---------------|---------------|-------------|
            KC_BTN4,       KC_BTN5,        KC_BTN3,       KC_BTN2,
            KC_BTN1,                                    DRAG_SCROLL ),
    [1] = LAYOUT( // Alt
//      |-------------|---------------|---------------|-------------|
//      |   KC_BTN2   |  DRAG_SCROLL  |    KC_BTN3    |   KC_BTN5   |
//      |-------------|---------------|---------------|-------------|
//      |   KC_BTN1   |               |               |   KC_BTN4   |
//      |-------------|---------------|---------------|-------------|
            KC_BTN2,     DRAG_SCROLL,      KC_BTN3,       KC_BTN5,
            KC_BTN1,                                      KC_BTN4 ),
    [2] = LAYOUT( // Game
//      |-------------|---------------|---------------|-------------|
//      |   KC_LSFT   |    KC_LCTL    |  DRAG_SCROLL  |   KC_BTN2   |
//      |-------------|---------------|---------------|-------------|
//      |   KC_BTN1   |               |               |   KC_LALT   |
//      |-------------|---------------|---------------|-------------|
            KC_LSFT,       KC_LCTL,      DRAG_SCROLL,     KC_BTN2,
            KC_BTN1,                                      KC_LALT),
};

// Implement the layer-cycling logic
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case LAYER_CYCLE:
                // Cycle through layers 0, 1, and 2
                switch (get_highest_layer(layer_state)) {
                    case 0:
                        layer_move(1);
                        break;
                    case 1:
                        layer_move(2);
                        break;
                    case 2:
                        layer_move(0);
                        break;
                }
                return false; // Skip further processing of this key
        }
    }
    return true; // Process all other keycodes normally
}
