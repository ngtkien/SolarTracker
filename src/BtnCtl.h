// BtnCtl.h

#ifndef _BTNCTL_h
#define _BTNCTL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Define.h"
#include "Button2.h"
void btnHandle(Button2& btn);
void buttonSelect();
void buttonLeft();
void buttonDown();
void buttonUp();
void buttonRight();


void button_loop(void *pvParameters );
#endif

