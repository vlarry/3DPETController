/*
 * button.cpp
 *
 *  Created on: 19 апр. 2021 г.
 *      Author: gruffi
 */
#include "button.h"
//--------------
namespace button
{
	// Class Button
	Button::Button(const BUTTON_SetDef &button_set):
		is_active(false),
		state(INACTIVE_STATE),
		time(0),
		_set(button_set)
	{}
	//-------------------------------
	const uint32_t Button::id() const
	{
		return _set.id;
	}
	//----------------------------------
	const bool Button::is_repeat() const
	{
		return _set.is_repeat;
	}
	//-----------------------------
	const bool Button::read() const
	{
		return (HAL_GPIO_ReadPin(_set.port, _set.pin) == _set.trigger_level);
	}
	//------------------
	void Button::reset()
	{
		is_active = false;
		state = INACTIVE_STATE;
		time = 0;
	}
	//------------------------------------
	const uint32_t Button::timeout() const
	{
		return _set.timeout;
	}
	//---------------------------------------------
	const ButtonState Button::trigger_state() const
	{
		return _set.trigger_state;
	}
	//---------------
	// Class Keyboard
	uint32_t Keyboard::period = 100;
	Keyboard::Keyboard(Button **buttons, size_t size)
	{
		_buttons = buttons;
		_size = size;
	}
	//-------------------
	void Keyboard::scan()
	{
		for(uint8_t i = 0; i < _size; i++)
		{
			Button *button = *(_buttons + i);

			if(button)
			{
				if(button->read())
				{
					switch(button->state)
					{
						case INACTIVE_STATE:
							button->state = BOUNCE_STATE;
						break;
						case BOUNCE_STATE:
							if(button->time >= button->timeout())
							{
								button->state = PRESSED_STATE;
								if(button->trigger_state() == PRESSED_STATE)
								{
									button->is_active = true;
									button->time = 0;
								}
							}
						break;
						case PRESSED_STATE:
							if(button->is_repeat())
							{
								if(button->time >= button->timeout())
								{
									button->is_active = true;
								}
							}
						break;

						default: break;
					}

					button->time += period;
				}
				else
				{
					if(button->trigger_state() == RELEASED_STATE)
					{
						if(button->time >= button->timeout())
						{
							button->is_active = true;
						}
						else
							button->reset();
					}
					else
						button->reset();
				}
			}
		}
	}
} /* namespace BUTTON */
