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
	#include "key.h"
	//------------
	namespace menu
	{
		enum key_t
		{
			BUTTON_LEFT    = 0,
			BUTTON_RIGHT   = 1,
			BUTTON_UP      = 2,
			BUTTON_DOWN    = 4,
			BUTTON_SELECT  = 8,
			BUTTON_UNKNOWN = 0xFF
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
				int8_t start_position(int8_t index);
				void onClick(const key_t key);

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
				virtual bool onClick(const key_t key);
				const Rectangle& rect() const;
				void setRectangle(const Rectangle &rect);

			public:
				bool     filled;
				bool     focused;
				margin_t margin;
				bool     is_rect;

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
				bool onClick(const key_t key) override;

			public:
				bool is_toggle;
				bool checked;

			private:
				callback_t _callback;
		};
		//--------------------------
		class CheckBox: public Label
		{
			public:
				explicit CheckBox();
				explicit CheckBox(const char *text, const Rectangle &rect, const font_t &font, alignment_t align);
				void draw() override;
				bool onClick(const key_t key) override;

			public:
				bool              checked;
				alignment_t alignment;
		};
		//---------------------------
		class SpinBox: public Control
		{
			public:
				explicit SpinBox(const Rectangle &rect, const font_t &font, uint32_t *ivalue, uint32_t imax = 1000, uint32_t imin = 0, uint32_t istep = 1);
				void draw() override;
				bool onClick(const key_t key) override;
				uint32_t value();

			public:
				uint32_t max;
				uint32_t min;
				uint32_t step;
				bool     edit;
				uint8_t  arrow_dir;

			private:
				uint32_t *_value;
		};

		class Line: public Control
		{
			public:
				explicit Line();
				explicit Line(const Rectangle &rect, const font_t &font, uint32_t *ivalue);
				void draw() override;

			public:
				uint32_t *_value;
		};
	} /* namespace menu */
#endif /* SCREEN_H_ */
