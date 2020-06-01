#include <motor.h>

void initMotor(StepperMotor m){
    pinMode(m->pulse_plus, OUTPUT);
    pinMode(m->pulse_min, OUTPUT);
    pinMode(m->dir_plus, OUTPUT);
    pinMode(m->dir_min, OUTPUT);

    digitalWrite(m->pulse_plus, HIGH);
    digitalWrite(m->dir_plus, HIGH);

    if(m->reversed){
        m->cw = CCW;
        m->ccw = CW;
    } else {
        m->cw = CW;
        m->ccw = CCW;
    }

    m->steps = 0;
    m->dir = m->cw;
}

int reverse_dir(int dir){
    if(dir == CW) dir = CCW;
    if(dir == CCW) dir = CW;
    return dir;
}

void updateTargetAngle(StepperMotor m, double targetAngle){

    // always take the shorter route between current and target angle
    //Angles between 0 and 360 in radians

    // Give a 2 degree space to allow for double imprecision motor.inRange(1)

    if(targetAngle - 1 <  m->currAngle && m->currAngle < targetAngle + 1){
        return;
    }

    // Determine direction
    // Positive angle direction is CCW from 0 to 360 
    
    if (targetAngle >= m->currAngle) {
        m->dir = m->cw;
        // Serial.print("CW");
    } else {
        m->dir = m->ccw;
        // Serial.print("CCW");
    }   
        
    if (targetAngle <= -360) targetAngle = -360;

    if (targetAngle >= 360) targetAngle = fmod(targetAngle, 360);

    //Serial.print(" TargetAngle: "); Serial.print(targetAngle);

    float angleDifference = m->currAngle - targetAngle;

    // Load the difference in angles converted to steps dicretize it by casting to int
    m->steps = (uint16_t)(((abs(angleDifference))
                                    /
                                (ANGLE_PER_FULL_STEP
                                        /
                                (m->step_mode)))                            *2*(m->gearing_ratio));
    //Serial.print("AngleDifference: "); Serial.print(targetAngle); Serial.print(" Current angle: "); Serial.print(m->currAngle); Serial.print(" Steps"); Serial.println(m->steps);
    // Serial.print(" STEPS: "); Serial.print(m->steps);

    // HIGH = 1 CW = 1
    // LOW = 0 CCW = 0
    // set direction

    // if(m->reversed){
    //     m->dir = reverse_dir(m->dir);
    // }
    
    digitalWrite(m->dir_min, m->dir);  
        

}

void updateMotor(StepperMotor m){

    if (m->steps > 0){ // if steps still havent been finished yet
    
        (m->steps)--;

            //Serial.println(m->steps);
            if(m->dir == m->cw){ // update current position values
                if (m->currAngle >= 360) m->currAngle = fmod(m->currAngle, 360);
                m->currAngle += (0.9)/((m->step_mode)*(m->gearing_ratio)); // update the current position remove the corresponding angle per step
            } else {
                if(m->currAngle <= -360) m->currAngle = -360;
                m->currAngle -=  (0.9)/((m->step_mode)*(m->gearing_ratio)); // 1.8 degrees in radians/step 0.0314159
            }
       
        digitalToggle(m->pulse_min);
    }
}

void stopMotor(StepperMotor m){
    m->steps = 0;
}