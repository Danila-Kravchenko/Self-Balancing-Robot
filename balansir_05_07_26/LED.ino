#define ACTION_LED_PIN 17
#define BATTERY_LED_PIN 15

#define VOLTAGE_PIN A9
#define R2 333.0
#define R1 1000.0

void init_led() {
  pinMode(ACTION_LED_PIN, OUTPUT);
  pinMode(BATTERY_LED_PIN, OUTPUT);
}

void light(bool state){
  digitalWrite(ACTION_LED_PIN, state);
}
double check_battery(){
  double voltage = double(analogRead(VOLTAGE_PIN)) * (R1 + R2) * 5.0 / R2 / 1023.0;
  Serial.println(voltage);
  Serial.println(double(analogRead(VOLTAGE_PIN)) * 5 / 1023);

  int amounts = 0;
  if (voltage >= 16) amounts = 1;
  else if (voltage >= 15) amounts = 2;
  else amounts = 3;

  for (; amounts > 0; amounts --){
    digitalWrite(BATTERY_LED_PIN, 1);
    delay(150);
    digitalWrite(BATTERY_LED_PIN, 0);
    delay(150);
  }

  return voltage;
}