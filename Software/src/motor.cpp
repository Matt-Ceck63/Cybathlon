#include <motor.h>

void initMotor(StepperMotor m){
    pinMode(m->pulse_plus, OUTPUT);
    pinMode(m->pulse_min, OUTPUT);
    pinMode(m->dir_plus, OUTPUT);
    pinMode(m->dir_min, OUTPUT);

    digitalWrite(m->pulse_plus, HIGH);
    digitalWrite(m->dir_plus, HIGH);

    m->step_mode = FULL_STEP;
    m->steps = 0;
    m->dir = CW;
}

void updateTargetAngle(StepperMotor m, double targetAngle){

    // always take the shorter route between current and target angle
    //Angles between 0 and 360

    if (targetAngle < 0) targetAngle = 2 * PI + targetAngle;

    float angleDifference = m->currAngle - targetAngle;

    // Determine direction
    // Positive angle direction is CCW from 0 to 360 
    
    m->dir = (angleDifference > 0) ? CW : CCW;

    // Load the difference in angles converted to steps dicretize it by casting to int
    m->steps = (uint16_t)((abs(angleDifference))/(ANGLE_PER_FULL_STEP/(m->step_mode)));

    if(m->reversed) {
        if(m->dir == CW) m->dir = CCW;
        if(m->dir == CCW) m->dir = CW;
    }

    // HIGH = 1 CW = 1
    // LOW = 0 CCW = 0
    // set direction
    digitalWrite(m->dir_min, m->dir);
}

void updateMotor(StepperMotor m){

    if (m->steps > 0){ // if steps still havent been finished yet
        (m->steps)--;
        switch(m->dir){ // update current position values
            case CW:
                m->currAngle -= (0.0314159)/(m->step_mode); // update the current position remove the corresponding angle per step
                if(m->currAngle < 0) m->currAngle = 2 * PI + (m->currAngle); // wrap around 0 degrees, avoid negative angles
                break;
            case CCW:
                m->currAngle +=  (0.0314159)/(m->step_mode); // 1.8 degrees in radians/step
                if(m-> currAngle > 2 * PI) m->currAngle = m->currAngle - 2 * PI; //wrap around 360 degrees, avoid negative angles
                break;
        }
        
        pulseMotor(m); // go one step

    }


}

void pulseMotor(StepperMotor m){
    digitalToggle(m->pulse_min);
}

void stopMotor(StepperMotor m){
    m->steps = 0;
}