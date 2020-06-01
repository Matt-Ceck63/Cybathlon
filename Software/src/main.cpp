#include <main.h>

// ========= MOTOR SETUP =========

StepperMotor m1 = (StepperMotor)malloc(sizeof(struct stepper_motor));
StepperMotor m2 = (StepperMotor)malloc(sizeof(struct stepper_motor));

void init_motor1(){
  m1->pulse_plus = M1_PULP;
  m1->pulse_min = M1_PULM;
  m1->dir_plus = M1_DIRP;
  m1->dir_min = M1_DIRM;
  m1->reversed = true;
  m1->currAngle = 0;
  m1->step_mode = FULL_STEP;
  m1->gearing_ratio = 100;

  initMotor(m1);
}

void init_motor2(){
  m2->pulse_plus = M2_PULP;
  m2->pulse_min = M2_PULM;
  m2->dir_plus = M2_DIRP;
  m2->dir_min = M2_DIRM;
  m2->reversed = true;
  m2->gearing_ratio = 1;
  m2->step_mode = THIRTYTWO_STEP;
  m2->currAngle = 0;

  initMotor(m2);
}

// ========= TIMER SETUP =========
//EXTERN C compiles using c compiler not needed for this but needed for cubeMX IRQ_Handler
// timer3.setPrescaleFactor(32000); //Divides maximum clock by the prescaler CLOCK = 64Mhz
// 16-bit up to 65536 period = (clock/prescaler) * time(in seconds)
// TICK_FORMAT = value of the counter
// MICROSEC AND HERTZ doesn't need prescaler
// HERTZ FORMAT cannot take floating point numbers

void timer3_setup(){
  timer3.pause();
  timer3.setMode(1, TIMER_OUTPUT_COMPARE);
  timer3.setOverflow(2000, HERTZ_FORMAT); 
  timer3.attachInterrupt(timer3_handler);
  NVIC_EnableIRQ(TIM3_IRQn); //IT WAS THIS!!!
  // timer3.refresh();
}

void timer2_setup(){
  timer2.pause();
  timer2.setMode(1, TIMER_OUTPUT_COMPARE);
  timer2.setOverflow(2000, HERTZ_FORMAT); 
  timer2.attachInterrupt(timer2_handler);
  NVIC_EnableIRQ(TIM2_IRQn); //IT WAS THIS!!!
  // timer6.refresh();
}

extern "C" void timer3_handler(HardwareTimer *){ 
  //digitalToggle(LED_BUILTIN);
  //digitalToggle(M1_PULM);
  updateMotor(m1);
}

extern "C" void timer2_handler(HardwareTimer *){ 
  // digitalToggle(LED_BUILTIN);
  updateMotor(m2);
}

// ========= TASK 1 : JOYSTICK CONTROL =========

void init_joystick(){
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  analogReadResolution(12);
}

coordinates joystick_coords = {0,0};
double new_x, new_y, delta_x, delta_y;
coordinates end_effector_coords = {0, MAX_LENGTH - 10};
angles motor_angles = {0, 0};

void joystick_kinematic_task(){

  // Serial.println("Joystick started");

  /*
  * Takes the analog values of the x and y potentiometer on the joystick and converts them in 
  * end effector target x, y positions. The joystick position represents a delta x, y that will
  * be added or subtracted to the current x, y positions.
  */
  joystick_coords.x = end_effector_coords.x;
  joystick_coords.y = end_effector_coords.y;

  new_x = analogRead(VRx);
  new_y = analogRead(VRy);

  delta_x = map(new_x, 0, 4096, -MAX_LENGTH, MAX_LENGTH)/SENSITIVITY;
  delta_y = map(new_y, 0, 4096, MAX_LENGTH, -MAX_LENGTH)/SENSITIVITY; // Joystick has reversed y
  if (new_x > 1900 && new_x < 2100) delta_x = 0;
  if (new_y > 1900 && new_y < 2100) delta_y = 0;
  joystick_coords.x += delta_x;
  joystick_coords.y += delta_y;

  // TODO: Make limits based on workspace.
  if (joystick_coords.x > MAX_LENGTH) joystick_coords.x = MAX_LENGTH;
  if (joystick_coords.x < -MAX_LENGTH) joystick_coords.x = - MAX_LENGTH;
  if (joystick_coords.y > MAX_LENGTH) joystick_coords.y = MAX_LENGTH;
  if (joystick_coords.y < -MAX_LENGTH) joystick_coords.y = - MAX_LENGTH;

  end_effector_coords.x = joystick_coords.x;
  end_effector_coords.y = joystick_coords.y;

  Serial.print("x: "); Serial.print(joystick_coords.x); Serial.print("y: "); Serial.print(joystick_coords.y); 
  Serial.print(" || Vx :"); Serial.print(new_x); Serial.print(" Vy: "); Serial.print(new_y);
  Serial.print(" || M1 :"); Serial.print(m1->currAngle); Serial.print(" M2: "); Serial.print(m2->currAngle);

}

