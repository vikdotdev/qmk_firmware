/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once

#define USE_SERIAL
#define ONESHOT_TAP_TOGGLE 2
#define ONESHOT_TIMEOUT 5000
#define B6_AUDIO
#define MUSIC_MASK keycode != KC_NO

#undef MATRIX_COL_PINS
#undef MATRIX_ROW_PINS
#define MATRIX_COL_PINS { D4, C6, D7, E6, B4, B5 }
#define MATRIX_ROW_PINS { F5, F6, F7, B1, B3, B2 }

#define SIDE_LAYER_NOTE S__NOTE(_G3)
#define FN_LAYER_NOTE S__NOTE(_C3)
#define NUM_LAYER_NOTE S__NOTE(_E3)
#define OSL_ON_NOTE S__NOTE(_B3)
#define MOU_ON_NOTE S__NOTE(_C3)

#undef MOUSEKEY_MAX_SPEED
#undef MOUSEKEY_INTERVAL
#define MOUSEKEY_INTERVAL 7
#define MOUSEKEY_DELAY 0
#undef MOUSEKEY_TIME_TO_MAX
#define MOUSEKEY_TIME_TO_MAX 100
#define MOUSEKEY_MAX_SPEED 2
#define MOUSEKEY_WHEEL_DELAY 0
#define MOUSEKEY_WHEEL_INTERVAL 80
#define MOUSEKEY_WHEEL_MAX_SPEED 12
#define MOUSEKEY_WHEEL_TIME_TO_MAX 50
