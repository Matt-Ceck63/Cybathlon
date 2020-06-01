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
#define QUARTER_STEP 4
#define THIRTYTWO_STEP 32
#define GEAR_MULTIPLIER 100

struct stepper_motor{

    int pulse_plus;
    int pulse_min;
    int dir_plus;
    int dir_min;

    // Gearing ratio 1 if 1:1 ratio, 100 id 100:1 output
    // allows the angle to be controlled based on the output gear

    int gearing_ratio;

    // If the motor is upside down the clockwise direction is opposite
    bool reversed;

    // Set the appropriate pin values for CW or CCW depending on the motor
    int cw;
    int ccw;

    // Current direction of motor
    int dir;

    // Stepper motor step mode
    int step_mode;

    // Variables that are modified in the timer handlers
    uint16_t steps; //360/(1.8/stepMode)
    double currAngle;

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