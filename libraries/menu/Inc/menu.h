/*
 * screen.h
 *
 *  Created on: 4 июл. 2021 г.
 *      Author: Gruffi
 */

#ifndef SCREEN_H_
	#define SCREEN_H_
	//------------------
	#include "ssd1306.h"
	#include "string.h"
	//------------
	namespace menu
	{
		enum ButtonIdType
		{
			BUTTON_LEFT   = 0,
			BUTTON_RIGHT  = 1,
			BUTTON_UP     = 2,
			BUTTON_DOWN   = 4,
			BUTTON_SELECT = 8
		};
		//-------------
		struct margin_t
		{
			uint8_t left;
			uint8_t right;
			uint8_t top;
			uint8_t bottom;
		};
		//--------------------------------
		using callback_t = void (*)(void);
		//------------
		class Control;
		//----------
		class Screen
		{
			public:
				explicit Screen();
				explicit Screen(const Rectangle &rect, Screen *next, Screen *prev, Control **controls, uint8_t count_controls);
				void draw();
				int8_t find_focus();
				void onClick(ButtonIdType id);

			public:
				bool is_rect;

			private:
				Screen     *_next;
				Screen     *_prev;
				Control   **_controls;
				uint8_t     _count_controls;
				Rectangle   _rect;
		};
		//-----------
		class Control
		{
			public:
				explicit Control();
				explicit Control(const Rectangle &rect, const font_t &font);
				virtual void draw();
				const font_t& font() const;
				virtual void onClick(ButtonIdType button_id = BUTTON_SELECT);
				const Rectangle& rect() const;
				void setRectangle(const Rectangle &rect);

			public:
				bool     fill;
				bool     focus;
				bool     is_focus;
				margin_t margin;

			protected:
				Rectangle _rect;
				font_t    _font;
		};
		//-------------------------
		class Label: public Control
		{
			public:
				Label();
				Label(const char *text, const Rectangle &rect, const font_t &font);
				void draw() override;

			protected:
				enum { MAX_SIZE_TEXT = 20 };
				char _text[MAX_SIZE_TEXT];
		};
		//------------------------
		class Button: public Label
		{
			public:
				Button();
				Button(const char *text, const Rectangle &rect, const font_t &font, callback_t callback = nullptr);
				void onClick(ButtonIdType button_id = BUTTON_SELECT) override;

			public:
				bool is_toggle;
				bool checked;

			private:
				callback_t _callback;
		};
	} /* namespace menu */
#endif /* SCREEN_H_ */
