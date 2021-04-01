#include QMK_KEYBOARD_H
#include "musical_notes.h"

#define _MAIN 0
#define _NUM 1
#define _FN 2
#define _MOUSE 3
#define _GAME 4
#define _WARBAND 5
#define _SIDE 6
#define LC_ESC      LCTL_T(KC_ESC)
#define WIN_TAB     LWIN_T(KC_TAB)
#define WIN_SLS     RWIN_T(KC_BSLS)
#define RC_BSPC     RCTL_T(KC_BSPC)
#define TS_ENT      LT(_SIDE, KC_ENT)
#define OSL_FN      OSL(_FN)
#define OSL_NUM     OSL(_NUM)
#define OSM_LW      OSM(MOD_LGUI)
#define OSM_LS      OSM(MOD_LSFT)
#define OSM_LC      OSM(MOD_RCTL)
#define OSM_LA      OSM(MOD_LALT)
#define OSM_CS      OSM(MOD_LCTL | MOD_LSFT)
#define TG_MOU      TG(_MOUSE)
#define TG_GAME     TG(_GAME)
#define TG_WARBAND  TG(_WARBAND)

float side_layer_tone[][2] = SONG(SIDE_LAYER_NOTE);
float fn_layer_tone[][2] = SONG(FN_LAYER_NOTE);
float num_layer_tone[][2] = SONG(NUM_LAYER_NOTE);
float osl_on_tone[][2] = SONG(OSL_ON_NOTE);
float mouse_layer_tone[][2] = SONG(MOU_ON_NOTE);

static bool alt_set_by_special_tab = false;

