// ----------------------------------------------------------------------------
// TMC26X.h
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------

#ifndef TMC26X_H
#define TMC26X_H
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "SPI.h"

#include "Streaming.h"


class TMC26X
{
public:
  void setup(const size_t cs_pin);

  void setStepDirInput();
  void setSpiInput();

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
      uint16_t space : 12;
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
      uint16_t space0 : 12;
      Status status;
      uint8_t space1 : 2;
      uint32_t data : 10;
    } fields;
    uint32_t uint32;
  };

  // Union Structs
  union DrvConf
  {
    struct Fields
    {
      uint8_t space0 : 4;
      uint8_t rdsel : 2;
      uint8_t vsense : 1;
      uint8_t sdoff : 1;
      uint8_t ts2g : 2;
      uint8_t diss2g : 1;
      uint8_t space1 : 1;
      uint8_t slpl : 2;
      uint8_t slph : 2;
      uint8_t test : 1;
      uint8_t address : 3;
      uint16_t space3 : 12;
    } fields;
    uint32_t uint32;
  };

  const static uint8_t RDSEL_MICROSTEP = 0b00;
  const static uint8_t RDSEL_SG = 0b01;
  const static uint8_t RDSEL_SGCS = 0b11;

  union DrvContStepDir
  {
    struct Fields
    {
      uint8_t mres : 4;
      uint8_t space : 4;
      uint8_t dedge : 1;
      uint8_t intpol : 1;
      uint8_t space0 : 8;
      uint8_t address : 2;
      uint16_t space1 : 12;
    } fields;
    uint32_t uint32;
  };

  union ChopConf
  {
    struct Fields
    {
      uint8_t toff : 4;
      uint8_t hstrt : 3;
      uint8_t hend : 4;
      uint8_t hdec : 2;
      uint8_t rndtf : 1;
      uint8_t chm : 1;
      uint8_t tbl : 2;
      uint8_t address : 3;
      uint16_t space1 : 12;
    } fields;
    uint32_t uint32;
  };

  size_t cs_pin_;
  Status status_;

  MisoDatagram writeRead(const uint32_t data);

};

#endif
