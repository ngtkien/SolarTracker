#pragma once


//LCD pin to Arduino
constexpr auto pin_RS = 22;
constexpr auto pin_EN = 23;
constexpr auto pin_d4 = 5;
constexpr auto pin_d5 = 18;
constexpr auto pin_d6 = 19;
constexpr auto pin_d7 = 21;
// L298N pin to Arduino
constexpr auto pin_IN1 = 13;
constexpr auto pin_IN2 = 12;
constexpr auto pin_IN3 = 14;
constexpr auto pin_IN4 = 27;

//New Button  SEL: GPIO39, MODE: GPIO36, DOWN: GPIO35, UP: GPIO34, OK: GPIO25
// constexpr auto pin_SELECT = 39;
// constexpr auto pin_MODE = 36;
// constexpr auto pin_DOWN = 35;
// constexpr auto pin_UP = 34;
// constexpr auto pin_RIGHT = 25;

constexpr auto pin_SELECT = 39;
constexpr auto pin_MODE = 25;
constexpr auto pin_DOWN = 34;
constexpr auto pin_UP = 35;
constexpr auto pin_RIGHT = 36;
constexpr auto MAX_SPEED = 4096;
constexpr auto MIN_SPEED = 0;
constexpr auto SPEED = 1900;
constexpr auto SPEED_F = 2000;

// Setting PWM properties

#define I2C_SDA 33
#define I2C_SCL 32
enum Cursor {
    LINE1,
    LINE2
};
enum Scroll_State
{
    STOP,
    DOWN,
    UP

};
enum ScreenLcd {
    MAIN = 1,
    MANUAL,
    MANUAL_STEP,
    MANUAL_ESTI,
    AUTO
};
