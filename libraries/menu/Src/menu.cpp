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
	//-----------------------------
	void Screen::onClick(key_t key)
	{
		int8_t index = find_focus();

		if(index == -1)
			return;

		Control *control_current = *(_controls + index);

		// если onClick возвращает истину, то обрабатывать не надо (уже обработано)
		if(control_current == nullptr || control_current->onClick(key))
			return;

		if(key == BUTTON_UP)
		{
			if(index > 0)
			{
				Control *control_prev = *(_controls + (index - 1));
				control_current->focused = false;
				control_prev->focused = true;
			}
		}
		else if(key == BUTTON_DOWN)
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
	//------------------------------------
	bool Control::onClick(const key_t key)
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
	//-----------------------------------
	bool Button::onClick(const key_t key)
	{
		if(key != menu::BUTTON_SELECT)
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
	//-------------------------------------
	bool CheckBox::onClick(const key_t key)
	{
		if(key == BUTTON_SELECT)
		{
			checked = !checked;
			return true;
		}

		return false;
	}
	//------------------------
	//-----class SpinBox------
	SpinBox::SpinBox(const Rectangle &rect, const font_t &font, uint32_t *ivalue, uint32_t imax, uint32_t imin, uint32_t istep):
		Control(rect, font),
		max(imax),
		min(imin),
		step(istep),
		edit(false),
		arrow_dir(0),
		_value(ivalue)
	{
		filled = is_rect = false;
	}
	//------------------
	void SpinBox::draw()
	{
		Control::draw();
		Rectangle rectNumber(_rect.X() + margin.left, _rect.Y() + margin.top, _rect.width() - margin.right*2 - 15, _rect.height() - margin.bottom*2);
		Rectangle rectArrow(rectNumber.right() - 1, rectNumber.top(), 15, rectNumber.height());

		if(!edit)
			ssd1306_DrawRect(rectNumber);
		else
			ssd1306_DrawFillRect(rectNumber);

		char text[15];
		if(edit)
		{
			ssd1306_SetColor(Black);
			ssd1306_DrawString(rectNumber, convert(text, 15, *_value, 10), _font, ALIGN_CENTER);
			ssd1306_SetColor(White);
		}
		else
		{
			ssd1306_DrawString(rectNumber, convert(text, 15, *_value, 10), _font, ALIGN_CENTER);
		}

		ssd1306_DrawRect(rectArrow);

		Point arrow1_p1(rectArrow.X() + (rectArrow.width()/2), rectArrow.top() + 3);
		Point arrow1_p2(rectArrow.X() + rectArrow.width()/2 + 3, rectArrow.top() + 8);
		Point arrow1_p3(rectArrow.X() + (rectArrow.width()/2 - 3), rectArrow.top() + 8);

		Point arrow2_p1(rectArrow.X() + (rectArrow.width()/2), rectArrow.bottom() - 3);
		Point arrow2_p2(rectArrow.X() + rectArrow.width()/2 + 3, rectArrow.bottom() - 8);
		Point arrow2_p3(rectArrow.X() + (rectArrow.width()/2 - 3), rectArrow.bottom() - 8);

		if(arrow_dir == 0)
		{
			ssd1306_DrawTriangle(arrow1_p1, arrow1_p2, arrow1_p3);
			ssd1306_DrawTriangle(arrow2_p1, arrow2_p2, arrow2_p3);
		}
		else if(arrow_dir == 1)
			ssd1306_DrawFillTriangle(arrow1_p1, arrow1_p2, arrow1_p3);
		else if(arrow_dir == 2)
			ssd1306_DrawFillTriangle(arrow2_p1, arrow2_p2, arrow2_p3);
	}
	//------------------------------------
	bool SpinBox::onClick(const key_t key)
	{
		bool result = false;

		if(key == BUTTON_SELECT)
		{
			edit = !edit;

			if(!edit)
				arrow_dir = 0;

			result = true;
		}
		else if(key == BUTTON_UP && edit)
		{
			if((*_value + step) < max)
				*_value += step;

			arrow_dir = 1;
			result = true;
		}
		else if(key == BUTTON_DOWN && edit)
		{
			if((*_value - step) > min)
				*_value -= step;

			arrow_dir = 2;
			result = true;
		}

		return result;
	}
	//-----------------------
	uint32_t SpinBox::value()
	{
		return *_value;
	}
	//-------------------------
	//-------class Line--------
	Line::Line():
		Control(),
		_value(0)
	{
		filled = false;
	}
	//----------------------------------------------------------------------
	Line::Line(const Rectangle &rect, const font_t &font, uint32_t *ivalue):
		Control(rect, font),
		_value(ivalue)
	{
		filled = false;
	}
	//---------------
	void Line::draw()
	{
		Control::draw();
		Rectangle rect(_rect.X() + margin.left, _rect.Y() + margin.top, _rect.width() - margin.right*2, _rect.height() - margin.bottom*2);

		char text[15];
		ssd1306_DrawString(rect, convert(text, 15, *_value, 10), _font, ALIGN_CENTER);
	}
} /* namespace menu */
