// ----------------------------------------------------------------------------
// TMC26X.cpp
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------

#include "TMC26X.h"


void TMC26X::setup(const size_t cs_pin)
{
  cs_pin_ = cs_pin;

  pinMode(cs_pin_,OUTPUT);
  digitalWrite(cs_pin_,HIGH);

  SPI.begin();
}

void TMC26X::setStepDirInput()
{
  DrvConf drv_conf;
  drv_conf.uint32 = 0;
  drv_conf.fields.rdsel = RDSEL_MICROSTEP;
  drv_conf.fields.vsense = 1;
  drv_conf.fields.sdoff = 0;
  drv_conf.fields.ts2g = 0;
  drv_conf.fields.diss2g = 0;
  drv_conf.fields.slpl = 0b11;
  drv_conf.fields.slph = 0b11;
  drv_conf.fields.test = 0;
  drv_conf.fields.address = ADDRESS_DRVCONF;
  Serial << "drv_conf: " << _HEX(drv_conf.uint32) << "\n";
  // writeRead(drv_conf.uint32);
}

void TMC26X::setSpiInput()
{
}

// private
TMC26X::MisoDatagram TMC26X::writeRead(const uint32_t data)
{
  MosiDatagram datagram_write;
  datagram_write.uint32 = 0;
  datagram_write.fields.data = data;
  MisoDatagram datagram_read;
  datagram_read.uint32 = 0;
  SPI.beginTransaction(SPISettings(SPI_CLOCK,SPI_BIT_ORDER,SPI_MODE));
  digitalWrite(cs_pin_,LOW);
  for (int i=(sizeof(datagram_write) - 1); i>=0; --i)
  {
    uint8_t byte_write = (datagram_write.uint32 >> (8*i)) & 0xff;
    uint8_t byte_read = SPI.transfer(byte_write);
    datagram_read.uint32 |= byte_read << (8*i);
  }
  digitalWrite(cs_pin_,HIGH);
  SPI.endTransaction();
  noInterrupts();
  status_ = datagram_read.fields.status;
  interrupts();
  return datagram_read;
}
