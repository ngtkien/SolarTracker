#include <Arduino.h>
#include "BtnCtl.h"
#include "CylinCtl.h"
#include "Button2.h"
#include <Wire.h>
#include "LcdKeyShield.h"
#include <LiquidCrystal.h>
#include <optional>
#include "RTClib.h"
#include "Define.h"
//0:Sunday - 6:Saturday {
uint8_t TimeSeries[7][24] = {
//  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23 
	{10, 60, 20, 60, 40, 60, 10, 60, 30, 60, 10, 60, 20, 60, 40, 60, 10, 60, 30, 60, 50, 60, 20, 60}, //Sun - 0
	{60, 11, 45, 60, 14, 60, 11, 21, 33, 54, 60, 15, 60, 43, 60, 51, 14, 23, 56, 60, 22, 17, 60, 31}, //Mon - 1
	{20, 42, 22, 42, 24, 42, 21, 42, 23, 42, 20, 21, 42, 23, 42, 20, 42, 22, 42, 24, 42, 21, 42, 23}, //Tue - 2
	{30, 13, 32, 13, 34, 13, 31, 52, 33, 64, 30, 31, 12, 33, 64, 30, 60, 22, 60, 34, 60, 60, 31, 60}, //Wed - 3
	{40, 60, 42, 60, 44, 60, 41, 60, 43, 60, 40, 60, 42, 60, 44, 60, 41, 60, 43, 60, 40, 60, 42, 60}, //Thu - 4
	{50, 11, 52, 11, 54, 11, 51, 11, 53, 11, 50, 11, 52, 11, 54, 11, 51, 11, 53, 11, 50, 11, 52, 11}, //Fri - 5
	{60, 41, 60, 21, 60, 60, 60, 60, 60, 60, 60, 31, 62, 95, 64, 74, 61, 24, 63, 46, 60, 32, 62, 28}  //Sat - 6
};

