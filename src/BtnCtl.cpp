// 
// 
// 

#include "BtnCtl.h"
#include "CylinCtl.h"
extern  Button2 selectBtn;
extern Button2 modeBtn;
extern Button2 downBtn;
extern Button2 upBtn;
extern Button2 rightBtn;

extern ScreenLcd stateScreen;
extern Cursor cursor;
extern Scroll_State scroll_state;
extern char t[32];
extern uint32_t refresh;

//using for manul_esti menu
extern byte esti_index;
extern int esti_move;
extern int final_move;
extern  int freq;
extern  int In1Channel;
extern  int In2Channel;
extern  int resolution; //Resolution 8, 10, 12, 15
void btnHandle(Button2& btn)
{
    if (btn == selectBtn)
    {
        buttonSelect();
    }
    else  if (btn == modeBtn)
    {
        //ButtonMode

        buttonLeft();
    }
    else if (btn == downBtn)
    {
        buttonDown();
    }
    else if (btn == upBtn)
    {
        buttonUp();
    }
    else if (btn == rightBtn)
    {
        buttonRight();
    }

}

void buttonSelect() {
    switch (stateScreen) {
    case MAIN:
        if (cursor == LINE1) {
            //TODO: Change Screen to Auto screen
            Serial.println("MAIN -> AUTO");
            stateScreen = AUTO;
        }
        else if (cursor == LINE2) {
            //TODO: Change Screen to Auto screen
            Serial.println("MAIN -> AUTO");
            stateScreen = AUTO;
        }
        break;
    case MANUAL:
        if (cursor == LINE1) {
            //TODO: Change Screen to MANUAL_STEP screen
            Serial.println("MANUAL -> MANUAL_STEP");
            stateScreen = MANUAL_STEP;
        }
        else if (cursor == LINE2) {
            Serial.println("MANUAL -> MANUAL_ESTI");
            //TODO: Change Screen to MANUAL_ESTI screen
            stateScreen = MANUAL_ESTI;
        }
        break;
    }
}
void buttonLeft() {

    switch (stateScreen) {

    case MANUAL_ESTI:
        Serial.print("Manual Estimate run ");
        esti_move = 0;
        //Move to Es variable on Screen;
        move_to_postion(final_move);
        //move_estimate(esti_move);
        break;
    case MANUAL_STEP:
        scroll_state = STOP;
        Serial.print("Manual Step Stop ");
        motor_A_Stop(In1Channel);
        motor_A_Stop(In2Channel);
        break;

    }
}
void buttonDown() {
    switch (stateScreen) {
    case MAIN:
        break;
    case MANUAL:
        if (cursor == LINE1) {
            //TODO: Change Screen to MANUAL_STEP screen
            Serial.println("LINE1 -> LINE2");
            cursor = LINE2;
        }
        else if (cursor == LINE2) {
            Serial.println("LINE2 -> LINE1");
            //TODO: Change Screen to MANUAL_ESTI screen
            cursor = LINE1;
        }
        break;
    case MANUAL_STEP:
        Serial.println("Manual Step: Press Scroll Down");
        scroll_state = DOWN;
        motor_A_Backward(SPEED, In1Channel); // Scroll Down
        break;
    case MANUAL_ESTI:
        Serial.println("Manual ESTI: decrease esti_move");
        if (esti_index + esti_move > 0)
        {
            --esti_move;
        }
        break;
    }
}
void buttonUp() {

    switch (stateScreen) {
    case MAIN:
        break;
    case MANUAL:
        if (cursor == LINE1) {
            //TODO: Change Screen to MANUAL_STEP screen
            Serial.println("LINE1 -> LINE2");
            cursor = LINE2;
        }
        else if (cursor == LINE2) {
            Serial.println("LINE2 -> LINE1");
            //TODO: Change Screen to MANUAL_ESTI screen
            cursor = LINE1;

        }
        break;
    case MANUAL_STEP:
        Serial.println("Manual Step: Press Scroll Up");
        scroll_state = UP;
        motor_A_Forward(SPEED_F, In2Channel); // Scroll Up
        break;
    case MANUAL_ESTI:
        Serial.println("Manual ESTI: increase esti_move");
        if (esti_index + esti_move < 100)
        {
            ++esti_move;
        }
        break;
    }
}
void buttonRight() {
    switch (stateScreen) {
    case MAIN:
        //TODO: MAIN->MANUAL
        Serial.println("MAIN->MANUAL");
        stateScreen = MANUAL;
        break;
    case MANUAL:
        Serial.println("MANUAL->MAIN");
        stateScreen = MAIN;
        break;
    case MANUAL_STEP:
        Serial.println("MANUAL_STEP->MANUAL");
        stateScreen = MANUAL;
        break;
    case MANUAL_ESTI:
        Serial.println("MANUAL_ESTI->MANUAL");
        stateScreen = MANUAL;
        break;
    case AUTO:
        Serial.printf("AUTO -> MAIN");
        stateScreen = MAIN;
        break;
    }
}

void button_loop(void *pvParameters) {
    selectBtn.loop();
    modeBtn.loop();
    downBtn.loop();
    upBtn.loop();
    rightBtn.loop();
}
