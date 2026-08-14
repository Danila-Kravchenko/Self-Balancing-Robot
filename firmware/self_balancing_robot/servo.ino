#include <Servo.h>

#define CLAWS_PIN 9
#define CLOSE 20
#define OPEN 70

#define ARM_PIN 10
#define UP 0
#define DOWN 70

Servo arm, claws;

void init_servo(){
  arm.attach(ARM_PIN);
  claws.attach(CLAWS_PIN);

  up();
  open_claws();
}
void up(){
  arm.write(UP);
}
void down(){
  arm.write(DOWN);
}
void open_claws(){
  claws.write(OPEN);
}
void close_claws(){
  claws.write(CLOSE);
}