enum custom_keycodes {
    A_TAB_F = SAFE_RANGE,
    A_TAB_B,
    C_TAB_F,
    C_TAB_B,
    SP_SPC, // custom LT(_SIDE, KC_SPC)
    C_LBRC,
    C_RBRC,
    LW_SPC
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

void oneshot_layer_changed_user(uint8_t layer) {
  if (layer) {
      PLAY_SONG(osl_on_tone);
  }
}

void led_set_user(uint8_t usb_led) {
  if (!(usb_led & (1<<USB_LED_NUM_LOCK))) {
    tap_code(KC_NUMLOCK);
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if(record->event.pressed) {
        if(IS_LAYER_ON(_SIDE)) {
            PLAY_SONG(side_layer_tone);
        }
        if(IS_LAYER_ON(_NUM)) {
            PLAY_SONG(num_layer_tone);
        }
        if(IS_LAYER_ON(_FN)) {
            PLAY_SONG(fn_layer_tone);
        }
        if(IS_LAYER_ON(_MOUSE)) {
            PLAY_SONG(mouse_layer_tone);
        }
    }
    /* uprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %b, time: %u, interrupt: %b, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count); */
    switch (keycode) {
    case LW_SPC:
        if (record->event.pressed) {
            register_code(KC_LWIN);
            register_code(KC_SPC);
            unregister_code(KC_SPC);
            unregister_code(KC_LWIN);
        }
        return false;
        break;
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

    if (keycode == LC_ESC && record->event.pressed) {
        bool queue = true;
        uint8_t mods = 0;
        if ((mods = get_oneshot_mods()) && !has_oneshot_mods_timed_out()) {
            clear_oneshot_mods();
            unregister_mods(mods);
            queue = false;
        }
        if ((mods = get_oneshot_locked_mods())) {
            clear_oneshot_locked_mods();
            unregister_mods(mods);
            queue = false;
        }
        if (layer_state & (1UL<<_NUM)) {
            layer_off(_NUM);
            queue = false;
        }
        if (layer_state & (1UL<<_FN)) {
            layer_off(_FN);
            queue = false;
        }

        return queue;
    }


    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_MAIN] = LAYOUT_5x6(
     KC_1   , KC_2  , KC_3  , KC_4  , KC_5  , KC_6,                 KC_7, KC_8, KC_9,    KC_0,   KC_TILD, KC_GRV ,
     WIN_TAB, KC_Q  , KC_W  , KC_E  , KC_R  , KC_T,                 KC_Y, KC_U, KC_I,    KC_O,   KC_P,    WIN_SLS,
     LC_ESC , KC_A  , KC_S  , KC_D  , KC_F  , KC_G,                 KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, KC_QUOT,
     OSM_LS,  KC_Z  , KC_X  , KC_C  , KC_V  , KC_B,                 KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, OSM_LS,
                      KC_LBRC, KC_RBRC,                                         KC_EQL,  KC_MINS,
                                      OSL_NUM,                            TG_MOU,
                                      SP_SPC,KC_UNDS,           OSM_LC,   TS_ENT,
                                      OSM_LA,OSM_CS,            OSM_LW,   OSL_FN
  ),
  [_SIDE] = LAYOUT_5x6(
     _______,_______,_______,_______,_______,_______,                      TG_GAME,TG_WARBAND,_______,_______, AU_TOG ,RESET  ,
     _______,_______,_______,C_TAB_B,C_TAB_F,LW_SPC ,                      _______,C_LBRC ,C_RBRC ,_______,KC_PSCR,_______,
     KC_MUTE,KC_VOLD,KC_VOLU,KC_BSPC,KC_DEL ,KC_ENT ,                      KC_LEFT,KC_DOWN,KC_UP  ,KC_RGHT,_______,_______,
     KC_MPLY,KC_MPRV,KC_MNXT,A_TAB_B,A_TAB_F,OSL_FN ,                      _______,_______,_______,_______,_______,_______,
                     _______,_______,                                                      _______,_______,
                                     _______,                                      _______,
                                             _______,_______,      _______,LW_SPC ,
                                             _______,_______,      KC_CAPS,_______
  ),
  [_NUM] = LAYOUT_5x6(
     KC_NO  ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO  ,                  KC_NO  ,KC_PAST,KC_LCBR,KC_RCBR,KC_NO  ,KC_NO  ,
     KC_NO  ,KC_NO  ,KC_P7 , KC_P8 , KC_P9  ,KC_P0  ,                  KC_COMM, KC_P7 , KC_P8 , KC_P9 ,KC_P0  ,KC_NO  ,
     KC_NO  ,KC_NO  ,KC_P4 , KC_P5 , KC_P6  ,KC_NO  ,                  KC_PDOT, KC_P4 , KC_P5 , KC_P6 ,KC_NO  ,KC_NO  ,
     KC_NO  ,KC_NO  ,KC_P1 , KC_P2 , KC_P3  ,KC_NO  ,                  KC_NO  , KC_P1 , KC_P2 , KC_P3 ,KC_PSLS,KC_NO  ,
                     KC_NO , KC_NO ,                                                    KC_PLUS, KC_MINS,
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
  [_MOUSE] = LAYOUT_5x6(
     _______,_______,_______,_______,_______,_______,                  _______,_______,_______,_______,_______,_______,
     _______,_______,_______,KC_MS_U,_______,_______,                  KC_BTN4,KC_BTN1,KC_BTN3,KC_BTN2,KC_BTN5,_______,
     TG_MOU ,_______,KC_MS_L,KC_MS_D,KC_MS_R,_______,                  KC_WH_L,KC_WH_D,KC_WH_U,KC_WH_R,_______,_______,
     _______,_______,_______,_______,_______,_______,                  _______,_______,_______,_______,_______,_______,
                     _______,_______,                                                  _______,_______,
                                             _______,                  _______,
                                             _______,_______,  _______,_______,
                                             _______,_______,  _______,_______

  ),
  [_GAME] = LAYOUT_5x6(
     KC_1   , KC_2  , KC_3  , KC_4  , KC_5  , KC_6,                 KC_7, KC_8, KC_9,    KC_0,   KC_TILD, KC_GRV ,
     KC_TAB,  KC_Q  , KC_W  , KC_E  , KC_R  , KC_T,                 KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_BSLS,
     KC_LSFT, KC_A  , KC_S  , KC_D  , KC_F  , KC_G,                 KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, KC_QUOT,
     KC_LCTL, KC_Z  , KC_X  , KC_C  , KC_V  , KC_B,                 KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
                      KC_LBRC, KC_RBRC,                                         KC_EQL,  KC_MINS,
                                      MO(_SIDE),                          OSM_LC,
                                      KC_SPC,KC_UNDS,          TG_MOU,    TS_ENT,
                                      OSM_LA,TG_GAME,          OSM_LW,   OSM_LA
  ),
  [_WARBAND] = LAYOUT_5x6(
     KC_GRV, KC_5   , KC_1  , KC_2  , KC_3  , KC_4,                 KC_NO, KC_F10, KC_F11, KC_F12, KC_NO, KC_NO,
     KC_T,   KC_TAB,  KC_Q  , KC_W  , KC_E  , KC_R,                 KC_NO, KC_F7 , KC_F8 , KC_F9 , KC_NO, KC_NO,
     KC_G,   KC_LSFT, KC_A  , KC_S  , KC_D  , KC_F,                 KC_NO, KC_F4 , KC_F5 , KC_F6 , KC_NO, KC_NO,
     KC_B,   KC_LCTL, KC_Z  , KC_X  , KC_C  , KC_V,                 KC_NO, KC_F1 , KC_F2 , KC_F3 , KC_NO, KC_NO,
                      KC_NO, KC_NO,                                                KC_NO, KC_NO,
                                      MO(_SIDE),                          KC_NO,
                                      KC_SPC ,KC_ESC,            KC_NO,   KC_NO,
                                      KC_LALT,TG_WARBAND,        KC_NO,   KC_NO
  ),
};

/* remove turn on sound */
/* fix only left side working with alt tab */