uint8_t TimeSeries_Quarter_Minute[7][96] = {
/*Quarter*/  /*00h00 00h15 00h30 00h45 01h00 01h15 01h30 01h45 02h00 02h15 02h30 02h45 03h00 03h15 03h30 03h45 04h00 04h15 04h30 04h45 05h00 05h15 05h30 05h45*/ /*Quarter*/ /*06h00 06h15 06h30 06h45 07h00 07h15 07h30 07h45 08h00 08h15 08h30 08h45 09h00 09h15 09h30 09h45 10h00 10h15 10h30 10h45 11h00 11h15 11h30 11h45*/ /*Quarter*/ /*12h00 12h15 12h30 12h45 13h00 13h15 13h30 13h45 14h00 14h15 14h30 14h45 15h00 15h15 15h30 15h45 16h00 16h15 16h30 16h45 17h00 17h15 17h30 17h45*/ /*Quarter*/ /*18h00  18h15 18h30 18h45 19h00 19h15 19h30 19h45 20h00 20h15 20h30 20h45 21h00 21h15 21h30 21h45 22h00 22h15 22h30 22h45 23h00 23h15 23h30 23h45*/
/*Sun - 0*/    {10,   57,   20,   35,   40,   29,   10,   43,   30,   57,   10,   22,   20,   51,   40,   21,   10,   43,   30,   55,   50,   23,   20,   32,    /*Sun - 0*/    10,   35,   20,   34,   40,   34,   10,   34,   30,   18,   10,   34,   20,   45,   40,   13,   10,   24,   30,   45,   50,   28,   20,   45,    /*Sun - 0*/    10,   33,   20,   33,   40,   56,   10,   20,   30,   23,   10,   34,   20,   22,   40,   24,   10,   55,   30,   41,   50,   33,   20,   34,    /*Sun - 0*/    10,    44,   20,   56,   40,   33,   10,   43,   30,   43,   10,   34,   20,   21,   40,   22,   10,   44,   30,   23,   50,   34,   20,   54}, 
/*Mon - 1*/    {58,   11,   45,   21,   14,   44,   11,   21,   33,   54,   25,   15,   32,   43,   24,   51,   14,   23,   56,   23,   22,   17,   38,   31,    /*Mon - 1*/    45,   11,   45,   52,   14,   54,   11,   21,   33,   54,   25,   15,   45,   43,   18,   51,   14,   23,   56,   34,   22,   17,   36,   31,    /*Mon - 1*/    45,   11,   45,   34,   14,   58,   11,   21,   33,   54,   45,   15,   33,   43,   23,   51,   14,   23,   56,   32,   22,   17,   55,   31,    /*Mon - 1*/    45,    11,   45,   32,   14,   22,   11,   21,   33,   54,   19,   15,   32,   43,   21,   51,   14,   23,   56,   45,   22,   17,   45,   31}, 
/*Tue - 2*/    {20,   42,   22,   42,   24,   42,   21,   42,   23,   42,   20,   21,   42,   23,   42,   20,   42,   22,   42,   24,   42,   21,   42,   23,    /*Tue - 2*/    20,   42,   22,   42,   24,   42,   21,   42,   23,   42,   20,   21,   42,   23,   42,   20,   42,   22,   42,   24,   42,   21,   42,   23,    /*Tue - 2*/    20,   42,   22,   42,   24,   42,   21,   42,   23,   42,   20,   21,   42,   23,   42,   20,   42,   22,   42,   24,   42,   21,   42,   23,    /*Tue - 2*/    20,    42,   22,   42,   24,   42,   21,   42,   23,   42,   20,   21,   42,   23,   42,   20,   42,   22,   42,   24,   42,   21,   42,   23}, 
/*Wed - 3*/    {30,   13,   32,   13,   34,   13,   31,   52,   33,   23,   30,   31,   12,   33,   55,   30,   55,   22,   34,   34,   25,   43,   31,   12,    /*Wed - 3*/    30,   13,   32,   13,   34,   13,   31,   52,   33,   21,   30,   31,   12,   33,   42,   30,   20,   22,   25,   34,   23,   38,   31,   48,    /*Wed - 3*/    30,   13,   32,   13,   34,   13,   31,   52,   33,   21,   30,   31,   12,   33,   44,   30,   12,   22,   12,   34,   45,   11,   31,   12,    /*Wed - 3*/    30,    13,   32,   13,   34,   13,   31,   52,   33,   12,   30,   31,   12,   33,   17,   30,   11,   22,   45,   34,   21,   11,   31,   55}, 
/*Thu - 4*/    {40,   56,   42,   23,   44,   12,   41,   57,   43,   15,   40,   11,   42,   32,   44,   22,   41,   56,   43,   12,   40,   34,   42,   23,    /*Thu - 4*/    40,   22,   42,   32,   44,   58,   41,   54,   43,   58,   40,   21,   42,   23,   44,   23,   41,   25,   43,   16,   40,   19,   42,   23,    /*Thu - 4*/    40,   56,   42,   22,   44,   55,   41,   21,   43,   54,   40,   23,   42,   34,   44,   55,   41,   30,   43,   23,   40,   22,   42,   22,    /*Thu - 4*/    40,    54,   42,   11,   44,   12,   41,   32,   43,   22,   40,   16,   42,   24,   44,   35,   41,   23,   43,   32,   40,   29,   42,   21}, 
/*Fri - 5*/    {50,   11,   52,   11,   54,   11,   51,   11,   53,   11,   50,   11,   52,   11,   54,   11,   51,   11,   53,   11,   50,   11,   52,   11,    /*Fri - 5*/    50,   11,   52,   11,   54,   11,   51,   11,   53,   11,   50,   11,   52,   11,   54,   11,   51,   11,   53,   11,   50,   11,   52,   11,    /*Fri - 5*/    50,   11,   52,   11,   54,   11,   51,   11,   53,   11,   50,   11,   52,   11,   54,   11,   51,   11,   53,   11,   50,   11,   52,   11,    /*Fri - 5*/    50,    11,   52,   11,   54,   11,   51,   11,   53,   11,   50,   11,   52,   11,   54,   11,   51,   11,   53,   11,   50,   11,   52,   11}, 
/*Sat - 6*/    {58,   41,   55,   21,   52,   23,   34,   23,   54,   33,   45,   31,   45,   10,   38,   11,   33,   24,   29,   46,   32,   32,   49,   28,    /*Sat - 6*/    45,   41,   60,   21,   32,   21,   54,   23,   56,   32,   52,   31,   23,   45,   39,   53,   21,   24,   45,   46,   17,   32,   51,   28,    /*Sat - 6*/    34,   41,   11,   21,   43,   27,   34,   45,   56,   33,   21,   31,   54,   12,   23,   32,   43,   24,   56,   46,   32,   45,   34,   28,    /*Sat - 6*/    33,    41,   34,   21,   33,   45,   22,   34,   56,   45,   18,   31,   18,   23,   19,   34,   55,   24,   34,   46,   12,   32,   56,   28}  
};


