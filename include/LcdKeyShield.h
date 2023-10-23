// LcdKeyShield.h

#ifndef _LCDKEYSHIELD_h
#define _LCDKEYSHIELD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <inttypes.h>
#include <LiquidCrystal.h>

#define KEYPAD_NONE 0
#define KEYPAD_RIGHT 1
#define KEYPAD_UP 2
#define KEYPAD_DOWN 3
#define KEYPAD_LEFT 4
#define KEYPAD_SELECT 5
#define KEYPAD_BLOCKED 255

#define KEYPAD_BUTTON_ANALOG_PIN 4
#define KEYPAD_BACKLIGHT_PIN 34
//const int pin_RS = 22;
//const int pin_EN = 23;
//const int pin_d4 = 5;
//const int pin_d5 = 18;
//const int pin_d6 = 19;
//const int pin_d7 = 21;
#define KEYPAD_LCD_PINS 22, 23, 5, 18, 19, 21

// you can change tresholds for detecting key presses

enum KeyPad {
   KEYPAD_TRESHOLD_NONE = 2800,
   KEYPAD_TRESHOLD_SELECT = 1950,
   KEYPAD_TRESHOLD_LEFT = 1300,
   KEYPAD_TRESHOLD_DOWN = 800,
   KEYPAD_TRESHOLD_UP = 300,
   KEYPAD_TRESHOLD_RIGHT = 100,

} ;



class LCDKeypad : public LiquidCrystal
{
public:
    LCDKeypad();
    LCDKeypad(uint8_t bl_pin, uint8_t button_apin);

    void keypadInit(uint8_t bl_pin, uint8_t button_apin);


    uint8_t button();

    // use this method for menus and other things where repeated and filtered keypresses are needed
    uint8_t buttonBlocking(uint16_t block_delay = 500, uint16_t repeat_delay = 300);

    // some lcd boards have issue with backlight frying output pins
    // see http://forum.arduino.cc//index.php?topic=96747 for discussion and solutions
    // this code uses universal solution with pin input/output toggling, so it should be
    // safe on all types of this board
    void backlight();
    void noBacklight();

private:
    uint8_t _bl_pin;
    uint8_t _button_pin;

    uint32_t _block_time;
    uint8_t _last_button;
};

#endif

