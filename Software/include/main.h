#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>
#include <kinematics.h>
#include <motor.h>

// motor pins 5 Volt Logic

#define M1_PULP D1
#define M1_PULM D2 //PA_12
#define M1_DIRP D3
#define M1_DIRM D4

#define M2_PULP D6
#define M2_PULM D7 //PF_0
#define M2_DIRP D8
#define M2_DIRM D9

// Joystick

#define SENSITIVITY 50
#define VRx A0
#define VRy A1
#define BUTTON D10

//States
#define CONTROL_KINEMATICS 0
#define CONTROL_JOINTS 1

// Timers

HardwareTimer timer3(TIM3);
HardwareTimer timer2(TIM2);

extern "C" void timer3_handler(HardwareTimer *);
extern "C" void timer2_handler(HardwareTimer *);

// Assign memory to motor types, in header in case I move the timers to a different file

#endif