Button2 selectBtn(pin_SELECT);
Button2 modeBtn(pin_MODE);
Button2 downBtn(pin_DOWN);
Button2 upBtn(pin_UP);
Button2 rightBtn(pin_RIGHT);

const int pin_analog = 4;

// Setting PWM properties
int freq = 10000;
int In1Channel = 0;
int In2Channel = 0;
int resolution = 12; //Resolution 8, 10, 12, 15
//LiquidCrystal lcdKeyPad(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);
extern double A;
extern double B;


LCDKeypad lcdKeyPad;
TwoWire w2rtc = TwoWire(0);
RTC_DS3231 rtc;

ScreenLcd stateScreen;
Cursor cursor;
Scroll_State scroll_state ;
char t[32];
uint32_t refresh = 5;

//using for manul_esti menu
byte esti_index = 0;
int esti_move = 0;
int final_move = 0;






void initialize() {
    
    w2rtc.begin(I2C_SDA, I2C_SCL, 100000);
    rtc.begin(&w2rtc);
    
    //Just use in first time
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));


    pinMode(pin_IN1, OUTPUT);
    pinMode(pin_IN2, OUTPUT);
    pinMode(pin_IN3, OUTPUT);
    pinMode(pin_IN4, OUTPUT);

    selectBtn.setPressedHandler(btnHandle);
    downBtn.setPressedHandler(btnHandle);
    upBtn.setPressedHandler(btnHandle);
    modeBtn.setPressedHandler(btnHandle);
    rightBtn.setPressedHandler(btnHandle);

    // Configure PWM functionalitites
    ledcSetup(In2Channel, freq, resolution);
    ledcSetup(In1Channel, freq, resolution);

    // Attach the channel to the pin_IN2 to be controlled
    ledcAttachPin(pin_IN1, In1Channel);
    ledcAttachPin(pin_IN2, In2Channel);
    motor_A_Stop(In2Channel);
}

