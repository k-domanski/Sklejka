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
		BUTTON_GUIDE           = 8,
		BUTTON_LEFT_THUMB      = 9,
		BUTTON_RIGHT_THUMB     = 10,
		BUTTON_DPAD_UP         = 11,
		BUTTON_DPAD_RIGHT      = 12,
		BUTTON_DPAD_DOWN       = 13,
		BUTTON_DPAD_LEFT       = 14,
		BUTTON_LAST            = BUTTON_DPAD_LEFT,
		
		BUTTON_CROSS       = BUTTON_A,
		BUTTON_CIRCLE      = BUTTON_B,
		BUTTON_SQUARE      = BUTTON_X,
		BUTTON_TRIANGLE    = BUTTON_Y);
	
	typedef __GamepadCode GamepadCode;
}