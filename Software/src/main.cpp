#include <main.h>

// ========= MOTOR SETUP =========

void init_motor1(){
  m1->pulse_plus = M1_PULP;
  m1->pulse_min = M1_PULM;
  m1->dir_plus = M1_DIRP;
  m1->dir_min = M1_DIRM;
  m1->reversed = true;
  m1->currAngle = 0;

  initMotor(m1);
}

void init_motor2(){
  m2->pulse_plus = M2_PULP;
  m2->pulse_min = M2_PULM;
  m2->dir_plus = M2_DIRP;
  m2->dir_min = M2_DIRM;
  m2->reversed = false;
  m2->currAngle = PI;

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
  timer3.refresh();
}

void timer6_setup(){
  timer6.pause();
  timer6.setMode(1, TIMER_OUTPUT_COMPARE);
  timer6.setOverflow(2000, HERTZ_FORMAT); 
  timer6.attachInterrupt(timer6_handler);
  NVIC_EnableIRQ(TIM6_IRQn); //IT WAS THIS!!!
  timer6.refresh();
}

extern "C" void timer3_handler(HardwareTimer *){ 
  //digitalToggle(LED_BUILTIN);
  //digitalToggle(M1_PULM);
  updateMotor(m1);
}

extern "C" void timer6_handler(HardwareTimer *){ 
  updateMotor(m2);
}

// ========= TASK 1 : JOYSTICK CONTROL =========

void init_joystick(){
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  analogReadResolution(12);
}

double x = 0, y = MAX_LENGTH - 100;
double new_x, new_y, delta_x, delta_y;
int count = 0;

void joystick_task(){

  /*
  * Takes the analog values of the x and y potentiometer on the joystick and converts them in 
  * end effector target x, y positions. The joystick position represents a delta x, y that will
  * be added or subtracted to the current x, y positions.
  */

  new_x = analogRead(VRx);
  new_y = analogRead(VRy);
  delta_x = map(new_x, 0, 4096, -MAX_LENGTH, MAX_LENGTH)/SENSITIVITY;
  delta_y = map(new_y, 0, 4096, MAX_LENGTH, -MAX_LENGTH)/SENSITIVITY; // Joystick has reversed y
  if (new_x > 1900 && new_x < 2100) delta_x = 0;
  if (new_y > 1900 && new_y < 2100) delta_y = 0;
  x += delta_x;
  y += delta_y;

  // TODO: Make limits based on workspace.
  if (x > MAX_LENGTH) x = MAX_LENGTH;
  if (x < -MAX_LENGTH) x = - MAX_LENGTH;
  if (y > MAX_LENGTH) y = MAX_LENGTH;
  if (y < -MAX_LENGTH) y = - MAX_LENGTH;

  Serial.print(x); Serial.print(" "); Serial.print(y), Serial.print(" "); 
  Serial.print(analogRead(VRx)); Serial.print(" "); Serial.println(analogRead(VRy));
}

// ========= TASK 2: CALCULATE KINEMATICS =========

angles targetAngle;
coordinates currPos;

void kinematics_task(){
  currPos.x = x;
  currPos.y = y;
  targetAngle = inverse_kinematics_planar2R(currPos);
  updateTargetAngle(m1, targetAngle.q1);
  updateTargetAngle(m2, targetAngle.q2);
}


// ========= MAIN =========
void setup(){
  Serial.begin(9600);
  //pinMode(LED_BUILTIN, OUTPUT);
  timer3_setup();
  timer6_setup();
  init_motor1();
  init_motor2();
  init_joystick();

  // Resume timers after motor setup
  timer3.resume();
  timer6.resume();
}

void loop(){
  
  joystick_task();
  kinematics_task();
  delay(200);
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