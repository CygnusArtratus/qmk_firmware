/* Copyright 2019 Josh Johnson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied w"sarranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include <stdio.h>

// Keyboard Layers
enum keyboard_layers{
    _BASE = 0,
    _MACRO,
    _LEDS
};

// Tap Dance Declarations
enum tap_dance {
  TD_LAYER_TOGGLE,
  TD_DOTEQUALS,

};

// LAYERTOGGLE FUNCTION

void dance_layertoggle_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        register_code16(TG(_MACRO));
    } else {
      register_code16(TG(_LEDS));
    }
}

void dance_layertoggle_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        unregister_code16(TG(_MACRO));
    } else {
        unregister_code16(TG(_MACRO));
    }
}

// `finished` and `reset` functions for each tapdance keycode
void dance_layertoggle_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_layertoggle_reset(qk_tap_dance_state_t *state, void *user_data);

// Define TAP DANCE ACTION for each tapdance keycode, passing in `finished` and `reset` functions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once to toggle macro layer, tap twice to toggle LED layer
    [TD_LAYER_TOGGLE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_layertoggle_finished, dance_layertoggle_reset),
    [TD_DOTEQUALS] = ACTION_TAP_DANCE_DOUBLE(KC_KP_EQUAL, KC_KP_DOT),
};

// Layers

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT( /* Base */
/*
* .-----------------------.
* |  vol  |       | zoom  |
* |       |       |       |
* |-----+-----+-----+-----|
* |_macro|  * |  /  |  -  |
* |_leds |    |     |  _  |
* |-----+-----+-----+-----|
* |  9  |  6  |  3  |  +  |
* |     |  ^  |  #  |shift|
* |-----+-----+-----+-----|
* |  8  |  5  |  2  |  =  |
* |     |  %  |  @  |  .  |
* |-----+-----+-----+-----|
* |  7  |  4  |  1  |  0  |
* |  &  |  $  |  !  |     |
* |-----+-----+-----+-----|
*/
        KC_AUDIO_MUTE,      LGUI(LALT(KC_8)),
    TD(TD_LAYER_TOGGLE), KC_KP_ASTERISK, KC_KP_SLASH, KC_MINUS,
    KC_KP_9,  KC_6, KC_3, LSFT_T(KC_KP_PLUS),
    KC_KP_8,  KC_5, KC_2, TD(TD_DOTEQUALS),
    KC_7,     KC_4, KC_1, KC_KP_0
  ),

  [_MACRO] = LAYOUT(


/*
* .-----------------------.
* |  vol  |       | zoom  |
* |       |       |       |
* |-----+-----+-----+-----|
* |_macro|     |    |_base|
* |_leds|     |     |     |
* |-----+-----+-----+-----|
* |macro|macro|     |right|
* |     |     |     |     |
* |-----+-----+-----+-----|
* |macro|macro|  up |down |
* |     |     |     |     |
* |-----+-----+-----+-----|
* |macro|macro|     |left |
* |     |     |     |     |
* |-----+-----+-----+-----|
*/

        KC_TRNS,     KC_TRNS,
    KC_TRNS, KC_NO, KC_NO, TG(_BASE),
    HYPR(KC_F16), HYPR(KC_F19), KC_NO, KC_RIGHT,
    HYPR(KC_F15), HYPR(KC_F18), KC_UP, KC_DOWN,
    HYPR(KC_F14), HYPR(KC_F17), KC_NO, KC_LEFT
  ),


  [_LEDS] = LAYOUT( /* LED Control */
          KC_NO,            KC_NO,
    KC_TRNS, RGB_MOD, RGB_RMOD, TG(_BASE),
    RGB_VAD, RGB_VAI, RGB_HUD,  RGB_HUI,
    RGB_SAD, RGB_SAI, KC_NO,  KC_NO,
    RGB_TOG, KC_NO, RESET,   KC_NO
  ),
};

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* Left Encoder */
        if (clockwise) {
            tap_code(KC_AUDIO_VOL_UP);
        } else {
            tap_code(KC_AUDIO_VOL_DOWN);
        }
    } else if (index == 1) { /* Right Encoder */
        if (clockwise) {
            tap_code16(LGUI(LALT(KC_EQUAL)));
        } else {
            tap_code16(LGUI(LALT(KC_MINUS)));
        }
    }
};
