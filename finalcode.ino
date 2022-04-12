//Stepper libary 
#include <Stepper.h>

// the number of steps in one revolution of the stepper motor (28BYJ-48)
#define STEPS 2038 

// define the three steppers used
Stepper stepper_X_axis(STEPS, 8, 10, 9, 11);
Stepper stepper_Y_axis(STEPS, 2, 4, 3, 5);
Stepper stepper_pen(STEPS, 22,  26, 24, 28);

// buttons
const int pen_button = 18;
const int art_button = 50;
const int sig_button = 46;

// button state between manual controll or generative art 
int manual_or_art_state;

//joystick consisting of two potentiometers (internal button is broken)
int joystick__pot_X = 0;
int joystick__pot_Y = 1;

//state of the pen
int pen_state = 1;

//variables used for generative art
int xposition = 0;
int yposition = 0;
int linestepx = 0;
int linestepy = 0;
int change_pen = 0;
int lengthdiagonal = 0;
int direction1 = 0;
int direction2 = 0;

//start in manual controll mode
int state = 1;

void setup() {
  Serial.begin(115200);
}

void loop() {
  switch (state) {

    case 1:
      Serial.println("manual joystick");
      manual_or_art_state = digitalRead(art_button);
      Serial.print("manual_or_art_state = ");
      Serial.println(manual_or_art_state);
      if (digitalRead(art_button) == HIGH && state == 1) {
        state = 2;
        Serial.println("if1");
        delay(1000);
      }

      if (digitalRead(sig_button) == HIGH && state == 1) {
        state = 3;
        Serial.println("if3");
        delay(1000);
      }
      Serial.println(pen_state);

      if (pen_state == 0 && digitalRead(pen_button) == LOW) {
        //switch pen stepper to next position  
        stepper_pen.setSpeed(5);
        stepper_pen.step(200);
        pen_state = 1;
      }
      
      if (pen_state == 1 && digitalRead(pen_button) == LOW) {
        //switch pen stepper to next position
        stepper_pen.setSpeed(5);
        stepper_pen.step(200);
        pen_state = 2;
      }

      if (pen_state == 2 && digitalRead(pen_button) == LOW) {
        //switch pen stepper to next position
        stepper_pen.setSpeed(5);
        stepper_pen.step(-400);
        pen_state = 0;
      }

        //reads the values from the joystick potentiometers
      joystick__pot_X = analogRead(0); 
      joystick__pot_Y = analogRead(1);


        //all code below controls movement in manual mode
      if (joystick__pot_X > 600) { 
        stepper_X_axis.setSpeed(map(joystick__pot_X, 600, 1023, 1, 10));
        stepper_X_axis.step(-1);
      }

      if (joystick__pot_X < 400) {
        stepper_X_axis.setSpeed(map(joystick__pot_X, 0, 400, 10, 1));
        stepper_X_axis.step(1);
      }

      if (joystick__pot_Y < 400) {
        stepper_Y_axis.setSpeed(map(joystick__pot_Y, 0, 400, 10, 1));
        stepper_Y_axis.step(-1);

      }
      if (joystick__pot_Y > 600) {
        stepper_Y_axis.setSpeed(map(joystick__pot_Y, 600, 1023, 1, 10));
        stepper_Y_axis.step(1);
      }

      break;

    case 2:
      Serial.println("art");
      manual_or_art_state = digitalRead(art_button);
      Serial.print("manual_or_art_state = ");
      Serial.println(manual_or_art_state);
      if (digitalRead(art_button) == HIGH && state == 2) {
        state = 1;
        Serial.println("if2");
        delay(1000);
      }
      Serial.println(pen_state);


      //change_pen is a value for randomly swithing the colors. 
      //In these if statements it checks the current penstate and if it randomly chose the changing value.
      if (pen_state == 1 && change_pen == 1) {
        stepper_pen.setSpeed(5);
        stepper_pen.step(200);
        pen_state = 2;
      }

      if (pen_state == 2 && change_pen == 2) {
        stepper_pen.setSpeed(5);
        stepper_pen.step(-400);  
        pen_state = 1;
      }
        // art mode should be started roughly in the middle of the canvas
        // this if statement ensures that the X and Y steppers dont move to far which would result in too much stress on the motors or the assembly falling apart 
      if ( -2300 < xposition < 2300 && -2300 < yposition < 2300) {
        
        //random X stepper
        stepper_X_axis.setSpeed(10);
        linestepx = random(-300, 300);
        Serial.print("distance in x-axis =");
        Serial.println(linestepx);
        stepper_X_axis.step(linestepx);
        xposition = xposition + linestepx;
        Serial.print("x position =");
        Serial.println(xposition);

        //random Y stepper
        stepper_Y_axis.setSpeed(10);
        linestepy = random(-300, 300);
        Serial.print("distance in y-axis =");
        Serial.println(linestepy);
        stepper_Y_axis.step(linestepy);
        yposition = yposition + linestepy;
        Serial.print("y position =");
        Serial.println(yposition);

        //random diagonal lines
        lengthdiagonal = random(0,400);
        direction1 = random(1,3);
        direction2 = random(1,3);
        
        for (int i = 0; i <= lengthdiagonal; i++) {
        stepper_X_axis.setSpeed(10);
        if(direction1 == 1){
        stepper_X_axis.step(-1);
         }
         else{stepper_X_axis.step(1);
         }
         stepper_Y_axis.setSpeed(10);
         if(direction2 == 1){
         stepper_Y_axis.step(1);
          }
          else{stepper_Y_axis.step(-1);
         }
         }

        
        //bigger value of change_pen makes switching less often
        change_pen = random(2);


      }
      break;

    case 3:
      Serial.println("signature");

      stepper_X_axis.setSpeed(10);
      stepper_X_axis.step(400);
      
        // These are the instructions for the robots own signature ("mondrio")
      for (int i = 0; i <= 200; i++) {
        stepper_X_axis.setSpeed(10);
        stepper_X_axis.step(-1);
        stepper_Y_axis.setSpeed(10);
        stepper_Y_axis.step(1);
      }
      for (int j = 0; j <= 200; j++) {
        stepper_X_axis.setSpeed(10);
        stepper_X_axis.step(1);
        stepper_Y_axis.setSpeed(10);
        stepper_Y_axis.step(1);
      }
      stepper_X_axis.setSpeed(10);
      stepper_X_axis.step(-400);

      stepper_Y_axis.setSpeed(10);
      stepper_Y_axis.step(200);

      stepper_X_axis.setSpeed(10);
      stepper_X_axis.step(250);

      stepper_Y_axis.setSpeed(10);
      stepper_Y_axis.step(-200);

      stepper_Y_axis.setSpeed(10);
      stepper_Y_axis.step(200);

      for (int k = 0; k <= 250; k++) {
        stepper_X_axis.setSpeed(10);
        stepper_X_axis.step(-1);
        stepper_Y_axis.setSpeed(10);
        stepper_Y_axis.step(1);
      }

      stepper_X_axis.setSpeed(10);
      stepper_X_axis.step(250);

      stepper_Y_axis.setSpeed(10);
      stepper_Y_axis.step(200);

      stepper_X_axis.setSpeed(10);
      stepper_X_axis.step(150);

      stepper_X_axis.setSpeed(10);
      stepper_X_axis.step(-400);

      stepper_Y_axis.setSpeed(10);
      stepper_Y_axis.step(-200);

      stepper_Y_axis.setSpeed(10);
      stepper_Y_axis.step(200);

      stepper_X_axis.setSpeed(10);
      stepper_X_axis.step(200);

      for (int l = 0; l <= 50; l++) {
        stepper_X_axis.setSpeed(10);
        stepper_X_axis.step(1);
        stepper_Y_axis.setSpeed(10);
        stepper_Y_axis.step(1);
      }

      stepper_Y_axis.setSpeed(10);
      stepper_Y_axis.step(150);

      //switch pen for the io in mondrio
      stepper_pen.setSpeed(10);
      stepper_pen.step(-400);

      stepper_X_axis.setSpeed(10);
      stepper_X_axis.step(-250);

      stepper_Y_axis.setSpeed(10);
      stepper_Y_axis.step(600);

      stepper_X_axis.setSpeed(10);
      stepper_X_axis.step(400);

      stepper_Y_axis.setSpeed(10);
      stepper_Y_axis.step(-600);

      stepper_X_axis.setSpeed(10);
      stepper_X_axis.step(-150);

      stepper_X_axis.setSpeed(10);
      stepper_X_axis.step(150);

      stepper_Y_axis.setSpeed(10);
      stepper_Y_axis.step(200);

      stepper_X_axis.setSpeed(10);
      stepper_X_axis.step(-400);

      //lift the pen up
      stepper_pen.setSpeed(10);
      stepper_pen.step(400);

      //return to manual control mode
      state = 1;
  }
}
