// 
// 
// 

#include "CylinCtl.h"
#include "LcdKeyShield.h"
#include <LiquidCrystal.h>
#include "RTClib.h"
#include <Wire.h>

extern  Button2 selectBtn;
extern Button2 modeBtn;
extern Button2 downBtn;
extern Button2 upBtn;
extern Button2 rightBtn;

extern ScreenLcd stateScreen;
extern LCDKeypad lcdKeyPad;
extern RTC_DS3231 rtc;
extern Cursor cursor;
extern Scroll_State scroll_state;
extern char t[32];
extern uint32_t refresh;

//using for manul_esti menu
extern byte esti_index;
extern int esti_move;

extern  int freq;
extern  int In1Channel;
extern  int In2Channel;
extern  int resolution; //Resolution 8, 10, 12, 15

double curr_length_feedback() {
    double y;
    double ymm;
    uint32_t x;
    y = 0;
    for (byte i = 0; i < 100; i++) {
        x = analogRead(2);
        //Serial.printf("x: %d\n", x);
        y += double(x); //D2
    }
    //Serial.printf("y before: %f\n", y);
    y = y / 100;
    //Serial.println(y);
    //y = analogRead(2);
    ymm = -y*100/906+374800/906;
    Serial.println(ymm);
    return ymm;
    //delay(50);
}

void motor_A_Stop(int channel) {
    ledcDetachPin(pin_IN1);
    ledcDetachPin(pin_IN2);
    digitalWrite(pin_IN1, LOW);
    digitalWrite(pin_IN2, LOW);
    //ledcWrite(channel, MAX_SPEED);
}
void motor_A_Forward(int speed, int channel) {
    speed = constrain(speed, MIN_SPEED, MAX_SPEED);
    ledcAttachPin(pin_IN2, channel);
    ledcDetachPin(pin_IN1);
    digitalWrite(pin_IN1, HIGH);
    ledcWrite(channel, MAX_SPEED - speed);
}

