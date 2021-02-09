#include QMK_KEYBOARD_H


#define _MAIN 0
#define _SIDE 1
#define _NUM 2
#define _FN 3
#define LC_ESC      LCTL_T(KC_ESC)
#define LS_LBRC     LSFT_T(KC_LBRC)
#define LS_TAB      LSFT_T(KC_TAB)
#define LS_BSLS     LSFT_T(KC_BSLASH)
#define LS_A        LSFT_T(KC_A)
#define LA_ENT      LALT_T(KC_ENT)
#define RS_RBRC     RSFT_T(KC_RBRC)
#define RS_SCLN     RSFT_T(KC_SCLN)
#define RC_BSPC     RCTL_T(KC_BSPC)
#define RA_BSPC     LALT_T(KC_BSPC)
#define TS_SPC      LT(_SIDE, KC_SPC)
#define TS_ENT      LT(_SIDE, KC_ENT)
#define TF_TAB      LT(_FN, KC_TAB)
#define TF_EQL      LT(_FN, KC_EQL)
#define LC_QUOT     LCTL_T(KC_QUOT)

static bool alt_set_by_special_tab = false;

enum custom_keycodes {
    A_TAB_F = SAFE_RANGE,
    A_TAB_B,
    C_TAB_F,
    C_TAB_B,
    SP_SPC  // custom LT(_SIDE, KC_SPC)
};

// called everytime _SIDE is switched off.
static void unregister_special_alt_tab(void) {
    bool alt_on = get_mods() & MOD_BIT(KC_LALT);

    if (alt_set_by_special_tab && IS_LAYER_OFF(_SIDE) && alt_on) {
        unregister_mods(MOD_LALT);
        alt_set_by_special_tab = false;
    }
}

// State of the SP_SPC key.
typedef struct {
  bool sp_key_pressed;
  uint16_t timer;         // Timer since the SP_SPC key has been pressed.
  uint16_t key_presses;   // Number of other key presses while SP_SPC is active.
  uint16_t key_releases;  // Number of other key releases while SP_SPC is active.
} LTInfo;

static LTInfo lt_info;

// TODO: maybe rewrite this as generic function by passing layer, keycode and state
static void handle_lt(bool pressed) {
  if (pressed) {
    if (!lt_info.sp_key_pressed) {
      layer_on(_SIDE);
      lt_info.sp_key_pressed = true;
      lt_info.timer = timer_read();
      lt_info.key_presses = 0;
      lt_info.key_releases = 0;
    }
  } else {
    lt_info.sp_key_pressed = false;

    if (lt_info.key_releases >= lt_info.key_presses) {
      // No "interrupt" keys pending release.
      layer_off(_SIDE);
    }
    // Only click the key if:
    // - No other keys are pressed between the press and release events
    // - The release happens before the TAPPING_TERM
    if (lt_info.key_presses == 0 && timer_elapsed(lt_info.timer) < TAPPING_TERM) {
      register_code(KC_SPC);
      unregister_code(KC_SPC);
    }
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case SP_SPC:
        handle_lt(record->event.pressed);
        unregister_special_alt_tab();

        return false;
        break;
    case A_TAB_F:
        if (record->event.pressed) {
            bool alt_on = get_mods() & MOD_BIT(KC_LALT);

            if (!alt_on) {
                register_mods(MOD_LALT);
                alt_set_by_special_tab = true;
            }

            register_code(KC_TAB);
            unregister_code(KC_TAB);
        }
        return false;
        break;
    case A_TAB_B:
        if (record->event.pressed) {
            bool alt_on = get_mods() & MOD_BIT(KC_LALT);

            if (!alt_on) {
                register_mods(MOD_LALT);
                alt_set_by_special_tab = true;
            }

            register_code(KC_LSFT);
            register_code(KC_TAB);
            unregister_code(KC_LSFT);
            unregister_code(KC_TAB);
        }
        return false;
        break;
    case C_TAB_F:
      if (record->event.pressed) {
        register_code(KC_LCTL);
        register_code(KC_TAB);
        unregister_code(KC_TAB);
        unregister_code(KC_LCTL);
      }
      return false;
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
      return false;
      break;
    }

    // Manage "interrupt" keys after SP_SPC key is released.
    if (IS_LAYER_ON(_SIDE)) {
        if (record->event.pressed) {
            lt_info.key_presses++;
        } else {
            lt_info.key_releases++;

            if (!lt_info.sp_key_pressed && lt_info.key_releases >= lt_info.key_presses) {
                register_code16(keycode);
                unregister_code16(keycode);
                layer_off(_SIDE);
                unregister_special_alt_tab();

                return false;
            }
        }
    }

    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_MAIN] = LAYOUT_5x6(
     KC_1,    KC_2, KC_3, KC_4, KC_5, KC_6,                           KC_7, KC_8, KC_9,    KC_0,   KC_MINS, KC_PLUS,
     KC_TAB,  KC_Q, KC_W, KC_E, KC_R, KC_T,                           KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_BSLS,
     LC_ESC , KC_A, KC_S, KC_D, KC_F, KC_G,                           KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, LC_QUOT,
     KC_CAPS, KC_Z, KC_X, KC_C, KC_V, KC_B,                           KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, RESET,
                    KC_EQL,KC_UNDS,                                               RESET, RESET,
                                      LS_LBRC,                        RS_RBRC,
                                      SP_SPC , KC_BSPC,      RC_BSPC, TS_ENT,
                                      LA_ENT , RESET,        KC_RWIN, RESET
  ),
  [_NUM] = LAYOUT_5x6(
     _______,_______,_______,_______,_______,_______,                  _______,_______,_______,_______,_______,_______,
     _______,_______,_______,_______,_______,_______,                  KC_RBRC, KC_P7 , KC_P8 , KC_P9 ,_______,KC_PLUS,
     _______,_______,_______,_______,_______,_______,                  KC_RPRN, KC_P4 , KC_P5 , KC_P6 ,KC_MINS,KC_PIPE,
     _______,_______,_______,_______,_______,_______,                  _______, KC_P1 , KC_P2 , KC_P3 ,KC_EQL ,KC_UNDS,
     _______,_______,                                                  _______, _______,
                                             _______,                      _______,
                                             _______,_______,      _______,_______,
                                             _______,_______,      _______,_______

  ),
  [_SIDE] = LAYOUT_5x6(
     KC_MUTE,KC_VOLD,KC_VOLU,_______,_______,_______,                      _______,_______,_______,_______,_______,_______,
     _______,_______,_______,A_TAB_B,A_TAB_F,KC_TILD,                      _______,_______,_______,_______,KC_PSCR,_______,
     _______,_______,_______,KC_BSPC,KC_DEL ,_______,                      KC_LEFT,KC_DOWN,KC_UP  ,KC_RGHT,_______,_______,
     _______,_______,_______,C_TAB_B,C_TAB_F,KC_GRV ,                      _______,_______,_______,_______,_______,_______,
                    _______,_______,                                                       _______,_______,
                                             _______,                      _______,
                                             _______,_______,      _______,_______,
                                             _______,_______,      _______,_______
  ),
};
