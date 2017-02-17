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

  DrvContStepDir drv_cont;
  drv_cont.uint32 = 0;
  drv_cont.fields.mres = 0;
  drv_cont.fields.dedge = 0;
  drv_cont.fields.intpol = 0;
  drv_cont.fields.address = ADDRESS_DRVCTRL;
  Serial << "drv_cont: " << _HEX(drv_cont.uint32) << "\n";

  ChopConf chop_conf;
  chop_conf.uint32 = 0;
  chop_conf.fields.toff = 1;
  chop_conf.fields.hstrt = 0b011;
  chop_conf.fields.hend = 0b0010;
  chop_conf.fields.hdec = 0b00;
  chop_conf.fields.rndtf = 0;
  chop_conf.fields.chm = 0;
  chop_conf.fields.tbl = 0b10;
  chop_conf.fields.address = ADDRESS_CHOPCONF;
  Serial << "chop_conf: " << _HEX(chop_conf.uint32) << "\n";
}

void TMC26X::setSpiInput()
{
}

TMC26X::Status TMC26X::getStatus()
{
  return status_;
}

// private
TMC26X::MisoDatagram TMC26X::writeRead(const uint32_t data)
{
  MosiDatagram datagram_write;
  datagram_write.uint32 = 0;
  datagram_write.fields.data = data;
  Serial << "mosi datagram: " << _HEX(datagram_write.uint32) << "\n";
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
  Serial << "miso datagram: " << _HEX(datagram_read.uint32) << "\n";
  return datagram_read;
}
