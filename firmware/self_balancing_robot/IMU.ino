#include <Wire.h>

#define BNO_ADDR 0x29
#define BNO_CHIP_ID_REG 0x00
#define RESET_PIN 24

// angles[0] - pitch (тангаж)
// angles[1] - yaw (рыскание)
// angles[2] - roll (крен)
double gyro[3], angles[3], gyro_angles[3];

void writeReg(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(BNO_ADDR);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}
void readRegs(uint8_t reg, uint8_t *buf, uint8_t len) {
  Wire.beginTransmission(BNO_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)BNO_ADDR, (uint8_t)len);
  for (uint8_t i = 0; i < len; i++) {
    buf[i] = Wire.read();
  }
}
void readBNOData() {
  static uint8_t buffer[6];

  // Гироскоп (X,Y,Z). 1 LSB = 1/16 Dps
  readRegs(0x14, buffer, 6);
  for (int i = 0; i < 3; i++) gyro[i] = (int16_t)((buffer[i * 2 + 1] << 8) | buffer[i * 2]) / 16.0;
  // Эйлеровы углы (H,R,P). 1 LSB = 1/16 Degree
  readRegs(0x1A, buffer, 6);
  for (int i = 0; i < 3; i++) angles[2 - i] = (int16_t)((buffer[i * 2 + 1] << 8) | buffer[i * 2]) / 16.0;
}
void init_wire() {
  Wire.begin();
  //Wire.setClock(400000);  // use 400 kHz I2C
}
void init_bno() {
  // Проверка связи с датчиком, остановит программу если BNO055 не найден
  uint8_t id;
  readRegs(BNO_CHIP_ID_REG, &id, 1);
  if (id != 0xA0) {
    digitalWrite(14, 1);
    Serial.println("Ошибка: BNO055 не найден!");
    while (1)
      ;
  }

  // Настройка датчика
  writeReg(0x3D, 0x00);  // OPR_MODE: CONFIGMODE (нужен для настройки)
  writeReg(0x3F, 0x20);  // SYS_TRIGGER: Сброс системы
  delay(700);            // Время на перезагрузку

  writeReg(0x3B, 0x00);  // UNIT_SEL: m/s2, Dps, Degrees, Celsius (выбор единиц измерения)
  writeReg(0x3D, 0x0C);  // OPR_MODE: NDOF (Sensor Fusion включен)
  delay(100);
}

void wait(double ms = Ts) {
  static uint32_t timer = micros();

  while ((micros() - timer) < ms * 1000){
    count_dist();
    check_black();
  }
  timer = micros();
}

// angles[0] - pitch (тангаж)
// angles[1] - yaw (рыскание)
// angles[2] - roll (крен)
void update_angle() {
  readBNOData();
  for (int angle = 0; angle < 3; angle++) {
    gyro_angles[angle] += gyro[angle] * Ts_s;
  }
  angles[2] = gyro_angles[2];
}
void z_to_zero() {
  gyro_angles[2] = 0;
  angles[2] = 0;
}