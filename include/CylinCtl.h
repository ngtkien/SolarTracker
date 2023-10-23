// CylinCtl.h

#ifndef _CYLINCTL_h
#define _CYLINCTL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Define.h"
#include "Button2.h"
void move_to_postion(double pos);
void move_estimate(int m_esti_move);
double curr_length_feedback();
void motor_A_Stop(int channel);
void motor_A_Forward(int speed, int channel);
void motor_A_Backward(int speed, int channel);

#endif

