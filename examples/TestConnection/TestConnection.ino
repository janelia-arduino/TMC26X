#include <Arduino.h>
#include <SPI.h>
#include <Streaming.h>
#include <TMC26X.h>


const long BAUD = 115200;
const int LOOP_DELAY = 2000;
const int CS_PIN = 9;

// Instantiate TMC26X
TMC26X stepper_driver;

void setup()
{
  // Setup serial communications
  Serial.begin(BAUD);

  stepper_driver.setup(CS_PIN);

}

void loop()
{
  stepper_driver.setMicrostepsPerStepPowerOfTwo(8);
  Serial << "setMicrostepsPerStepToPowerOfTwo(8)\n";
  uint8_t current_percent = 18;
  stepper_driver.setRunCurrent(current_percent);

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
