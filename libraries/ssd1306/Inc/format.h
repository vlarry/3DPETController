/*
 * format.h
 *
 *  Created on: May 12, 2021
 *      Author: gruffi
 */
#ifndef SSD1306_INC_FORMAT_H_
	#define SSD1306_INC_FORMAT_H_
	#define _CRT_NONSTDC_NO_WARNINGS	1
	//-----------------
	#include <string.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdarg.h>
	//----------------------------------------------
	#define INT_STR_SIZE	(sizeof(int)* 8 / 3 + 1)
	//--------------------------------------------------------------
	char* convert(char* buff, int size, unsigned int num, int base);
	int formatToString(char* buff, int size, const char *str, ...);
	void itox(unsigned int val, char* buff);
	int GetFloatPoint(float, int);
	char* itoa(char* buff, int size, int val);
	char* ftoa(char* buff, int size, float val, int digits);
	int GetNumBefore(char** str);
	char* FractionalPartToStr(char* , int , int , int );
#endif /* SSD1306_INC_FORMAT_H_ */
