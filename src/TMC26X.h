// ----------------------------------------------------------------------------
// TMC26X.h
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef TMC26X_H
#define TMC26X_H
#include <Arduino.h>
#include <SPI.h>


class TMC26X
{
public:
  void setup(size_t cs_pin);
  void setup(size_t cs_pin,
    size_t enable_pin);

  // bool communicating();
  // uint8_t getVersion();

  void initialize();

  void enable();
  void disable();

  // valid values = 1,2,4,8,...128,256, other values get rounded down
  void setMicrostepsPerStep(size_t microsteps_per_step);
  size_t getMicrostepsPerStep();

  void setRunCurrent(uint8_t percent);

  struct Status
  {
    uint8_t stall : 1;
    uint8_t over_temperature_shutdown : 1;
    uint8_t over_temperature_warning : 1;
    uint8_t short_to_ground_a : 1;
    uint8_t short_to_ground_b : 1;
    uint8_t open_load_a : 1;
    uint8_t open_load_b : 1;
    uint8_t standstill : 1;
  };
  Status getStatus();

private:
  // SPISettings
  const static uint32_t SPI_CLOCK = 1000000;
  const static uint8_t SPI_BIT_ORDER = MSBFIRST;
  const static uint8_t SPI_MODE = SPI_MODE3;

  // MOSI Datagrams
  union MosiDatagram
  {
    struct Fields
    {
      uint32_t data : 20;
      uint32_t space : 12;
    } fields;
    uint32_t uint32;
  };

  // Addresses
  const static uint8_t ADDRESS_DRVCTRL = 0b00;
  const static uint8_t ADDRESS_CHOPCONF = 0b100;
  const static uint8_t ADDRESS_SMARTEN = 0b101;
  const static uint8_t ADDRESS_SGCSCONF = 0b110;
  const static uint8_t ADDRESS_DRVCONF = 0b111;

  // MISO Datagrams
  union MisoDatagram
  {
    struct Fields
    {
      uint32_t space0 : 12;
      Status status;
      uint32_t space1 : 2;
      uint32_t data : 10;
    } fields;
    uint32_t uint32;
  };

  // Union Structs
  union DrvConf
  {
    struct Fields
    {
      uint32_t reserved0 : 4;
      uint32_t rdsel : 2;
      uint32_t vsense : 1;
      uint32_t sdoff : 1;
      uint32_t ts2g : 2;
      uint32_t diss2g : 1;
      uint32_t reserved1 : 1;
      uint32_t slpl : 2;
      uint32_t slph : 2;
      uint32_t test : 1;
      uint32_t address : 3;
      uint32_t space0 : 12;
    } fields;
    uint32_t uint32;
  };

  const static uint8_t RDSEL_MICROSTEP = 0b00;
  const static uint8_t RDSEL_SG = 0b01;
  const static uint8_t RDSEL_SGCS = 0b11;
  const static uint8_t VSENSE_BEST_CURRENT_REG = 0b0;
  const static uint8_t VSENSE_LOW_SENSE_POWER = 0b1;
  const static uint8_t SDOFF_STEP_DIR = 0b0;
  const static uint8_t SDOFF_SPI = 0b1;
  const static uint8_t TS2G_3200NS = 0b00;
  const static uint8_t TS2G_1600NS = 0b01;
  const static uint8_t TS2G_1200NS = 0b10;
  const static uint8_t TS2G_0800NS = 0b11;
  const static uint8_t DISS2G_ENABLE_SHORT_PROTECTION = 0b0;
  const static uint8_t DISS2G_DISABLE_SHORT_PROTECTION = 0b1;
  const static uint8_t SLP_MINIMUM = 0b00;
  const static uint8_t SLP_MINIMUM_TEMP = 0b01;
  const static uint8_t SLP_MEDIUM = 0b10;
  const static uint8_t SLP_MAXIMUM = 0b11;

  union DrvContStepDir
  {
    struct Fields
    {
      uint32_t mres : 4;
      uint32_t reserved0 : 4;
      uint32_t dedge : 1;
      uint32_t intpol : 1;
      uint32_t reserved1 : 8;
      uint32_t address : 2;
      uint32_t space0 : 12;
    } fields;
    uint32_t uint32;
  };

  const static uint8_t MRES_256 = 0b0000;
  const static uint8_t MRES_128 = 0b0001;
  const static uint8_t MRES_064 = 0b0010;
  const static uint8_t MRES_032 = 0b0011;
  const static uint8_t MRES_016 = 0b0100;
  const static uint8_t MRES_008 = 0b0101;
  const static uint8_t MRES_004 = 0b0110;
  const static uint8_t MRES_002 = 0b0111;
  const static uint8_t MRES_001 = 0b1000;
  const static uint8_t DEDGE_RISING = 0b0;
  const static uint8_t DEDGE_BOTH = 0b1;
  const static uint8_t INTPOL_DISABLE_INTERPOLATION = 0b0;
  const static uint8_t INTPOL_ENABLE_INTERPOLATION = 0b1;

