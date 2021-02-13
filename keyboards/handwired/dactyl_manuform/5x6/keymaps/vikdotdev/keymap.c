#include QMK_KEYBOARD_H

#define _MAIN 0
#define _SIDE 1
#define _NUM 2
#define _FN 3
#define LC_ESC      LCTL_T(KC_ESC)
#define WIN_TAB     LWIN_T(KC_TAB)
#define RC_BSPC     RCTL_T(KC_BSPC)
#define TS_ENT      LT(_SIDE, KC_ENT)
#define OSL_FN      OSL(_FN)
#define OSL_NUM     OSL(_NUM)
#define OSM_WIN     OSM(KC_LWIN)
#define OSM_LS      OSM(MOD_LSFT)
#define OSM_LC      OSM(MOD_RCTL)
#define OSM_LA      OSM(MOD_LALT)

static bool alt_set_by_special_tab = false;

enum custom_keycodes {
    A_TAB_F = SAFE_RANGE,
    A_TAB_B,
    C_TAB_F,
    C_TAB_B,
    SP_SPC, // custom LT(_SIDE, KC_SPC)
    C_LBRC,
    C_RBRC
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
    if (keycode == LC_ESC && record->event.pressed) {
        bool rc = true;
        uint8_t mods = 0;
        if ((mods = get_oneshot_mods()) && !has_oneshot_mods_timed_out()) {
            clear_oneshot_mods();
            unregister_mods(mods);
            rc = false;
        }
        if ((mods = get_oneshot_locked_mods())) {
            clear_oneshot_locked_mods();
            unregister_mods(mods);
            rc = false;
        }
        if (is_oneshot_layer_active()) {
            layer_clear();
            rc = false;
        }

        return rc;
    }

    switch (keycode) {
    case C_LBRC:
        if (record->event.pressed) {
            register_code(KC_LCTL);
            register_code(KC_LBRC);
            unregister_code(KC_LBRC);
            unregister_code(KC_LCTL);
        }
        return false;
        break;
    case C_RBRC:
        if (record->event.pressed) {
            register_code(KC_LCTL);
            register_code(KC_RBRC);
            unregister_code(KC_RBRC);
            unregister_code(KC_LCTL);
        }
        return false;
        break;
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
    if (keycode == SP_SPC && IS_LAYER_ON(_SIDE)) {
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
     KC_6   , KC_1  , KC_2  , KC_3  , KC_4  , KC_5,                 KC_7, KC_8, KC_9,    KC_0,   KC_GRV,  KC_DEL,
     WIN_TAB, KC_Q  , KC_W  , KC_E  , KC_R  , KC_T,                 KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_BSLS,
     OSM_LS , KC_A  , KC_S  , KC_D  , KC_F  , KC_G,                 KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, OSM_LS,
     KC_CAPS, KC_Z  , KC_X  , KC_C  , KC_V  , KC_B,                 KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, RESET,
                      KC_EQL, KC_MINS,                                          KC_LBRC, KC_RBRC,
                                      LC_ESC ,                            RC_BSPC,
                                      SP_SPC, KC_UNDS,         KC_QUOT,   TS_ENT,
                                      OSL_NUM,OSM_LA,          OSM_LA,    OSM_LC
  ),
  [_NUM] = LAYOUT_5x6(
     _______,_______,_______,_______,_______,_______,                  _______,KC_PAST,_______,_______,_______,_______,
     _______,KC_PEQL,KC_P7 , KC_P8 , KC_P9  ,KC_PDOT,                  KC_PEQL, KC_P7 , KC_P8 , KC_P9 ,KC_PDOT,_______,
     _______,KC_PPLS,KC_P4 , KC_P5 , KC_P6  ,KC_PAST,                  KC_PPLS, KC_P4 , KC_P5 , KC_P6 ,KC_PAST,_______,
     _______,KC_PMNS,KC_P1 , KC_P2 , KC_P3  ,KC_PSLS,                  KC_PMNS, KC_P1 , KC_P2 , KC_P3 ,KC_PSLS,_______,
                     _______,KC_P0 ,                                                    KC_P0 , KC_NLCK,
                                             _______,                  _______,
                                             _______,_______,  _______,_______,
                                             _______,_______,  _______,_______

  ),
  [_FN] = LAYOUT_5x6(
     _______,_______, KC_F10, KC_F11, KC_F12,_______,                  _______, KC_F10, KC_F11, KC_F12,_______,_______,
     _______,_______, KC_F7 , KC_F8 , KC_F9 ,_______,                  _______, KC_F7 , KC_F8 , KC_F9 ,_______,_______,
     _______,_______, KC_F4 , KC_F5 , KC_F6 ,_______,                  _______, KC_F4 , KC_F5 , KC_F6 ,_______,_______,
     _______,_______, KC_F1 , KC_F2 , KC_F3 ,_______,                  _______, KC_F1 , KC_F2 , KC_F3 ,_______,_______,
     _______,_______,                                                                  _______,_______,
                                             _______,                  _______,
                                             _______,_______,  _______,_______,
                                             _______,_______,  _______,_______

  ),
  [_SIDE] = LAYOUT_5x6(
     KC_CIRC,KC_EXLM,KC_AT  ,KC_HASH,KC_DLR ,KC_PERC,                      KC_AMPR,KC_ASTR,KC_LPRN,KC_RPRN,KC_TILD,_______,
     KC_MPLY,KC_MPRV,KC_MNXT,C_TAB_B,C_TAB_F,OSL_FN ,                      _______,KC_PGDN,KC_PGUP,_______,KC_PSCR,KC_PIPE,
     KC_MUTE,KC_VOLD,KC_VOLU,A_TAB_B,A_TAB_F,KC_ENT ,                      KC_LEFT,KC_DOWN,KC_UP  ,KC_RGHT,KC_COLN,KC_DQUO,
     _______,_______,_______,C_LBRC ,C_RBRC ,OSM_WIN,                      _______,_______,KC_LABK,KC_RABK,KC_QUES,_______,
                    KC_PLUS,KC_UNDS,                                                       KC_LCBR,KC_RCBR,
                                     _______,                                      _______,
                                             _______,_______,      KC_DQUO,_______,
                                             _______,_______,      _______,_______
  ),
};