void joystick_joint_task(){

  joystick_coords.x = motor_angles.q1;
  joystick_coords.y = motor_angles.q2;

  new_x = analogRead(VRx);
  new_y = analogRead(VRy);

  delta_x = map(new_x, 0, 4096, -180, 180)/SENSITIVITY;
  delta_y = map(new_y, 0, 4096, 180, -180)/SENSITIVITY; // Joystick has reversed y
  if (new_x > 1900 && new_x < 2100) delta_x = 0; //currPos.inRange()
  if (new_y > 1900 && new_y < 2100) delta_y = 0; //currPos.inRange(optional min, max, axis)

  joystick_coords.x += delta_x;
  joystick_coords.y += delta_y;

  // TODO: Make limits based on workspace.
  if (joystick_coords.x > 180) joystick_coords.x = 180;
  if (joystick_coords.x < -180) joystick_coords.x = -180;
  if (joystick_coords.y > 180) joystick_coords.y = 180;
  if (joystick_coords.y < -180) joystick_coords.y = -180;

  motor_angles.q1 = joystick_coords.x;
  motor_angles.q2 = joystick_coords.y;

  Serial.print("M1: "); Serial.print(joystick_coords.x); Serial.print(" M2: "); Serial.print(joystick_coords.y); 
  Serial.print(" || Vx :"); Serial.print(new_x); Serial.print(" Vy: "); Serial.print(new_y);
  Serial.print(" || M1 :"); Serial.print(m1->currAngle); Serial.print(" M2: "); Serial.println(m2->currAngle);

}

// ========= TASK 2: CALCULATE KINEMATICS =========


void kinematics_task(){ 
  angles targetAngle = inverse_kinematics_planar2R(end_effector_coords);
  Serial.print(" || q1: "); Serial.print((targetAngle.q1)); Serial.print(" q2: "); Serial.println((targetAngle.q2));
  updateTargetAngle(m1, (targetAngle.q1));
  // Serial.println(m1->currAngle);
  updateTargetAngle(m2, (targetAngle.q2));
}

void joint_task(){
  updateTargetAngle(m1, motor_angles.q1);
  updateTargetAngle(m2, motor_angles.q2);
}

// ====== BUTTON =======

int count = 0;
int control_state = CONTROL_JOINTS;

void control_button(int targetState){

  if (digitalRead(BUTTON) == HIGH) {
    count = 0;
    return;
  } 

  Serial.print("BUTTON PRESSED");
  count++;
  if (count == 12){
    Serial.print("STATE CHANGED");
    count = 0;
    control_state = targetState;

    if (control_state == CONTROL_KINEMATICS){

      Serial.print("GOING TO KINEMATICS CONTROL");

      end_effector_coords = forward_kinematics_planar2R(motor_angles);

    } else if(control_state == CONTROL_JOINTS){
      Serial.print("GOING TO JOINT CONTROL");

      motor_angles = inverse_kinematics_planar2R(end_effector_coords);

      
    }
  }
}

// ========= MAIN =========
void setup(){
  Serial.begin(9600);

  // pinMode(LED_BUILTIN, OUTPUT);
  timer3_setup();
  timer2_setup();
  init_motor1();
  init_motor2();
  init_joystick();

  // Resume timers after motor setup
  timer3.resume();
  timer2.resume();

}

// void joystick_task2(){

//   /*
//   * Takes the analog values of the x and y potentiometer on the joystick and converts them in 
//   * end effector target x, y positions. The joystick position represents a delta x, y that will
//   * be added or subtracted to the current x, y positions.
//   */

//   //Serial.println("Joystick started");

//   new_x = analogRead(VRx);
//   delta_x = map(new_x, 0, 4096, -360, 360)/SENSITIVITY;
//   if (new_x > 1900 && new_x < 2100) delta_x = 0;
//   x += delta_x;

//   // TODO: Make limits based on workspace.
//   if (x > 360) x = 360;
//   if (x < -360) x = -360;

//   Serial.print("x: "); Serial.print(x); 
//   Serial.print(" || Vx :"); Serial.print(analogRead(VRx));
// }

void loop(){

  switch (control_state)
  {
  case CONTROL_KINEMATICS:

    control_button(CONTROL_JOINTS);
    joystick_kinematic_task();
    kinematics_task();
    
    break;

  case CONTROL_JOINTS:

    control_button(CONTROL_KINEMATICS);
    joystick_joint_task();
    joint_task();
    break;
  
  }

  delay(100);

}

  // #ifdef CCW
  // // DIR: HIGH - LOW = CW
  // // DIR: LOW - HIGH = CCW
  // digitalWrite(M1_DIRM, HIGH);

  // #else

  // digitalWrite(M1_DIRM, LOW);

  // #endif

  // // M1 IS REVERSED
  // //digitalWrite(M2_PULP, HIGH);
  // //digitalWrite(M2_PULM, LOW);

  // // DIR: HIGH - LOW = CCW
  // #ifdef CCW
  // digitalWrite(M2_DIRM, LOW);
  // #endif
  // // DIR: LOW - HIGH = CW
  // #ifdef CW
  // digitalWrite(M2_DIRM, HIGH);
  // #endif

// extern "C" void timer_setup(){
//   RCC->APB1ENR |= 0x00000002;
//   TIM3->PSC = 320000;         // Set prescaler to 24 000 (PSC + 1)
//   TIM3->ARR = 65500;           // Auto reload value 1000
//   TIM3->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)
//   TIM3->CR1 = 0x0001;   //enable timer
//   NVIC->ISER[0] |= 0x20000000;

// }