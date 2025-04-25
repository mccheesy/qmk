#include QMK_KEYBOARD_H
#include "config.h"
#include "quantum.h"

const uint16_t PROGMEM dpi_combo[] = {KC_BTN4, KC_BTN5, COMBO_END};
const uint16_t PROGMEM layer0_combo[] = {KC_BTN1, KC_BTN2, COMBO_END};
const uint16_t PROGMEM layer1_combo[] = {KC_BTN2, KC_BTN3, COMBO_END};
const uint16_t PROGMEM bootloader_combo[] = {KC_BTN1, KC_BTN2, KC_BTN3, COMBO_END};

combo_t key_combos[] = {
    COMBO(dpi_combo, DPI_CONFIG),
    COMBO(layer0_combo, TO(0)),
    COMBO(layer1_combo, TO(1)),
    COMBO(bootloader_combo, QK_BOOT),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
//      |-------------|---------------|---------------|-------------|
//      |   KC_BTN1   |    KC_BTN3    |    KC_BTN2    |   KC_BTN5   |
//      |-------------|---------------|---------------|-------------|
//      | DRAG_SCROLL |               |               |   KC_BTN4   |
//      |-------------|---------------|---------------|-------------|
            KC_BTN1,       KC_BTN3,        KC_BTN2,       KC_BTN5,
          DRAG_SCROLL,                                    KC_BTN4 ),
    [1] = LAYOUT(
//      |-------------|---------------|---------------|-------------|
//      |   KC_BTN1   | LSFT(KC_BTN1) | LSFT(KC_BTN2) |   KC_BTN2   |
//      |-------------|---------------|---------------|-------------|
//      | DRAG_SCROLL |               |               |   KC_BTN5   |
//      |-------------|---------------|---------------|-------------|
            KC_BTN1,    LSFT(KC_BTN1),  LSFT(KC_BTN2),    KC_BTN2,
          DRAG_SCROLL,                                    KC_BTN5 )
};