  union DrvContSpi
  {
    struct Fields
    {
      uint32_t cb : 8;
      uint32_t phb : 1;
      uint32_t ca : 8;
      uint32_t pha : 1;
      uint32_t address : 2;
      uint32_t space0 : 12;
    } fields;
    uint32_t uint32;
  };

  union ChopConf
  {
    struct Fields
    {
      uint32_t toff : 4;
      uint32_t hstrt : 3;
      uint32_t hend : 4;
      uint32_t hdec : 2;
      uint32_t rndtf : 1;
      uint32_t chm : 1;
      uint32_t tbl : 2;
      uint32_t address : 3;
      uint32_t space0 : 12;
    } fields;
    uint32_t uint32;
  };

  union SmartEn
  {
    struct Fields
    {
      uint32_t semin : 4;
      uint32_t reserved0 : 1;
      uint32_t seup : 2;
      uint32_t reserved1 : 1;
      uint32_t semax : 4;
      uint32_t reserved2 : 1;
      uint32_t sedn : 2;
      uint32_t seimin : 1;
      uint32_t reserved3 : 1;
      uint32_t address : 3;
      uint32_t space0 : 12;
    } fields;
    uint32_t uint32;
  };

  const static uint8_t SEMIN_DISABLED = 0b0000;
  const static uint8_t SEUP_1 = 0b00;
  const static uint8_t SEUP_2 = 0b01;
  const static uint8_t SEUP_4 = 0b10;
  const static uint8_t SEUP_8 = 0b11;
  const static uint8_t SEDN_32 = 0b00;
  const static uint8_t SEDN_08 = 0b01;
  const static uint8_t SEDN_02 = 0b10;
  const static uint8_t SEDN_01 = 0b11;
  const static uint8_t SEIMIN_HALF = 0b0;
  const static uint8_t SEIMIN_QUARTER = 0b1;

  union SgcsConf
  {
    struct Fields
    {
      uint32_t cs : 5;
      uint32_t reserved0 : 3;
      uint32_t sgt : 7;
      uint32_t reserved1 : 1;
      uint32_t sfilt : 1;
      uint32_t address : 3;
      uint32_t space0 : 12;
    } fields;
    uint32_t uint32;
  };

  const static uint8_t CS_REGISTER_MIN = 0b00000;
  const static uint8_t CS_REGISTER_MAX = 0b11111;
  const static int8_t SGT_REGISTER_MIN = -10;
  const static int8_t SGT_REGISTER_MAX = 63;
  const static int8_t SGT_DEFAULT = 0b0000101;
  const static uint8_t SFILT_STANDARD_MODE = 0b0;
  const static uint8_t SFILT_FILTERED_MODE = 0b1;

  const static uint8_t PERCENT_MIN = 0;
  const static uint8_t PERCENT_MAX = 100;

  size_t cs_pin_;
  int enable_pin_;

  const static uint8_t MICROSTEPS_PER_STEP_EXPONENT_MAX = 8;
  uint8_t microsteps_per_step_exponent_;

  Status status_;

  void setEnablePin(size_t enable_pin);

  void setStepDirInput();
  // void setSpiInput();

  // microsteps = 2^exponent, 0=1,1=2,2=4,...8=256
  void setMicrostepsPerStepPowerOfTwo(uint8_t exponent);

  MisoDatagram writeRead(uint32_t data);

  void configDriver(uint8_t rdsel,
    uint8_t vsense,
    uint8_t sdoff,
    uint8_t ts2g,
    uint8_t diss2g,
    uint8_t slpl,
    uint8_t slph);
  void setDriverControlStepDir(uint8_t mres,
    uint8_t dedge,
    uint8_t intpol);
  void setDefaultChopperConfig();
  void configChopper(uint8_t toff,
    uint8_t hstrt,
    uint8_t hend,
    uint8_t hdec,
    uint8_t rndtf,
    uint8_t chm,
    uint8_t tbl);
  void disableCoolStep();
  // void enableCoolStep();
  void setCoolStepRegister(uint8_t semin,
    uint8_t seup,
    uint8_t semax,
    uint8_t sedn,
    uint8_t seimin);
  uint8_t percentToCurrentSetting(uint8_t percent);
  void setStallGuardRegister(uint8_t cs,
    int8_t sgt,
    uint8_t sfilt);

};

#endif
