#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>
#include <kinematics.h>
#include <motor.h>

// motor pins 5 Volt Logic

#define M1_PULP D1
#define M1_PULM D2
#define M1_DIRP D3
#define M1_DIRM D4

#define M2_PULP D6
#define M2_PULM D7
#define M2_DIRM D8
#define M2_DIRP D9

// Joystick

#define SENSITIVITY 50
#define VRx A0
#define VRy A1

// Timers

HardwareTimer timer3(TIM3);
HardwareTimer timer6(TIM6);

extern "C" void timer3_handler(HardwareTimer *);
extern "C" void timer6_handler(HardwareTimer *);

// Assign memory to motor types, in header in case I move the timers to a different file

StepperMotor m1 = (StepperMotor)malloc(sizeof(struct stepper_motor));
StepperMotor m2 = (StepperMotor)malloc(sizeof(struct stepper_motor));

#endif