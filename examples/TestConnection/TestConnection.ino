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

  // uint32_t version = stepper_driver.getVersion();
  // Serial << "version: " << _HEX(version) << "\n";

  // bool check_version = stepper_driver.checkVersion();
  // Serial << "check_version: " << check_version << "\n";

  // stepper_driver.setStepDirInput();

}

void loop()
{
  stepper_driver.setStepDirInput();

  TMC26X::Status status = stepper_driver.getStatus();
  Serial << "status.stall = " << status.stall << "\n";
  Serial << "status.over_temperature_shutdown = " << status.over_temperature_shutdown << "\n";
  Serial << "status.over_temperature_warning = " << status.over_temperature_warning << "\n";
  Serial << "status.short_to_ground_a = " << status.short_to_ground_a << "\n";
  Serial << "status.short_to_ground_b = " << status.short_to_ground_b << "\n";
  Serial << "status.open_load_a = " << status.open_load_a << "\n";
  Serial << "status.open_load_b = " << status.open_load_b << "\n";
  Serial << "status.standstill = " << status.standstill << "\n";

  // stepper_driver.setMode(MOTOR,TMC26X::VELOCITY_MODE);
  // stepper_driver.setMode(MOTOR,TMC26X::RAMP_MODE);

  // TMC26X::Mode mode = stepper_driver.getMode(MOTOR);
  // if (mode == TMC26X::RAMP_MODE)
  // {
  //   Serial << "mode: ramp_mode\n";
  // }
  // else if (mode == TMC26X::SOFT_MODE)
  // {
  //   Serial << "mode: soft_mode\n";
  // }
  // else if (mode == TMC26X::VELOCITY_MODE)
  // {
  //   Serial << "mode: velocity_mode\n";
  // }
  // else if (mode == TMC26X::HOLD_MODE)
  // {
  //   Serial << "mode: hold_mode\n";
  // }

  // uint32_t velocity_max_upper_limit = stepper_driver.getVelocityMaxUpperLimitInHz();
  // Serial << "velocity_max_upper_limit: " << velocity_max_upper_limit << "\n";

  // uint32_t velocity_min = 100;
  // uint32_t velocity_max = 2000;
  // uint32_t acceleration_max = 20000;
  // stepper_driver.setLimitsInHz(MOTOR,velocity_min,velocity_max,acceleration_max);
  // Serial << "setLimitsInHz: " << velocity_min << ", " << velocity_max << ", " << acceleration_max << "\n";

  // acceleration_max = stepper_driver.getAccelerationMaxInHzPerS(MOTOR);
  // Serial << "acceleration_max: " << acceleration_max << "\n";

  // acceleration_max = stepper_driver.getAccelerationMaxUpperLimitInHzPerS(MOTOR);
  // Serial << "acceleration_max_upper_limit: " << acceleration_max << "\n";

  // uint32_t acceleration_actual = stepper_driver.getAccelerationActualInHzPerS(MOTOR);
  // Serial << "acceleration_actual: " << acceleration_actual << "\n";

  // velocity_min = stepper_driver.getVelocityMinInHz(MOTOR);
  // Serial << "velocity_min: " << velocity_min << "\n";

  // velocity_max = stepper_driver.getVelocityMaxInHz(MOTOR);
  // Serial << "velocity_max: " << velocity_max << "\n";

  // int16_t velocity_target = 321;
  // stepper_driver.setVelocityTargetInHz(MOTOR,velocity_target);
  // Serial << "set velocity_target: " << velocity_target << "\n";

  // velocity_target = stepper_driver.getVelocityTargetInHz(MOTOR);
  // Serial << "velocity_target: " << velocity_target << "\n";

  // int16_t velocity_actual = stepper_driver.getVelocityActualInHz(MOTOR);
  // Serial << "velocity_actual: " << velocity_actual << "\n";

  // int32_t position_target = 12345;
  // stepper_driver.setPositionTarget(MOTOR,position_target);
  // Serial << "set position_target: " << position_target << "\n";

  // position_target = stepper_driver.getPositionTarget(MOTOR);
  // Serial << "position_target: " << position_target << "\n";

  // int32_t position_actual = stepper_driver.getPositionActual(MOTOR);
  // Serial << "position_actual: " << position_actual << "\n";

  // TMC26X::Status status = stepper_driver.getStatus();
  // Serial << "status.at_target_position_0 = " << status.at_target_position_0 << "\n";
  // Serial << "status.switch_left_0 = " << status.switch_left_0 << "\n";
  // Serial << "status.at_target_position_1 = " << status.at_target_position_1 << "\n";
  // Serial << "status.switch_left_1 = " << status.switch_left_1 << "\n";
  // Serial << "status.at_target_position_2 = " << status.at_target_position_2 << "\n";
  // Serial << "status.switch_left_2 = " << status.switch_left_2 << "\n";

  // stepper_driver.disableLeftSwitchStop(MOTOR);
  // stepper_driver.disableRightSwitchStop(MOTOR);
  // stepper_driver.disableSoftStop(MOTOR);
  // stepper_driver.setReferenceSwitchToRight(MOTOR);
  // TMC26X::ReferenceConfiguration ref_conf = stepper_driver.getReferenceConfiguration(MOTOR);
  // Serial << "ref_conf.disable_stop_l: " << ref_conf.disable_stop_l << "\n";
  // Serial << "ref_conf.disable_stop_r: " << ref_conf.disable_stop_r << "\n";
  // Serial << "ref_conf.soft_stop: " << ref_conf.soft_stop << "\n";
  // Serial << "ref_conf.ref_rnl: " << ref_conf.ref_rnl << "\n";

  // bool position_latched = stepper_driver.positionLatched(MOTOR);
  // Serial << "position_latched: " << position_latched << "\n";

  // stepper_driver.setPositionCompareMotor(MOTOR);
  // stepper_driver.enableRightReferences();
  // TMC26X::InterfaceConfiguration if_conf = stepper_driver.getInterfaceConfiguration();
  // Serial << "if_conf.inv_ref: " << if_conf.inv_ref << "\n";
  // Serial << "if_conf.sdo_int: " << if_conf.sdo_int << "\n";
  // Serial << "if_conf.step_half: " << if_conf.step_half << "\n";
  // Serial << "if_conf.inv_stp: " << if_conf.inv_stp << "\n";
  // Serial << "if_conf.inv_dir: " << if_conf.inv_dir << "\n";
  // Serial << "if_conf.en_sd: " << if_conf.en_sd << "\n";
  // Serial << "if_conf.pos_comp_sel: " << if_conf.pos_comp_sel << "\n";
  // Serial << "if_conf.en_refr: " << if_conf.en_refr << "\n";

  // TMC26X::SwitchState switch_state = stepper_driver.getSwitchState();
  // Serial << "switch_state.r0: " << switch_state.r0 << "\n";
  // Serial << "switch_state.l0: " << switch_state.l0 << "\n";
  // Serial << "switch_state.r1: " << switch_state.r1 << "\n";
  // Serial << "switch_state.l1: " << switch_state.l1 << "\n";
  // Serial << "switch_state.r2: " << switch_state.r2 << "\n";
  // Serial << "switch_state.l2: " << switch_state.l2 << "\n";

  // TMC26X::ClockConfiguration clk_config = stepper_driver.getClockConfiguration(MOTOR);
  // Serial << "clk_config.usrs: " << clk_config.usrs << "\n";
  // Serial << "clk_config.ramp_div: " << clk_config.ramp_div << "\n";
  // Serial << "clk_config.pulse_div: " << clk_config.pulse_div << "\n";

  // double proportionality_factor = stepper_driver.getProportionalityFactor(MOTOR);
  // Serial << "proportionality_factor: " << proportionality_factor << "\n";

  // double step_time = stepper_driver.getStepTimeInMicroS();
  // Serial << "step_time: " << step_time << "\n";

  // Serial << "\n";
  delay(LOOP_DELAY);
}
