#define BUTTON_PIN 13
#define BLACK_TRESHOLD 425
const int SENSORS_PIN[] = { A1, A3, A5, A7 };
const double LINE_SENS_FACTOR[] = { -1.5, -1.3, 1, 1.5 };
bool is_black[4];
int amounts_of_black = 0;

extern double rot_comp;

void init_button() {
  pinMode(BUTTON_PIN, INPUT);
}
bool button_pressed() {
  return (1 - digitalRead(BUTTON_PIN));
}
void wait_start() {
  while (!button_pressed()) {
    down();
    open_claws();
    speed(0);
  }
}

void show_line_sens_data(bool print_end = false) {
  for (int sensor = 0; sensor < 4; sensor++) {
    Serial.print(analogRead(SENSORS_PIN[sensor]) * LINE_SENS_FACTOR[sensor]);
    Serial.print("\t");
  }
  if (print_end) Serial.println();
}
int count_err() {
  static int err;

  err = 0;
  for (int sensor = 0; sensor < 4; sensor++) {
    err += analogRead(SENSORS_PIN[sensor]) * LINE_SENS_FACTOR[sensor];
  }
  return err;
}
void check_black() {
  amounts_of_black = 0;
  for (int sensor = 0; sensor < 4; sensor++) {
    if (analogRead(SENSORS_PIN[sensor]) >= BLACK_TRESHOLD) {
      is_black[sensor] = true;
      amounts_of_black++;
    } else {
      is_black[sensor] = false;
    }
  }
}

void line(int wish_speed) {
  static int err, u;
  static double k = 0.2;

  err = count_err();
  u = err * k;
  rot_comp = u;
  speed(wish_speed);
}
void line_by_dist(int wish_speed, int dist) {
  dist_to_zero();
  while (center < dist) line(wish_speed);
}
void line_by_time(int wish_speed, int wish_time) {
  static uint32_t timer;

  timer = millis();
  while (millis() - timer < wish_time) line(wish_speed);
}
void line_by_angle(int wish_speed, int wish_angle) {
  z_to_zero();

  while (abs(angles[2]) < abs(wish_angle)) line(wish_speed);
}
void line_to_cross(int wish_speed){
  while (amounts_of_black < 3) line(wish_speed);
}
void main_task() {
  //муфта
  line_by_dist(500, 800);
  close_claws();
  line_by_time(500, 100);
  up();
  line_by_angle(500, 175);
  light(0);

  //объезд
  line_by_dist(500, 200);
  light(1);
  z_to_zero();
  while (abs(angles[2]) < 5) {
    rot_comp = -100;
    speed(500);
  }
  while (abs(angles[2]) < 20) {
    rot_comp = -250;
    speed(500);
  }
  dist_to_zero();
  while (center < 275) {
    speed(500);
  }
  z_to_zero();
  while (abs(angles[2]) < 50) {
    rot_comp = 175;
    speed(500);
  }
  dist_to_zero();
  while (center < 275) {
    speed(500);
  }
  z_to_zero();
  while (abs(angles[2]) < 20) {
    rot_comp = -125;
    speed(500);
  }
  light(0);
  line_by_dist(500, 50);

  line_by_angle(500, 170);

  //горка
  line_by_dist(500, 400);
  light(1);
  //line_by_dist(300, 800);
  line_by_time(300, 2600);
  down();
}