void motor_A_Backward(int speed, int channel) {
    speed = constrain(speed, MIN_SPEED, MAX_SPEED);
    ledcAttachPin(pin_IN1, channel);
    ledcDetachPin(pin_IN2);
    digitalWrite(pin_IN2, HIGH);
    ledcWrite(channel, MAX_SPEED - speed);
}
void move_to_postion(double pos)
{
    DateTime now = rtc.now();
    //pos_nex = pos
    Serial.println("pos input");
    Serial.println(pos);
    double pos_curr = curr_length_feedback();
    Serial.println("pos curr");
    Serial.println(pos_curr);
    //double pos_next;
    Serial.println("direct");
    double direct = pos - pos_curr;
    Serial.println(direct);

    if (direct >= 0.5)
    {
        Serial.println("Auto: Scroll up -> pos");
        motor_A_Forward(SPEED_F, In2Channel);
        do
        {
            if (stateScreen == AUTO) {
              lcdKeyPad.setCursor(0, 0);
              lcdKeyPad.printf("Auto:   %02d:%02d:%02d", now.hour(), now.minute(), now.second());
            }
            delay(50);
            pos_curr = curr_length_feedback();
            //Serial.println("pos_curr");
            //Serial.println(pos_curr);
            //Serial.println("pos input");
            //Serial.println(pos);
        } while ((pos_curr < pos));
        motor_A_Stop(In2Channel);
        delay(50);
        pos_curr = curr_length_feedback();
        if(pos_curr < pos){
          motor_A_Forward(SPEED_F, In2Channel);
          do
          {
              if (stateScreen == AUTO) {
                lcdKeyPad.setCursor(0, 0);
                lcdKeyPad.printf("Auto:   %02d:%02d:%02d", now.hour(), now.minute(), now.second());
              }
              delay(50);
              pos_curr = curr_length_feedback();
              //Serial.println("pos_curr continue");
              //Serial.println(pos_curr);
              //Serial.println("pos input");
              //Serial.println(pos);
          } while ((pos_curr < pos));
          //Serial.println("Stop at 2nd");
          //Serial.println(pos_curr);
          motor_A_Stop(In2Channel);
          delay(50);
          pos_curr = curr_length_feedback();
          if(pos_curr < pos){
            motor_A_Forward(SPEED_F, In2Channel);
            do
            {
                if (stateScreen == AUTO) {
                  lcdKeyPad.setCursor(0, 0);
                  lcdKeyPad.printf("Auto:   %02d:%02d:%02d", now.hour(), now.minute(), now.second());
                }
                delay(50);
                pos_curr = curr_length_feedback();
                //Serial.println("pos_curr continue");
                //Serial.println(pos_curr);
                //Serial.println("pos input");
                //Serial.println(pos);
            } while ((pos_curr < pos));
            //Serial.println("Stop at 3rd");
            //Serial.println(pos_curr);
            motor_A_Stop(In2Channel);
          }
        }
        else if (pos_curr > pos + 0.8) {
          motor_A_Backward(SPEED, In1Channel);
          do
          {
              if (stateScreen == AUTO) {
                lcdKeyPad.setCursor(0, 0);
                lcdKeyPad.printf("Auto:   %02d:%02d:%02d", now.hour(), now.minute(), now.second());
              }
              delay(50);
              pos_curr = curr_length_feedback();
//              Serial.println("pos_curr continue1");
//              Serial.println(pos_curr);
//              Serial.println("pos input");
//              Serial.println(pos);
          } while ((pos_curr > pos));
//          Serial.println("Stop at 2nd");
//          Serial.println(pos_curr);
          motor_A_Stop(In1Channel);
        }
    }
    else if (direct <= -0.5)
    {
        Serial.println("Auto: Scroll down -> pos");
        motor_A_Backward(SPEED, In1Channel);
        do
        {
            if (stateScreen == AUTO) {
              lcdKeyPad.setCursor(0, 0);
              lcdKeyPad.printf("Auto:   %02d:%02d:%02d", now.hour(), now.minute(), now.second());
            }
            delay(50);
            pos_curr = curr_length_feedback();
            //Serial.println("pos_curr");
            //Serial.println(pos_curr);
            //Serial.println("pos input");
            //Serial.println(pos);
        } while ((pos_curr > pos));
        motor_A_Stop(In1Channel);
        delay(50);
        pos_curr = curr_length_feedback();
        if(pos_curr > pos){
          motor_A_Backward(SPEED, In1Channel);
          do
          {
              if (stateScreen == AUTO) {
                lcdKeyPad.setCursor(0, 0);
                lcdKeyPad.printf("Auto:   %02d:%02d:%02d", now.hour(), now.minute(), now.second());
              }
              delay(50);
              pos_curr = curr_length_feedback();
//              Serial.println("pos_curr continue1");
//              Serial.println(pos_curr);
//              Serial.println("pos input");
//              Serial.println(pos);
          } while ((pos_curr > pos));
//          Serial.println("Stop at 2nd");
//          Serial.println(pos_curr);
          motor_A_Stop(In1Channel);
          delay(50);
          pos_curr = curr_length_feedback();
          if(pos_curr > pos){
            motor_A_Backward(SPEED, In1Channel);
            do
            {
                if (stateScreen == AUTO) {
                  lcdKeyPad.setCursor(0, 0);
                  lcdKeyPad.printf("Auto:   %02d:%02d:%02d", now.hour(), now.minute(), now.second());
                }
                delay(50);
                pos_curr = curr_length_feedback();
//                Serial.println("pos_curr continue2");
//                Serial.println(pos_curr);
//                Serial.println("pos input");
//                Serial.println(pos);
            } while ((pos_curr > pos));
//            Serial.println("Stop at 3rd");
//            Serial.println(pos_curr);
            motor_A_Stop(In1Channel);
          }
          else if (pos_curr < pos + 0.5) {
            motor_A_Forward(SPEED_F, In2Channel);
            do
            {
                if (stateScreen == AUTO) {
                  lcdKeyPad.setCursor(0, 0);
                  lcdKeyPad.printf("Auto:   %02d:%02d:%02d", now.hour(), now.minute(), now.second());
                }
                delay(50);
                pos_curr = curr_length_feedback();
//                Serial.println("pos_curr continue2'");
//                Serial.println(pos_curr);
//                Serial.println("pos input");
//                Serial.println(pos);
            } while ((pos_curr < pos));
//            Serial.println("Stop at 3'rd");
//            Serial.println(pos_curr);
            motor_A_Stop(In2Channel);
          }
       }
       else if (pos_curr < pos) {
          motor_A_Forward(SPEED_F, In2Channel);
          do
            {
                if (stateScreen == AUTO) {
                  lcdKeyPad.setCursor(0, 0);
                  lcdKeyPad.printf("Auto:   %02d:%02d:%02d", now.hour(), now.minute(), now.second());
                }
                delay(50);
                pos_curr = curr_length_feedback();
//                Serial.println("pos_curr continue1'");
//                Serial.println(pos_curr);
//                Serial.println("pos input");
//                Serial.println(pos);
            } while ((pos_curr < pos));
//            Serial.println("Stop at 2'nd");
//            Serial.println(pos_curr);
            motor_A_Stop(In2Channel);
       }
    }
}
void move_estimate(int m_esti_move)
{
    //pos_next = pos_current +- m_esti_move
    byte pos_curr = curr_length_feedback();
    byte pos_next;
    if (m_esti_move > 0)
    { 
        do
        {
          motor_A_Forward(SPEED_F, In2Channel);
          pos_next = curr_length_feedback();
            //Di toi lon hon hoac bang thi dung
        } while (!(pos_next >= pos_curr + m_esti_move));
        motor_A_Stop(In2Channel);
    }
    else
    {
        do
        {
            motor_A_Backward(SPEED, In1Channel);
            pos_next = curr_length_feedback();
        } while (!(pos_next <= pos_curr + m_esti_move));
        motor_A_Stop(In1Channel);

    }
    esti_move = 0;
    Serial.printf("Move: %d", pos_next);
}
