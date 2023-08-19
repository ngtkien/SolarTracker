// 
// 
// 
#include <Arduino.h>
#include "LcdKeyShield.h"

LCDKeypad::LCDKeypad() : LiquidCrystal(KEYPAD_LCD_PINS) {
	keypadInit(KEYPAD_BACKLIGHT_PIN, KEYPAD_BUTTON_ANALOG_PIN);
}
void LCDKeypad::keypadInit(uint8_t bl_pin, uint8_t button_apin)
{
	_bl_pin = bl_pin;
	_button_pin = button_apin;
	_block_time = 0;
	_last_button = KEYPAD_NONE;

	// set button pin as input
	pinMode(_button_pin, INPUT);

	// turn on backlight properly just to be sure we are not frying outputs
	backlight();
}

uint8_t LCDKeypad::button()
{
	// read the value from the sensor
	long adc_sum = 0;
	for(int i = 0; i < 50; i++)
	{
		adc_sum += analogRead(_button_pin);
	}
	
	long adc_key_in = adc_sum / 50;
	//int adc_key_in = analogRead(_button_pin);
	Serial.println(adc_key_in);
	if (adc_key_in > KEYPAD_TRESHOLD_NONE) return KEYPAD_NONE; // We make this the 1st option for speed reasons since it will be the most likely result
	if (adc_key_in < KEYPAD_TRESHOLD_RIGHT)   return KEYPAD_RIGHT; // 0 on my board
	if (adc_key_in < KEYPAD_TRESHOLD_UP && adc_key_in > KEYPAD_TRESHOLD_RIGHT)  return KEYPAD_UP; // 133 on my board
	if (adc_key_in < KEYPAD_TRESHOLD_DOWN && adc_key_in > KEYPAD_TRESHOLD_UP)  return KEYPAD_DOWN; // 308 on my board
	if (adc_key_in < KEYPAD_TRESHOLD_LEFT && adc_key_in > KEYPAD_TRESHOLD_DOWN)  return KEYPAD_LEFT; // 480 on my board
	if (adc_key_in < KEYPAD_TRESHOLD_SELECT && adc_key_in > KEYPAD_TRESHOLD_LEFT)  return KEYPAD_SELECT;  // 721 on my board
	return KEYPAD_NONE;  // when all others fail, return this...
}

uint8_t LCDKeypad::buttonBlocking(uint16_t block_delay, uint16_t repeat_delay)
{
	// if we are still blocked, return this status
	if (millis() < _block_time)
		return KEYPAD_BLOCKED;

	uint8_t current = button();

	// if some key is pressed, disable presses for block_delay or repeat_delay if button is kept down
	if (current != KEYPAD_NONE)
		_block_time = millis() + (current == _last_button ? repeat_delay : block_delay);

	_last_button = current;
	return current;
}
void LCDKeypad::backlight()
{
	// always try to keep the pin low
	digitalWrite(_bl_pin, LOW);
	pinMode(_bl_pin, INPUT);
}

void LCDKeypad::noBacklight()
{
	// always try to keep the pin low
	digitalWrite(_bl_pin, LOW);
	pinMode(_bl_pin, OUTPUT);
}