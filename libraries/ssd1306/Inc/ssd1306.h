/*
 * ssd1306.h
 *
 *  Created on: 14/04/2018
 *  Update on: 10/04/2019
 *      Author: Andriy Honcharenko
 *      version: 2
 */
#pragma once
//----------------
#include "stdlib.h"
#include "string.h"
#include "main.h"
#include "format.h"
#include "fonts.h"
#include "ssd1306_defines.h"
// I2c address
#define SSD1306_I2C_ADDR        SSD1306_ADDRESS<<1 // 0x3C<<1 = 0x78

#ifdef SSD1306_128X64
    #define SSD1306_GEOMETRY		GEOMETRY_128_64
    // SSD1306 width in pixels
    #define SSD1306_WIDTH           128
    // SSD1306 LCD height in pixels
    #define SSD1306_HEIGHT          64
#elif defined(SSD1306_128X32)
    #define SSD1306_GEOMETRY		GEOMETRY_128_32
    // SSD1306 width in pixels
    #define SSD1306_WIDTH           128
    // SSD1306 LCD height in pixels
    #define SSD1306_HEIGHT          32
#endif

// SSD1306 LCD Buffer Size
#define SSD1306_BUFFER_SIZE		(SSD1306_WIDTH * SSD1306_HEIGHT / 8)

// Display commands
#define CHARGEPUMP 			0x8D
#define COLUMNADDR 			0x21
#define COMSCANDEC 			0xC8
#define COMSCANINC 			0xC0
#define DISPLAYALLON 		0xA5
#define DISPLAYALLON_RESUME 0xA4
#define DISPLAYOFF 			0xAE
#define DISPLAYON 			0xAF
#define EXTERNALVCC 		0x1
#define INVERTDISPLAY 		0xA7
#define MEMORYMODE 			0x20
#define NORMALDISPLAY 		0xA6
#define PAGEADDR 			0x22
#define SEGREMAP 			0xA0
#define SETCOMPINS 			0xDA
#define SETCONTRAST 		0x81
#define SETDISPLAYCLOCKDIV 	0xD5
#define SETDISPLAYOFFSET 	0xD3
#define SETHIGHCOLUMN 		0x10
#define SETLOWCOLUMN 		0x00
#define SETMULTIPLEX 		0xA8
#define SETPRECHARGE 		0xD9
#define SETSEGMENTREMAP 	0xA1
#define SETSTARTLINE		0x40
#define SETVCOMDETECT 		0xDB
#define SWITCHCAPVCC 		0x2
//---------------------------------------------------------
#define SWAP_INT16_T(a, b) { int16_t t = a; a = b; b = t; }
//
//  Enumeration for screen colors
//
enum SSD1306_COLOR
{
    Black   = ((uint8_t)0x00), // Black color, no pixel
    White   = ((uint8_t)0x01),  //Pixel is set. Color depends on LCD
    Inverse = ((uint8_t)0x02)
};
//-------------------
enum SSD1306_Geometry
{
  GEOMETRY_128_64   = 0,
  GEOMETRY_128_32   = 1
};
//--------------------
enum SSD1306_Alignment
{
	ALIGN_NONE    = 0,
	ALIGN_LEFT    = 1,
	ALIGN_RIGHT   = 2,
	ALIGN_TOP     = 4,
	ALIGN_BOTTOM  = 8,
	ALIGN_HCENTER = 16,
	ALIGN_VCENTER = 32,
	ALIGN_CENTER  = ALIGN_HCENTER | ALIGN_VCENTER
};
//------------
enum MessageID
{
	ID_BUTTON,
	ID_PID
};
//
//  Struct to store transformations
//
struct SSD1306_t
{
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    SSD1306_COLOR Color;
    uint8_t Initialized;
};
//------------
typedef struct
{
	uint8_t  id;
	void    *param;
} Message_TypeDef;
//---------
class Point
{
	public:
		Point(): _x(0), _y(0) {}
		Point(uint32_t x, uint32_t y): _x(x), _y(y) {}
		uint32_t x() const { return _x; }
		uint32_t y() const { return _y; }
		void setX(uint32_t x) { _x = x; }
		void setY(uint32_t y) { _y = y; }

