// #include <Arduino.h>
// #define CW HIGH
// #define CCW LOW
// #define del 1000

// #define M1_PULP D1
// #define M1_PULM D2
// #define M1_DIRP D3
// #define M1_DIRM D4

// int dir = CW;

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(9600);
//   pinMode(M1_PULP, OUTPUT);
//   pinMode(M1_PULM, OUTPUT);
//   pinMode(M1_DIRP, OUTPUT);
//   pinMode(M1_DIRM, OUTPUT);
//   digitalWrite(M1_PULP, HIGH);
//   digitalWrite(M1_DIRP, HIGH);
//   digitalWrite(M1_DIRM, dir);

// }

// char in[2];

// bool moving = true;
// bool new_char = false;

// void loop() {
//   if(Serial.available() > 0){
//     Serial.readBytes(in, 2);
//     Serial.println(in[0]);
//     new_char = true;
//   }

//   if(in[0] == 'a' && new_char) {moving = !moving;}

//   if(in[0] == 'r' && new_char){
//     if(dir == HIGH){ dir = LOW;}
//     else { dir = HIGH;} //dir = 1 - dir ;
//     digitalWrite(M1_DIRM, dir);
//   }
  
//   if(moving){
//       digitalWrite(M1_PULM, HIGH);
//       delayMicroseconds(del);
//       digitalWrite(M1_PULM, LOW);
//       delayMicroseconds(del);
//   }

//   new_char = false;
// }