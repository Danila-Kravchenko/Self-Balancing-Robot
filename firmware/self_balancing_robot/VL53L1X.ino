#include <VL53L1X.h>

VL53L1X sensor;

void init_vl53l1x() {
  sensor.setTimeout(500);
  if (!sensor.init()) {
    Serial.println("Failed to detect and initialize sensor!");
    while (1)
      ;
  }
  // BNO-055 и VL53L1X имеют одинаковый адресс
  // Меняем во избежание конфликтов
  sensor.setAddress(0x28);
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(10000);

  sensor.startContinuous(10);
}
int get_dist() {
  static uint32_t timer = micros();
  static int dist = 0, last_dist = 0, filtr = 0;
  static double k_dist = 0.75;

  dist = sensor.read(false);
  if (sensor.timeoutOccurred()) dist = last_dist;
  filtr = k_dist * filtr + (1 - k_dist) * dist;

  last_dist = filtr;
  return filtr;
}