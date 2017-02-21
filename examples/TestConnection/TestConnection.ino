#include "Arduino.h"
#include "SPI.h"
#include "Streaming.h"
#include "TMC26X.h"

const int BAUDRATE = 115200;
const int LOOP_DELAY = 2000;
const int CS_PIN = 9;

// Instantiate TMC26X
TMC26X stepper_driver;

void setup()
{
  // Setup serial communications
  Serial.begin(BAUDRATE);

  stepper_driver.setup(CS_PIN);

}

void loop()
{
  stepper_driver.setStepDirInput();
  Serial << "setStepDirInput\n";
  stepper_driver.setMicrostepsPerStepTo256();
  Serial << "setMicrostepsPerStepTo256\n";
  stepper_driver.setDefaultChopperConfig();
  Serial << "setDefaultChopperConfig\n";
  stepper_driver.disableCoolStep();
  Serial << "disableCoolStep\n";
  uint8_t current_scale_set = 18;
  double current_scale_actual = stepper_driver.setCurrentScalePercent(current_scale_set);
  Serial << "current_scale_percent set: " << current_scale_set << ", actual: " << current_scale_actual << "\n";

  // TMC26X::Status status = stepper_driver.getStatus();
  // Serial << "status.stall = " << status.stall << "\n";
  // Serial << "status.over_temperature_shutdown = " << status.over_temperature_shutdown << "\n";
  // Serial << "status.over_temperature_warning = " << status.over_temperature_warning << "\n";
  // Serial << "status.short_to_ground_a = " << status.short_to_ground_a << "\n";
  // Serial << "status.short_to_ground_b = " << status.short_to_ground_b << "\n";
  // Serial << "status.open_load_a = " << status.open_load_a << "\n";
  // Serial << "status.open_load_b = " << status.open_load_b << "\n";
  // Serial << "status.standstill = " << status.standstill << "\n";

  Serial << "\n";
  delay(LOOP_DELAY);
}
