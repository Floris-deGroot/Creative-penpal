//This code can be used to turn the pen stepper in little increments to turn it to a middle position if it has gotten out of line

#include <Stepper.h>
#define STEPS 2038 // the number of steps in one revolution of your motor (28BYJ-48)
Stepper stepper_pen(STEPS, 22,  26, 24, 28);

const int art_button = 50;
const int sig_button = 46;

void setup() {
  Serial.begin(115200);

//  attachInterrupt(digitalPinToInterrupt(pen), lift, FALLING);
}
void loop() {
  if(digitalRead(art_button) == HIGH){
    stepper_pen.setSpeed(10);
    stepper_pen.step(10);
  }
    if(digitalRead(sig_button) == HIGH){
    stepper_pen.setSpeed(10);
    stepper_pen.step(-10);
  }
}
