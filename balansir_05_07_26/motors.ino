#include <TimerOne.h>
#include <TimerThree.h>

#define MOTOR_STEP_L 5
#define MOTOR_DIR_L 4
#define MOTOR_STEP_R 11
#define MOTOR_DIR_R 12

#define RADIUS 350.0 / PI / 2
#define STEPS_PER_ROTATE 200.0
#define DIST_PER_TICK 2 * PI* RADIUS / STEPS_PER_ROTATE
#define STEP_RESOLUTION 16

#define ACCEL 17500 //25000
#define V_MAX 1100.0

volatile int stepL = 0, stepR = 0;
int dirL = 1, dirR = 1, distL, distR, center;

void init_motors() {
  pinMode(MOTOR_DIR_L, OUTPUT);
  pinMode(MOTOR_STEP_L, OUTPUT);
  pinMode(MOTOR_DIR_R, OUTPUT);
  pinMode(MOTOR_STEP_R, OUTPUT);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(ISR_L);
  Timer1.stop();

  Timer3.initialize(1000);
  Timer3.attachInterrupt(ISR_R);
  Timer3.stop();
}

void ISR_L() {
  static bool step_state_L = false;

  digitalWrite(MOTOR_STEP_L, step_state_L);
  digitalWrite(MOTOR_DIR_L, dirL == 1 ? 0 : 1);

  stepL += dirL * step_state_L;

  step_state_L = 1 - step_state_L;
}
void ISR_R() {
  static bool step_state_R = false;

  digitalWrite(MOTOR_STEP_R, step_state_R);
  digitalWrite(MOTOR_DIR_R, dirR == 1 ? 1 : 0);

  stepR += dirR * step_state_R;

  step_state_R = 1 - step_state_R;
}
void count_dist(){
  distL = stepL * DIST_PER_TICK / STEP_RESOLUTION;
  distR = stepR * DIST_PER_TICK / STEP_RESOLUTION;
  center = (distL + distR) / 2;
}

void dist_to_zero() {
  stepL = 0, stepR = 0;
  count_dist();
}

double acceleration(double V_curr, double V_wish, double accel = ACCEL) {
  static double err;
  err = V_wish - V_curr;
  return min(accel * Ts / 1000, abs(err)) * sign(err);
}
int sign(double input) {
  return (input >= 0) ? 1 : -1;
}

void motor(double speedL, double speedR) {
  static double speed_x_last = 0, speed_y_last = 0;
  static double speedL_curr = 0, speedR_curr = 0, periodL = 1, periodR = 1, last_periodL = 1, last_periodR = 1;

  speedL = constrain(speedL, -V_MAX, V_MAX);
  speedR = constrain(speedR, -V_MAX, V_MAX);
  speedL_curr += acceleration(speedL_curr, speedL);
  speedR_curr += acceleration(speedR_curr, speedR);

  /*Serial.print(speedL);
  Serial.print("\t");
  Serial.println(speedL_curr);*/

  static double k_speed = 0.895;
  spd = spd * k_speed + (1 - k_speed) * (speedL_curr + speedR_curr) / 2;

  if (abs(speedL_curr) >= DIST_PER_TICK / STEP_RESOLUTION) {
    dirL = (speedL_curr > 0) ? 1 : -1;

    speedL = abs(speedL_curr);
    periodL = 1000000 * DIST_PER_TICK / speedL / 2 / STEP_RESOLUTION;

    if (last_periodL != periodL) {
      Timer1.setPeriod(periodL);
      Timer1.start();
    }
  } else {
    Timer1.stop();
  }
  last_periodL = periodL;

  if (abs(speedR_curr) >= DIST_PER_TICK / STEP_RESOLUTION) {
    dirR = (speedR_curr > 0) ? 1 : -1;

    speedR = abs(speedR_curr);
    periodR = 1000000 * DIST_PER_TICK / speedR / 2 / STEP_RESOLUTION;

    if (last_periodR != periodR) {
      Timer3.setPeriod(periodR);
      Timer3.start();
    }
  } else {
    Timer3.stop();
  }
  last_periodR = periodR;
}