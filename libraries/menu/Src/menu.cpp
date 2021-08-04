/*
 * screen.cpp
 *
 *  Created on: 4 июл. 2021 г.
 *      Author: Gruffi
 */
#include <menu.h>
//------------
namespace menu
{
	//--------------------
	//----class Screen----
	Screen::Screen():
		is_rect(false),
		_next(nullptr),
		_prev(nullptr),
		_controls(nullptr),
		_count_controls(0),
		_rect(Rectangle())
	{}
	//------------------------------------------------------------------------------------------------------------
	Screen::Screen(const Rectangle &rect, Screen *next, Screen *prev, Control **controls, uint8_t count_controls):
		is_rect(false),
		_next(next),
		_prev(prev),
		_controls(controls),
		_count_controls(count_controls),
		_rect(rect)
	{
		if(_controls && _count_controls > 0 && find_focus() == -1)
		{
			for(uint8_t i = 0; i < _count_controls; i++)
			{
				Control *control = *(_controls + i);
				if(control)
				{
					control->focused = true;
					break;
				}
			}
		}
	}
	//-----------------
	void Screen::draw()
	{
		ssd1306_Clear();
		ssd1306_SetColor(White);

		if(is_rect)
			ssd1306_DrawRect(_rect);

		if(_controls != nullptr && _count_controls > 0)
		{
			int8_t index = find_focus();
			int8_t start_index = index;

			if(index > 0)
				start_index = start_position(index);

			uint16_t pos_y = 0;

			for(uint8_t i = start_index; i < _count_controls; i++)
			{
				Control *control = *(_controls + i);

				if(control != nullptr)
				{
					Rectangle rect = control->rect();
					rect.setX(0);
					rect.setY(pos_y);
					control->setRectangle(rect);

					if(pos_y < SSD1306_HEIGHT)
						control->draw();
					else
						break;

					pos_y += rect.height();
				}
			}
		}

		ssd1306_UpdateScreen();
	}
	//-------------------------
	int8_t Screen::find_focus()
	{
		for(uint8_t i = 0; i < _count_controls; i++)
		{
			Control *control = *(_controls + i);
			if(control && control->focused)
				return i;
		}

		return -1;
	}
	//----------------------------
	void Screen::onClick(key_t id)
	{
		int8_t index = find_focus();

		if(index == -1)
			return;

		Control *control_current = *(_controls + index);

		// если onClick возвращает истину, то обрабатывать не надо (уже обработано)
		if(control_current == nullptr || control_current->onClick(id))
			return;

		if(id == BUTTON_UP)
		{
			if(index > 0)
			{
				Control *control_prev = *(_controls + (index - 1));
				control_current->focused = false;
				control_prev->focused = true;
			}
		}
		else if(id == BUTTON_DOWN)
		{
			if(index < (_count_controls - 1))
			{
				Control *control_next = *(_controls + (index + 1));

				control_current->focused = false;
				control_next->focused    = true;
			}
		}
	}
	//-----------------------------------------
	int8_t Screen::start_position(int8_t index)
	{
		int8_t start_index = index;
		uint16_t height = 0;

		Control *control = *(_controls + index);
		if(control)
			height = control->rect().height();

		do
		{
			Control *control = *(_controls + --start_index);
			if(control)
			{
				height += control->rect().height();
			}
		} while(height < SSD1306_HEIGHT && start_index > 0);

		return start_index;
	}
	//------------------------
	//-----class Control------
	Control::Control():
		filled(false),
		focused(false),
		margin({ 5, 5, 5, 5 }),
		is_rect(true),
		_rect(Rectangle(0, 0, 0, 0)),
		_font(font_t({ 0, 0, nullptr }))
	{}
	//----------------------------------------------------------
	Control::Control(const Rectangle &rect, const font_t &font):
		filled(false),
		focused(false),
		margin({ 5, 5, 5, 5 }),
		is_rect(true),
		_rect(rect),
		_font(font)
	{}
	//------------------
	void Control::draw()
	{
		Rectangle rect(_rect.X() + margin.left, _rect.Y() + margin.top, _rect.width() - margin.right*2, _rect.height() - margin.bottom*2);

		ssd1306_SetColor(White);

		if(is_rect)
			ssd1306_DrawRect(rect);

		if(filled)
			ssd1306_DrawFillRect(rect);

		if(focused)
			ssd1306_DrawRect(_rect);
	}
	//---------------------------------
	const font_t& Control::font() const
	{
		return _font;
	}
	//------------------------------------------
	bool Control::onClick(const key_t button_id)
	{
		return false;
	}
	//------------------------------------
	const Rectangle& Control::rect() const
	{
		return _rect;
	}
	//-----------------------------------------------
	void Control::setRectangle(const Rectangle &rect)
	{
		_rect = rect;
	}
	//------------------------
	//------class Label-------
	Label::Label():
		Control()
	{
		_text[0] = '\0';
	}
	//------------------------------------------------------------------------
	Label::Label(const char *text, const Rectangle &rect, const font_t &font):
		Control(rect, font)
	{
		_text[0] = '\0';
		strcpy(_text, text);
	}
	//----------------
	void Label::draw()
	{
		Control::draw();

		Rectangle rect(_rect.X() + margin.left, _rect.Y() + margin.top, _rect.width() - margin.right*2, _rect.height() - margin.bottom*2);

		ssd1306_color colorText = White;

		if(filled)
			colorText = Black;

		ssd1306_SetColor(colorText);
		ssd1306_DrawString(rect, _text, _font, ALIGN_CENTER);
	}
	//-------------------------
	//------class Button-------
	Button::Button():
		Label(),
		is_toggle(false),
		checked(false),
		_callback(nullptr)
	{}
	//-----------------------------------------------------------------------------------------------
	Button::Button(const char *text, const Rectangle &rect, const font_t &font, callback_t callback):
		Label(text, rect, font),
		is_toggle(false),
		checked(false),
		_callback(callback)
	{}
	//-----------------------------------------
	bool Button::onClick(const key_t button_id)
	{
		if(button_id != menu::BUTTON_SELECT)
			return false;

		if(is_toggle)
		{
			checked = !checked;
			filled = checked;
		}

		if(_callback != nullptr)
			_callback();

		return true;
	}
	//-------------------------
	//-----class CheckBox------
	CheckBox::CheckBox():
		Label(),
		checked(false),
		alignment(ALIGN_RIGHT)
	{
		filled = false;
		is_rect = false;
	}
	//-------------------------------------------------------------------------------------------------
	CheckBox::CheckBox(const char *text, const Rectangle &rect, const font_t &font, alignment_t align):
		Label(text, rect, font),
		checked(false),
		alignment(align)
	{
		filled = false;
		is_rect = false;
	}
	//-------------------
	void CheckBox::draw()
	{
		Control::draw();
		Rectangle rect(_rect.X() + margin.left, _rect.Y() + margin.top, _rect.width() - margin.right*2, _rect.height() - margin.bottom*2);
		Rectangle square(rect.X(), rect.Y(), rect.height(), rect.height());

		if(alignment == ALIGN_LEFT)
		{
			square.setX(rect.right() - square.width());
		}

		alignment_t align = (alignment_t)(ALIGN_VCENTER | alignment);

		ssd1306_SetColor(White);
		ssd1306_SetCursor(square.X(), square.Y());
		ssd1306_DrawRect(square);
		if(checked)
		{
			uint16_t len = square.width()/2;
			uint16_t cx = square.width()/2;
			uint16_t cy = square.height()/2;
			uint16_t pos_x = cx - len/2 - 1;
			uint16_t pos_y = cy - len/2 - 1;
			Rectangle indicatorRect(pos_x + square.X(), pos_y + square.Y(), len, len);
			ssd1306_DrawFillRect(indicatorRect);
		}

		//rect.setX(rect.X() - 5);
		ssd1306_DrawString(rect, _text, _font, align);
	}
	//-------------------------------------------
	bool CheckBox::onClick(const key_t button_id)
	{
		if(button_id == BUTTON_SELECT)
		{
			checked = !checked;
			return true;
		}

		return false;
	}
} /* namespace menu */
