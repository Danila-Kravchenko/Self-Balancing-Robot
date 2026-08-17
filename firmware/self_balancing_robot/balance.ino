#define KP_ANGLE 67.5 /*90.5*/
#define KI_ANGLE 800
#define KD_ANGLE 0.495
#define iMax_ANGLE 1000

#define KP_SPEED 0.00795  //0.005
#define KI_SPEED 0.0595  //0.085
#define iMax_SPEED 14    //12
#define MAX_ANGLE 14

double rot_comp = 0;
extern double acceleration(double V_curr, double V_wish, double accel);
extern double spd;

void set_angle(double wish_angle) {
  static double err, u, p, i = 0, d, err_old = 0;

  if (fallen()) {
    motor(0, 0);
    //Serial.println("fallen");
    return;
  }

  update_angle();
  wait();

  err = wish_angle - angles[0];

  p = err * KP_ANGLE;
  d = (err - err_old) * KD_ANGLE / Ts_s;
  err_old = err;
  i += err * KI_ANGLE * Ts_s;
  i = constrain(i, -iMax_ANGLE, iMax_ANGLE);

  u = p + i + d;

  // rot_comp - rotation component
  // создает разность линейных скоростей на моторах, что позволяет роботу двигаться по окружности
  motor(u + rot_comp, u - rot_comp);
  rot_comp = 0;
}
void speed(double wish_speed) {
  static double curr_wish_speed = 0, err, u, p, i = 0;

  curr_wish_speed += acceleration(curr_wish_speed, wish_speed, 400);
  err = (curr_wish_speed - spd);

  p = err * KP_SPEED;
  i += err * Ts_s * KI_SPEED;
  i = constrain(i, -iMax_SPEED, iMax_SPEED);

  u = p + i;
  u = constrain(u, -MAX_ANGLE, MAX_ANGLE);

  set_angle(-0.775 - u);
}
bool fallen() {
  static uint32_t fall_timer;
  if (angles[0] >= -25 && angles[0] <= 75) fall_timer = millis();

  return (fall_timer - millis() > 750) ? 1 : 0;
}