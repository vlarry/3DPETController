/*
 * button.h
 *
 *  Created on: 19 апр. 2021 г.
 *      Author: gruffi
 */

#ifndef LIB_BUTTON_BUTTON_H_
	#define LIB_BUTTON_BUTTON_H_
	//---------------
	#include "main.h"
	//--------------
	namespace button
	{
		enum ButtonState
		{
			INACTIVE_STATE,
			BOUNCE_STATE,
			PRESSED_STATE,
			RELEASED_STATE
		};
		//------------------
		struct BUTTON_SetDef
		{
			GPIO_TypeDef  *port;
			uint16_t       pin;
			uint32_t       id;
			GPIO_PinState  trigger_level;
			ButtonState    trigger_state;
			uint32_t       timeout;
			bool           is_repeat;
		};
		//----------
		class Button
		{
			public:
				explicit Button(const BUTTON_SetDef &buttonSet);
				const uint32_t id() const;
				const bool is_repeat() const;
				const bool read() const;
				void reset();
				const uint32_t timeout() const;
				const ButtonState trigger_state() const;

			public:
				bool        is_active;
				ButtonState state;
				uint32_t    time;

			private:
				BUTTON_SetDef _set;
				uint32_t      _timeout;
		};
		//------------
		class Keyboard
		{
			public:
				explicit Keyboard(Button **buttons, size_t size);
				void scan();

			public:
				static uint32_t period;

			private:
				Button** _buttons;
				size_t   _size;
		};
	} /* namespace BUTTON */
#endif /* LIB_BUTTON_BUTTON_H_ */
