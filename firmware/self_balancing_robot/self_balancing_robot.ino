#define SERIAL_SPEED 115200
#define Ts 10
#define Ts_s (Ts / 1000.0)

extern void wait(double ms = Ts), show_line_sens_data(bool print_end = false);
extern double angles[3], gyro[3], rot_comp;
extern volatile int stepL, stepR;
extern int distL, distR, center;
extern bool is_black[4];
extern int amounts_of_black;

double spd = 0;

void setup() {
  Serial.begin(SERIAL_SPEED);
  init_button();
  Serial.println("button");
  while (digitalRead(13))
    ;

  init_led();

  if (/*check_battery() <= 14*/ 0) {
    Serial.println("low battery");
    while (1)
      ;
  }

  init_wire();
  //init_vl53l1x();
  Serial.println("bno");
  init_bno();
  init_motors();
  init_servo();
  light(1);

  delay(500);
  update_angle();
  wait();
  while (abs(angles[0]) > 0.5) {
    update_angle();
    wait();
    Serial.println(angles[0]);
  }
  wait_start();

  main_task();

  // down();
  // open_claws();

  // uint32_t timer = millis();
  // while (millis() - timer < 1000) speed(300);
  // timer = millis();
  // close_claws();
  // while (millis() - timer < 100) speed(300);
  // timer = millis();
  // up();
  // while (millis() - timer < 3500) speed(300);
  // light(0);
  // timer = millis();
  // while (spd > 0) speed(-300);
  // put your setup code here, to run once:
}

void loop() {
  static uint32_t timer = millis();
  static double s = 0;
  //speed(0);
  /*update_angle();
  Serial.println(angles[0]);
  wait();*/

  /*Serial.println(get_dist());
  wait();*/
  //static uint32_t timer = millis();
  /*if (millis() % 1000 > 500) {
    motor(500, 500);
    //open_claws();
  } else {
    motor(-500, -500);
    //close_claws();
  }*/
  //wait();

  //balance(-0.78);
  //if (millis() - timer > 2000) rot_comp = 100;

  /*if (millis() - timer > 500) {
    s = double(random(-8, 8)) * 100;
    timer = millis();
  }
  motor(s, s);*/
  //speed(300);
  // show_line_sens_data();
  // Serial.println(count_err());
  //speed(0);
  //Serial.println(get_dist());
  //motor(100, 100);

  // int err, u;
  // err = count_err();
  // u = err * 0.2;
  //rot_comp = u;
  //show_line_sens_data(true);

  // line(500);
  // if (center > 800){
  //   close_claws();
  //   light(0);
  // } if (center > 850){
  //   up();
  // }
  //speed(0);
  //set_angle(0);
  while (millis() - timer < 250) set_angle(0);
  motor(0, 0);

  // show_line_sens_data();
  // for (int i = 0; i < 4; i ++){
  //   Serial.print(is_black[i]);
  //   Serial.print("\t");
  // } Serial.println(amounts_of_black);
  // wait();

  // put your main code here, to run repeatedly:
}
