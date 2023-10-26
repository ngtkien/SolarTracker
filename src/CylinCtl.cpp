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

int anaA = 279;
int anaB = 923;
double A = anaA*4096/3300;
double B = anaB*4096/3300;

double calibMax(){
    double y;
    uint32_t x;
    for (byte i = 0; i < 10; i++) {
        x = analogRead(2);
        Serial.printf("x: %d\n", x);
        y += double(x); //D2
    }
    //Serial.printf("y before: %f\n", y);
    y = y / 10;

    anaB = y;
    B = anaB*4096/3300;
    return B;
}
double calibMin(){
  double y;
    uint32_t x;
    for (byte i = 0; i < 10; i++) {
        x = analogRead(2);
        Serial.printf("x: %d\n", x);
        y += double(x); //D2
    }
    //Serial.printf("y before: %f\n", y);
    y = y / 10;
    anaA = y;
    A = anaA*4096/3300;
    return A;
}
double new_curr_length_feedback(double A, double B) {
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

    Serial.print("analog avg: ");
    Serial.println(y);
    //y = analogRead(2);
    if (y == 0){
      ymm = 0;
    }
    else ymm = (60/(B-A)) * y + 60*B/(A-B);

    Serial.println(ymm);
    
    return ymm;
    //delay(50);
}
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

    Serial.print("analog avg: ");
    Serial.println(y);
    //y = analogRead(2);
    if (y == 0){
      ymm = 0;
    }
    else ymm = (60/(B-A)) * y + 60*A/(A-B);

    Serial.println(ymm);
    
    return ymm;
    //delay(50);
}

void motor_A_Stop(int channel) {
    // ledcDetachPin(pin_IN1);
    // ledcDetachPin(pin_IN2);
    digitalWrite(pin_IN1, LOW);
    digitalWrite(pin_IN2, LOW);
    //ledcWrite(channel, MAX_SPEED);
}
void motor_A_Forward(int speed, int channel) {
    // speed = constrain(speed, MIN_SPEED, MAX_SPEED);
    // ledcAttachPin(pin_IN2, channel);
    // ledcDetachPin(pin_IN1);
    // digitalWrite(pin_IN1, HIGH);
    // ledcWrite(channel, MAX_SPEED - speed);

    digitalWrite(pin_IN1, HIGH);
    digitalWrite(pin_IN2, LOW);
}

void motor_A_Backward(int speed, int channel) {
    // speed = constrain(speed, MIN_SPEED, MAX_SPEED);
    // ledcAttachPin(pin_IN1, channel);
    // ledcDetachPin(pin_IN2);
    // digitalWrite(pin_IN2, HIGH);
    // ledcWrite(channel, MAX_SPEED - speed);

    digitalWrite(pin_IN1, LOW);
    digitalWrite(pin_IN2, HIGH);
}
void move_to_postion(double pos)
{
    DateTime now = rtc.now();
    //pos_nex = pos
    Serial.println("pos input");
    Serial.println(pos);
    double pos_curr = new_curr_length_feedback(A, B);
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
            pos_curr = new_curr_length_feedback(A, B);
            //Serial.println("pos_curr");
            //Serial.println(pos_curr);
            //Serial.println("pos input");
            //Serial.println(pos);
        } while ((pos_curr < pos));
        motor_A_Stop(In2Channel);
        delay(50);
        pos_curr = new_curr_length_feedback(A, B);
        if(pos_curr < pos){
          motor_A_Forward(SPEED_F, In2Channel);
          do
          {
              if (stateScreen == AUTO) {
                lcdKeyPad.setCursor(0, 0);
                lcdKeyPad.printf("Auto:   %02d:%02d:%02d", now.hour(), now.minute(), now.second());
              }
              delay(50);
              pos_curr = new_curr_length_feedback(A, B);
              //Serial.println("pos_curr continue");
              //Serial.println(pos_curr);
              //Serial.println("pos input");
              //Serial.println(pos);
          } while ((pos_curr < pos));
          //Serial.println("Stop at 2nd");
          //Serial.println(pos_curr);
          motor_A_Stop(In2Channel);
          delay(50);
          pos_curr = new_curr_length_feedback(A, B);
          if(pos_curr < pos){
            motor_A_Forward(SPEED_F, In2Channel);
            do
            {
                if (stateScreen == AUTO) {
                  lcdKeyPad.setCursor(0, 0);
                  lcdKeyPad.printf("Auto:   %02d:%02d:%02d", now.hour(), now.minute(), now.second());
                }
                delay(50);
                pos_curr = new_curr_length_feedback(A, B);
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
              pos_curr = new_curr_length_feedback(A, B);
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
            pos_curr = new_curr_length_feedback(A, B);
            //Serial.println("pos_curr");
            //Serial.println(pos_curr);
            //Serial.println("pos input");
            //Serial.println(pos);
        } while ((pos_curr > pos));
        motor_A_Stop(In1Channel);
        delay(50);
        pos_curr = new_curr_length_feedback(A, B);
        if(pos_curr > pos){
          motor_A_Backward(SPEED, In1Channel);
          do
          {
              if (stateScreen == AUTO) {
                lcdKeyPad.setCursor(0, 0);
                lcdKeyPad.printf("Auto:   %02d:%02d:%02d", now.hour(), now.minute(), now.second());
              }
              delay(50);
              pos_curr = new_curr_length_feedback(A, B);
//              Serial.println("pos_curr continue1");
//              Serial.println(pos_curr);
//              Serial.println("pos input");
//              Serial.println(pos);
          } while ((pos_curr > pos));
//          Serial.println("Stop at 2nd");
//          Serial.println(pos_curr);
          motor_A_Stop(In1Channel);
          delay(50);
          pos_curr = new_curr_length_feedback(A, B);
          if(pos_curr > pos){
            motor_A_Backward(SPEED, In1Channel);
            do
            {
                if (stateScreen == AUTO) {
                  lcdKeyPad.setCursor(0, 0);
                  lcdKeyPad.printf("Auto:   %02d:%02d:%02d", now.hour(), now.minute(), now.second());
                }
                delay(50);
                pos_curr = new_curr_length_feedback(A, B);
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
                pos_curr = new_curr_length_feedback(A, B);
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
                pos_curr = new_curr_length_feedback(A, B);
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
    byte pos_curr = new_curr_length_feedback(A, B);
    byte pos_next;
    if (m_esti_move > 0)
    { 
        do
        {
          motor_A_Forward(SPEED_F, In2Channel);
          pos_next = new_curr_length_feedback(A, B);
            //Di toi lon hon hoac bang thi dung
        } while (!(pos_next >= pos_curr + m_esti_move));
        motor_A_Stop(In2Channel);
    }
    else
    {
        do
        {
            motor_A_Backward(SPEED, In1Channel);
            pos_next = new_curr_length_feedback(A, B);
        } while (!(pos_next <= pos_curr + m_esti_move));
        motor_A_Stop(In1Channel);

    }
    esti_move = 0;
    Serial.printf("Move: %d", pos_next);
}
