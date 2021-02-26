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
#define AUTO_SHIFT_TIMEOUT 135
#define ONESHOT_TAP_TOGGLE 2  /* Tapping this number of times holds the key until tapped once again. */
#define ONESHOT_TIMEOUT 10000  /* Time (in ms) before the one shot key is released */
/* #define DEBUG_MATRIX_SCAN_RATE */
/* #define MASTER_LEFT */
/* #define SOFT_SERIAL_PIN D3 */
// #define MASTER_RIGHT
//#define EE_HANDS
// Rows are doubled-up
