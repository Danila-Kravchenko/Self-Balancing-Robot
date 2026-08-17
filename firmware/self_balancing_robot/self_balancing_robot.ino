#define SERIAL_SPEED 115200
#define Ts 10
#define Ts_s (Ts / 1000.0)

extern void wait(double ms = Ts);
extern double angles[3];

void setup() {
  Serial.begin(SERIAL_SPEED);
  init_button();
  while (!button_pressed())
    ;

  init_led();

  if (check_battery() <= 14) {
    Serial.println("low battery");
    while (1)
      ;
  }

  init_wire();
  init_vl53l1x();
  init_bno();
  init_motors();
  init_servo();
  light(1);
  Serial.println("ready");

  // Установка робота в вертикальное положение
  delay(500);
  update_angle();
  wait();
  while (abs(angles[0]) > 0.5) {
    update_angle();
    wait();
    Serial.println(angles[0]);
  }

  // Основной алгоритм перемещения
  wait_start();
  main_task();
}

void loop() {
  static uint32_t timer = millis();
  
  while (millis() - timer < 250) set_angle(0);
  motor(0, 0);
}
