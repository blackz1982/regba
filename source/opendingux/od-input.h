/* Per-platform code - gpSP on GCW Zero
 *
 * Copyright (C) 2013 Dingoonity/GBATemp user Nebuleon
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licens e as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __OD_INPUT_H__
#define __OD_INPUT_H__

#define OPENDINGUX_BUTTON_COUNT 16

// These must be in the order defined in OpenDinguxKeys in od-input.c.
enum OpenDingux_Buttons {
	OPENDINGUX_BUTTON_L          = 0x0001,
	OPENDINGUX_BUTTON_R          = 0x0002,
	OPENDINGUX_BUTTON_DOWN       = 0x0004,
	OPENDINGUX_BUTTON_UP         = 0x0008,
	OPENDINGUX_BUTTON_LEFT       = 0x0010,
	OPENDINGUX_BUTTON_RIGHT      = 0x0020,
	OPENDINGUX_BUTTON_START      = 0x0040,
	OPENDINGUX_BUTTON_SELECT     = 0x0080,
	OPENDINGUX_BUTTON_FACE_DOWN  = 0x0100,
	OPENDINGUX_BUTTON_FACE_RIGHT = 0x0200,
	OPENDINGUX_BUTTON_FACE_LEFT  = 0x0400,
	OPENDINGUX_BUTTON_FACE_UP    = 0x0800,
	OPENDINGUX_ANALOG_DOWN       = 0x1000,
	OPENDINGUX_ANALOG_UP         = 0x2000,
	OPENDINGUX_ANALOG_LEFT       = 0x4000,
	OPENDINGUX_ANALOG_RIGHT      = 0x8000,
};

enum GUI_Action {
	GUI_ACTION_NONE,
	GUI_ACTION_UP,
	GUI_ACTION_DOWN,
	GUI_ACTION_LEFT,
	GUI_ACTION_RIGHT,
	GUI_ACTION_ENTER,
	GUI_ACTION_LEAVE,
};

// 0 if not fast-forwarding.
// Otherwise, the amount of frames to skip per rendered frame.
// 1 amounts to targetting 200% real-time;
// 2 amounts to targetting 300% real-time;
// 3 amounts to targetting 400% real-time;
// 4 amounts to targetting 500% real-time;
// 5 amounts to targetting 600% real-time.
extern uint_fast8_t FastForwardFrameskip;

// The number of times to target while fast-forwarding is enabled. (UI option)
// 0 means 2x, 1 means 3x, ... 4 means 6x.
extern uint32_t FastForwardTarget;

// A value indicating how sensitive the analog stick is to being moved in a
// direction.
// 0 is the least sensitive, requiring the axis to be fully engaged.
// 4 is the most sensitive, requiring the axis to be lightly tapped.
extern uint32_t AnalogSensitivity;

// A value indicating what the analog stick does in a game.
// 0 means nothing special is done (but hotkeys may use the axes).
// 1 means the analog stick is mapped to the GBA d-pad.
extern uint32_t AnalogAction;

// If this is greater than 0, a frame and its audio are skipped.
// The value then goes to FastForwardFrameskip and is decremented until 0.
extern uint_fast8_t FastForwardFrameskipControl;

/*
 * Gets the current value of the analog horizontal axis.
 * Returns:
 *   A value between -32768 (left) and 32767 (right).
 */
extern int16_t GetHorizontalAxisValue();

/*
 * Gets the current value of the analog vertical axis.
 * Returns:
 *   A value between -32768 (up) and 32767 (down).
 */
extern int16_t GetVerticalAxisValue();

/*
 * Reads the buttons pressed at the time of the function call on the input
 * mechanism most appropriate for the port being compiled, and returns a GUI
 * action according to a priority order and the buttons being pressed.
 * Returns:
 *   A single GUI action according to the priority order, or GUI_ACTION_NONE
 *   if either the user is pressing no buttons or a repeat interval has not
 *   yet passed since the last automatic repeat of the same button.
 * Output assertions:
 *   a) The return value is a valid member of 'enum GUI_Action'.
 *   b) The priority order is Enter (sub-menu), Leave, Down, Up, Right, Left.
 *      For example, if the user had pressed Down, and now presses Enter+Down,
 *      Down's repeating is stopped while Enter is held.
 *      If the user had pressed Enter, and now presses Enter+Down, Down is
 *      ignored until Enter is released.
 */
extern enum GUI_Action GetGUIAction();

#if defined GCW_ZERO
#  define LEFT_FACE_BUTTON_NAME "X"
#  define TOP_FACE_BUTTON_NAME "Y"
#elif defined DINGOO_A320
#  define LEFT_FACE_BUTTON_NAME "Y"
#  define TOP_FACE_BUTTON_NAME "X"
#endif

extern const enum OpenDingux_Buttons DefaultKeypadRemapping[12];

// The OpenDingux_Buttons corresponding to each GBA or ReGBA button.
// [0] = GBA A
// GBA B
// GBA Select
// GBA Start
// GBA D-pad Right
// GBA D-pad Left
// GBA D-pad Up
// GBA D-pad Down
// GBA R trigger
// GBA L trigger
// ReGBA rapid-fire A
// [11] = ReGBA rapid-fire B
extern enum OpenDingux_Buttons KeypadRemapping[12];

// The OpenDingux_Buttons (as bitfields) for hotkeys.
// [0] = Fast-forward while held
// Menu
// [2] = Fast-forward toggle
extern enum OpenDingux_Buttons Hotkeys[3];

/*
 * Returns true if the given hotkey is completely impossible to input on the
 * port being compiled.
 * Input:
 *   Hotkey: Bitfield of OPENDINGUX_* buttons to test.
 * Returns:
 *   true if, and only if, any of the following conditions is true:
 *   a) D-pad Up and Down are both in the bitfield;
 *   b) D-pad Left and Right are both in the bitfield;
 *   c) Analog Up and Down are both in the bitfield;
 *   d) Analog Left and Right are both in the bitfield;
 *   e) on the Dingoo A320, any analog direction is in the bitfield.
 */
extern bool IsImpossibleHotkey(enum OpenDingux_Buttons Hotkey);

/*
 * Returns the OpenDingux buttons that are currently pressed.
 */
extern enum OpenDingux_Buttons GetPressedOpenDinguxButtons();

#endif // !defined __OD_INPUT_H__
