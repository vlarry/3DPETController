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
	//-----------
	namespace key
	{
		enum KeyState
		{
			INACTIVE_STATE,
			BOUNCE_STATE,
			PRESSED_STATE,
			RELEASED_STATE
		};
		//--------------
		struct key_set_t
		{
			GPIO_TypeDef  *port;
			uint16_t       pin;
			uint32_t       id;
			GPIO_PinState  trigger_level;
			KeyState       trigger_state;
			uint32_t       timeout;
			bool           is_repeat;
		};
		//-------
		class Key
		{
			public:
				explicit Key(const key_set_t &buttonSet);
				const uint32_t id() const;
				const bool is_repeat() const;
				const bool read() const;
				void reset();
				const uint32_t timeout() const;
				const KeyState trigger_state() const;

			public:
				bool     is_active;
				KeyState state;
				uint32_t time;

			private:
				key_set_t _set;
				uint32_t  _timeout;
		};
		//------------
		class Keyboard
		{
			public:
				explicit Keyboard(Key **buttons, size_t size);
				void scan();

			public:
				static uint32_t period;

			private:
				Key     **_buttons;
				size_t    _size;
		};
	} /* namespace BUTTON */
#endif /* LIB_BUTTON_BUTTON_H_ */