void Calibrate(){
    Serial.println("Calibrate");
    lcdKeyPad.setCursor(0, 0);
    lcdKeyPad.printf("     Calib    ");
    lcdKeyPad.setCursor(0,1);
    lcdKeyPad.printf("     Mode     ");

    delay(1000);
    lcdKeyPad.clear();
    lcdKeyPad.setCursor(0, 0);
    lcdKeyPad.printf("    Calib  A    ");

    Serial.println("Move to A");
    motor_A_Backward(SPEED, In1Channel);

    delay(10000);

    double a = calibMin();
    Serial.printf("a: %f\n", a);
    lcdKeyPad.setCursor(0, 0);
    lcdKeyPad.printf("  New A: %f", a);

    delay(1000);
    lcdKeyPad.clear();
    lcdKeyPad.printf("    Calib  B    ");
    Serial.println("Move to B");
    motor_A_Forward(SPEED_F, In2Channel);

    delay(10000);

    double b = calibMax();
    Serial.printf("b: %f\n", b);
    lcdKeyPad.setCursor(0, 0);
    lcdKeyPad.printf("  New B: %f", b);
}
void setup() {
  
    lcdKeyPad.begin(16, 2);
    lcdKeyPad.clear();
    lcdKeyPad.setCursor(0, 0);
    lcdKeyPad.print("   Welcome to");
    lcdKeyPad.setCursor(0, 1);
    lcdKeyPad.print("  Solar Tracker");


    initialize();

    Calibrate();
    stateScreen = ScreenLcd::MAIN;
    cursor = LINE2;



    //Serial Debug
    Serial.begin(115200);
    delay(2000);
    Serial.printf("Go to Application");


    // xTaskCreatePinnedToCore(button_loop, "buttonTask", 1024, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
    
    // xTaskCreatePinnedToCore(state_run, "readerTask", 1024, NULL, 4, NULL, ARDUINO_RUNNING_CORE);
    //  vTaskStartScheduler();
}  
void state_run(void *pvParameters) {
    if (refresh == 0) {
        //lcdKeyPad.clear();
        state_machine();
        refresh = 10;
    }
    else {
        --refresh;
    }
    
    delay(50);
}
void state_cursor(){
    switch (stateScreen)
    {
    case MAIN:
        lcdKeyPad.setCursor(15, 1);
        lcdKeyPad.printf("*");
        break;
    case MANUAL:
        switch (cursor) {
        case LINE1:
            lcdKeyPad.setCursor(15, 0);
            lcdKeyPad.printf("*");
            lcdKeyPad.setCursor(15, 1);
            lcdKeyPad.printf(" ");
            break;
        case LINE2:
            lcdKeyPad.setCursor(15, 0);
            lcdKeyPad.printf(" ");
            lcdKeyPad.setCursor(15, 1);
            lcdKeyPad.printf("*");
            break;
        }
        break;
    case MANUAL_STEP:
        break;
    case MANUAL_ESTI:
        break;
    case AUTO:
        break;
    default:
        break;
    }
    
}
void state_machine() {
    
    DateTime now = rtc.now();
    float pos;
    uint16_t minute_now;
    int quarter_now;
;   switch (stateScreen) {
    case MAIN:
        Serial.println("State: main    ");
        lcdKeyPad.setCursor(0, 0);
        lcdKeyPad.printf("TIME:   %02d:%02d:%02d", now.hour(), now.minute(), now.second());
        lcdKeyPad.setCursor(0, 1);
        lcdKeyPad.printf("AUTO           ");
        
        break;
    case MANUAL:
        Serial.println("State: Manual Menu");
        lcdKeyPad.setCursor(0, 0);
        lcdKeyPad.printf("Manual Step    ");
        lcdKeyPad.setCursor(0, 1);
        lcdKeyPad.printf("Manual Esti    ");
        break;
    case MANUAL_STEP:
        Serial.println("State: Manual Step");
        lcdKeyPad.setCursor(0, 0);
        lcdKeyPad.printf("Press: Up/Down  ");
        lcdKeyPad.setCursor(0, 1);
        switch(scroll_state)
        {
        case STOP: 
            lcdKeyPad.printf("Scr State: Stop ");
        	break;
        case DOWN:
            lcdKeyPad.printf("Scr State: Down   ");
            break;
        case UP:
            lcdKeyPad.printf("Scr State: Up   ");
            break;
        default: 
            lcdKeyPad.printf("Scr State: Stop");
        }
		if(new_curr_length_feedback(A, B) >= 60 || new_curr_length_feedback(A, B) <= 0)
		{
            scroll_state = STOP;
		}
        break;
    case MANUAL_ESTI:
        esti_index = curr_length_feedback();
        Serial.println("State: Manual Esti");
        lcdKeyPad.setCursor(0, 0);
        lcdKeyPad.printf("Pos: %03d Mar: ",esti_index);
        lcdKeyPad.setCursor(0, 1);
        final_move = esti_index + esti_move;
        lcdKeyPad.printf("Est: %03d  %2d ", final_move, esti_move);
        break;
    case AUTO:
	    {
	        Serial.println("State: Mode Auto");
	        lcdKeyPad.setCursor(0, 0);
          lcdKeyPad.printf("Auto:   %02d:%02d:%02d", now.hour(), now.minute(), now.second());
          pos = new_curr_length_feedback(A, B);
	        lcdKeyPad.setCursor(0, 1);
	        lcdKeyPad.printf("Pos: %2.f mm      ", pos);
//          Serial.println(now.dayOfTheWeek());
//          Serial.println(now.hour());
//          Serial.println(TimeSeries[now.dayOfTheWeek()][now.hour()]);
//	        move_to_postion(TimeSeries[now.dayOfTheWeek()][now.hour()]);
          minute_now = now.hour() * 60 + now.minute();
          quarter_now = (minute_now - (minute_now % 15))/15;
          printf("quarter now: %d", quarter_now);
          move_to_postion(TimeSeries_Quarter_Minute[now.dayOfTheWeek()][quarter_now]);
	      break;
	    }
        
    }
    state_cursor();
}
void test() {
    KeyPad press = (KeyPad)lcdKeyPad.button();
    if (press != KEYPAD_NONE) {
        switch (press) {
        case KEYPAD_TRESHOLD_NONE:
            break;
        case KEYPAD_SELECT:
            Serial.println("Select");
            break;
        case KEYPAD_LEFT:
            Serial.println("Left");
            lcdKeyPad.clear();
            lcdKeyPad.setCursor(0, 0);
            lcdKeyPad.print("Forward");
            motor_A_Forward(SPEED, In2Channel); // Scroll Down
            break;
        case KEYPAD_DOWN:
            Serial.println("Down");
            break;
        case KEYPAD_UP:
            Serial.println("Up    ");
            lcdKeyPad.clear();
            lcdKeyPad.setCursor(0, 0);
            lcdKeyPad.print("Backward");
            motor_A_Backward(SPEED, In1Channel); // Scroll Up
            break;
        case KEYPAD_RIGHT:
            Serial.println("Right");
            lcdKeyPad.clear();
            lcdKeyPad.setCursor(0, 0);
            lcdKeyPad.print("Stop");
            motor_A_Stop(In2Channel); //Okay
            break;
        }
    }
    
}


void buttonHandle() {
    KeyPad press = (KeyPad)lcdKeyPad.button();
    if (press != KEYPAD_NONE) {
        switch (press) {
        case KEYPAD_TRESHOLD_NONE:
            break;
        case KEYPAD_SELECT:
            Serial.println("Select");
            buttonSelect();
            break;
        case KEYPAD_LEFT:         
            Serial.println("Left");
            buttonOk();
            break;
        case KEYPAD_DOWN:
            Serial.println("Down");
            buttonDown();
            break;
        case KEYPAD_UP:
            Serial.println("Up");
            buttonUp();
            break;
        case KEYPAD_RIGHT:
            Serial.println("Right");
            buttonMode();
            break;
        }
    }
}
void loop() {
    button_loop();
    if (refresh == 0) {
        //lcdKeyPad.clear();
        state_machine();
        refresh = 10;
    }
    else {
        --refresh;
    }
    
    delay(50);
    //DateTime now = rtc.now();
    //lcdKeyPad.setCursor(0, 0);
    //Serial.printf("%2d:%2d:%2d", now.hour(), now.minute(), now.second());/
    //curr_length_feedback();
}
