#include QMK_KEYBOARD_H


extern keymap_config_t keymap_config;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _BASE 0
#define _FN1 1
#define _FFN 2
#define _PLEB 3

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  C_TAB_F = 2,
  C_TAB_B = 3,
//   C_HIS_F = 4,
//   C_HIS_B = 5,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case C_TAB_F:
      if (record->event.pressed) {
        register_code(KC_LCTL);
        register_code(KC_TAB);
        unregister_code(KC_TAB);
        unregister_code(KC_LCTL);
      }
      break;
    case C_TAB_B:
      if (record->event.pressed) {
        register_code(KC_LCTL);
        register_code(KC_LSFT);
        register_code(KC_TAB);
        unregister_code(KC_LCTL);
        unregister_code(KC_LSFT);
        unregister_code(KC_TAB);
      }
      break;
//    case C_HIS_F:
//      if (record->event.pressed) {
//        register_code(KC_LALT);
//        register_code(KC_RGHT);
//        unregister_code(KC_RGHT);
//        unregister_code(KC_LALT);
//      }
//      break;
//    case C_HIS_B:
//      if (record->event.pressed) {
//        register_code(KC_LALT);
//        register_code(KC_LEFT);
//        unregister_code(KC_LEFT);
//        unregister_code(KC_LALT);
//      }
//      break;
  }

  return true;
};
// add proper history switcher
// add media keys

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_60_with_macro(
    RGB_VAD, RGB_VAI,  KC_GRV,           KC_1,   KC_2,     KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,               KC_EQL, KC_NO,  KC_NO, \
    RGB_HUD, RGB_HUI,  LT(_FFN, KC_TAB), KC_Q,   KC_W,     KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,               KC_RBRC, KC_BSLS, \
    RGB_SAD, RGB_SAI,  LCTL_T(KC_ESC),   KC_A,   KC_S,     KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,               KC_NO, \
    RGB_MOD, RGB_RMOD, KC_LSFT,          KC_Z,   KC_X,     KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,               KC_NO, \
    TG(_PLEB),RGB_TOG, KC_NO,            KC_CAPS,LWIN_T(KC_DEL),LALT_T(KC_BSPC),LT(_FN1,KC_SPC), LT(_FN1, KC_ENT), KC_BSPC, KC_RALT, KC_NO, KC_NO, KC_NO, KC_NO
  ),
  [_FN1] = LAYOUT_60_with_macro(
    _______, _______,  KC_MUTE,          KC_VOLD, KC_VOLU, KC_MPRV, KC_MPLY, KC_MNXT, _______, _______, _______, KC_LPRN, KC_RPRN, _______,               _______, _______, _______, \
    _______, _______,  KC_LPRN,          KC_LBRC, KC_RBRC, C_TAB_B, C_TAB_F, KC_TILD, KC_NO,   KC_PGDN, KC_PGUP, _______, _______, _______,               _______, _______, \
    _______, _______,  KC_LCBR,          KC_LCBR, KC_RCBR, KC_BSPC, KC_DEL,  KC_ENT,  KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,               _______, \
    _______, _______,  _______,          KC_EQL,  KC_UNDS, KC_BTN4, KC_BTN5, KC_GRV,  KC_NO,   KC_HOME, KC_END,  KC_INS,  _______, _______,               _______, \
    _______, _______,  _______,          _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______,               _______
  ),
  [_FFN] = LAYOUT_60_with_macro(
    _______, _______,  _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,               _______, _______, _______, \
    _______, _______,  _______,          KC_F12,  KC_F11,  KC_F10,  KC_F9,   _______, _______, KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______,               _______, _______, \
    _______, _______,  TG(_FFN),         KC_F8,   KC_F7,   KC_F6,   KC_F5,   _______, _______, KC_F5,   KC_F6,   KC_F7,   KC_F8,   _______,               _______, \
    _______, _______,  _______,          KC_F4,   KC_F3,   KC_F2,   KC_F1,   _______, _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   _______,               _______, \
    _______, _______,  _______,          _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______,               _______
  ),
  [_PLEB] = LAYOUT_60_with_macro(
    _______, _______,  KC_GESC,          KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,      KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL, \
    _______, _______,  KC_TAB,           KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,      KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, \
    _______, _______,  KC_CAPS,          KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,      KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT, \
    _______, _______,  KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,      KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_NO, \
    TG(_PLEB),_______, KC_LCTL,          KC_LGUI, MO(_FN1),KC_LALT, KC_SPC,           KC_SPC,  KC_BSPC,   KC_RALT, KC_RGUI, KC_NO,   KC_CAPS, KC_APP
  )
};
