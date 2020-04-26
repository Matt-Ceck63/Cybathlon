#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include <kinematics.h>
#include <math.h>

#define CW 1
#define CCW 0
#define PULSE 0
#define NO_PULSE 1
#define ANGLE_PER_FULL_STEP 1.8
#define FULL_STEP 1
#define HALF_STEP 2

struct stepper_motor{
    int pulse_plus;
    int pulse_min;
    int dir_plus;
    int dir_min;

    // If the motor is upside down the clockwise direction is opposite
    bool reversed;

    int step_mode;

    volatile uint16_t steps; //360/(1.8/stepMode)
    int dir;
    volatile double currAngle;

    // Concurrency issues?

};

// Motor identified by its pointer so that we are passing it to the function 
// by reference and modifying it in place without having to return it, we can have it global
typedef struct stepper_motor * StepperMotor; 

void initMotor(StepperMotor m);

// Angle can be positive or negative in degrees, direction is calculated accordingly
void updateTargetAngle(StepperMotor m, double targetAngle);

// runs repeatedly to move the motor to its goal
void updateMotor(StepperMotor m);
void pulseMotor(StepperMotor m);

void stopMotor(StepperMotor m);

// setup externally defined variables
// StepperMotor m1;
// StepperMotor m2;

#endif