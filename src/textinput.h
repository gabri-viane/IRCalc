#pragma once
#ifndef H_TEXTINPUT_INCLUDED
#define H_TEXTINPUT_INCLUDED 1
#include "commons.h"
#include <fxcg/keyboard.h>
#include <fxcg/display.h>

#define INPUT_MODE_TEXT 0
#define INPUT_MODE_FLOAT 1
#define INPUT_MODE_INT 2
#define INPUT_MODE_POSINT 3

#define CURSOR_FLASH_RATE 32

int getTextLine(char* buf, int maxstrlen, int x, int y, int maxdisplen, unsigned short inmode);
void DrawCursor(int x, int y, int shiftmode, int alphamode, int cursorstate, char curchar);

fn_result displayAndGetInput(unsigned char* buffer, int x, int y);

#endif // !H_TEXTINPUT_INCLUDED