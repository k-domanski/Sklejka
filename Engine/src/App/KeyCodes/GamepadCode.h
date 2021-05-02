#pragma once
#include <enum.h>

namespace Engine {
	BETTER_ENUM(__GamepadCode, uint16_t,
		BUTTON_A               = 0,
		BUTTON_B               = 1,
		BUTTON_X               = 2,
		BUTTON_Y               = 3,
		BUTTON_LEFT_BUMPER     = 4,
		BUTTON_RIGHT_BUMPER    = 5,
		BUTTON_BACK            = 6,
		BUTTON_START           = 7,
		BUTTON_LEFT_THUMB      = 8,
		BUTTON_RIGHT_THUMB     = 9,
		BUTTON_DPAD_UP         = 10,
		BUTTON_DPAD_RIGHT      = 11,
		BUTTON_DPAD_DOWN       = 12,
		BUTTON_DPAD_LEFT       = 13,
		BUTTON_LAST            = BUTTON_DPAD_LEFT,
		
		BUTTON_CROSS       = BUTTON_A,
		BUTTON_CIRCLE      = BUTTON_B,
		BUTTON_SQUARE      = BUTTON_X,
		BUTTON_TRIANGLE    = BUTTON_Y);
	
	typedef __GamepadCode GamepadCode;

	BETTER_ENUM(__GamepadAxes, uint16_t,
		HORIZONTAL_LEFT = 0,
		VERTICAL_LEFT   = 1,
		HORIZONAL_RIGHT = 2,
		VERTICAL_RIGHT  = 3,
		TRIGGER_LEFT    = 4,
		TRIGGER_RIGHT   = 5
		);
	typedef __GamepadAxes GamepadAxes;
}