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
	{}
	//-----------------
	void Screen::draw()
	{
		ssd1306_Clear();
		ssd1306_SetColor(White);

		if(is_rect)
			ssd1306_DrawRect(_rect);

		if(_controls != nullptr && _count_controls > 0)
		{
			uint16_t pos_y = 0;
			for(uint8_t i = 0; i < _count_controls; i++)
			{
				Control *control = *(_controls + i);

				if(control != nullptr)
				{
					Rectangle rect = control->rect();
					rect.setX(0);
					rect.setY(pos_y);
					control->setRectangle(rect);
					control->draw();
					pos_y += rect.Height();
				}
			}
		}

		ssd1306_UpdateScreen();
	}
	//-----------------------------------
	void Screen::onClick(ButtonIdType id)
	{
		if(id == BUTTON_SELECT)
		{
			for(uint8_t i = 0; i < _count_controls; i++)
			{
				Control *control = *(_controls + i);
				if(control && control->is_focus && control->focus)
				{
					control->onClick(id);
				}
			}
		}
	}
	//------------------------
	//-----class Control------
	Control::Control():
		fill(false),
		focus(false),
		is_focus(false),
		margin({ 5, 5, 5, 5 }),
		_rect(Rectangle(0, 0, 0, 0)),
		_font(font_t({ 0, 0, nullptr }))
	{}
	//----------------------------------------------------------
	Control::Control(const Rectangle &rect, const font_t &font):
		fill(false),
		focus(false),
		is_focus(false),
		margin({ 5, 5, 5, 5 }),
		_rect(rect),
		_font(font)
	{}
	//------------------
	void Control::draw()
	{
		Rectangle rect(_rect.X() + margin.left, _rect.Y() + margin.top, _rect.Width() - margin.right*2, _rect.Height() - margin.bottom*2);

		ssd1306_SetColor(White);
		ssd1306_DrawRect(rect);

		if(fill)
			ssd1306_DrawFillRect(rect);

		if(is_focus && focus)
			ssd1306_DrawRect(_rect);
	}
	//---------------------------------
	const font_t& Control::font() const
	{
		return _font;
	}
	//-------------------------------------------
	void Control::onClick(ButtonIdType button_id)
	{}
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

		Rectangle rect(_rect.X() + margin.left, _rect.Y() + margin.top, _rect.Width() - margin.right*2, _rect.Height() - margin.bottom*2);

		SSD1306_COLOR colorText = White;

		if(fill)
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
	{
		is_focus = true;
	}
	//-----------------------------------------------------------------------------------------------
	Button::Button(const char *text, const Rectangle &rect, const font_t &font, callback_t callback):
		Label(text, rect, font),
		is_toggle(false),
		checked(false),
		_callback(callback)
	{
		is_focus = true;
	}
	//------------------------------------------
	void Button::onClick(ButtonIdType button_id)
	{
		if(is_toggle)
		{
			checked = !checked;
			fill = checked;
		}

		if(_callback != nullptr)
			_callback();
	}
} /* namespace menu */