	private:
		uint32_t _x;
		uint32_t _y;
};
//--------
class Rectangle
{
	public:
		Rectangle(): _x(0), _y(0), _width(0), _height(0) {}
		Rectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h): _x(x), _y(y), _width(w), _height(h) {}
		uint32_t bottom() const { return _y + _height; }
		uint32_t left() const { return _x; }
		uint32_t right() const { return _x + _width; }
		uint32_t top() const { return _y; }
		uint32_t X() const { return _x; }
		uint32_t Y() const { return _y; }
		uint32_t Height() const { return _height; }
		uint32_t Width() const { return _width; }
		void setX(uint32_t x) { _x = x; }
		void setY(uint32_t y) { _y = y; }
		void setWidth(uint32_t w) { _width = w; }
		void setHeight(uint32_t h) { _height = h; }
		bool isValid() const { return (_x >= 0 && _y >= 0 &&_width > 0 && _height > 0); }
		Point center() const
		{
			return Point(_width/2, _height/2);
		};

	private:
		uint32_t _x;
		uint32_t _y;
		uint32_t _width;
		uint32_t _height;
};
//------------------------------
uint16_t ssd1306_GetWidth(void);
uint16_t ssd1306_GetHeight(void);
SSD1306_COLOR ssd1306_GetColor(void);
void ssd1306_SetColor(SSD1306_COLOR color);
uint8_t ssd1306_Init(I2C_HandleTypeDef *hi2c);
void ssd1306_Fill();
void ssd1306_UpdateScreen(void);
void ssd1306_DrawPixel(uint8_t x, uint8_t y);
void ssd1306_DrawImage(const uint8_t x, const uint8_t y, const font_t& image);
void ssd1306_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
void ssd1306_DrawVerticalLine(int16_t x, int16_t y, int16_t length);
void ssd1306_DrawHorizontalLine(int16_t x, int16_t y, int16_t length);
void ssd1306_DrawRect(int16_t x, int16_t y, int16_t width, int16_t height);
void ssd1306_DrawRect(const Rectangle& rect);
void ssd1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
void ssd1306_DrawFillRect(int16_t xMove, int16_t yMove, int16_t width, int16_t height);
void ssd1306_DrawFillRect(const Rectangle& rect);
void ssd1306_DrawCircle(int16_t x0, int16_t y0, int16_t radius);
void ssd1306_FillCircle(int16_t x0, int16_t y0, int16_t radius);
void ssd1306_DrawCircleQuads(int16_t x0, int16_t y0, int16_t radius, uint8_t quads);
void ssd1306_DrawProgressBar(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t progress);
char ssd1306_DrawChar(char ch, const font_t& font);
uint16_t ssd1306_DrawUtf8Char(uint16_t ch, const font_t& font);
char ssd1306_DrawString(char* str, const font_t& font);
char ssd1306_DrawString(const Rectangle& rect, char* str, const font_t& font, const SSD1306_Alignment align = (SSD1306_Alignment)(ALIGN_NONE));
void ssd1306_SetCursor(uint8_t x, uint8_t y);
void ssd1306_DisplayOn(void);
void ssd1306_DisplayOff(void);
void ssd1306_InvertDisplay(void);
void ssd1306_NormalDisplay(void);
void ssd1306_ResetOrientation(void);
void ssd1306_FlipScreenVertically(void);
void ssd1306_MirrorScreen(void);
void ssd1306_Clear(void);
void ssd1306_Clear(const Rectangle& rect);
Rectangle ssd1306_BoundString(const char *str, const font_t& font, float factor = 1.0f);
