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
  enable_pin_ = -1;

  pinMode(cs_pin_,OUTPUT);
  digitalWrite(cs_pin_,HIGH);

  SPI.begin();
}

void TMC26X::setup(const size_t cs_pin,
                   const size_t enable_pin)
{
  setup(cs_pin);
  setEnablePin(enable_pin);
}

// bool TMC26X::communicating()
// {
//   return (getVersion() == VERSION);
// }

// uint8_t TMC26X::getVersion()
// {
// }

void TMC26X::initialize()
{
  setStepDirInput();
  setDefaultChopperConfig();
}

void TMC26X::enable()
{
  if (enable_pin_ >= 0)
  {
    digitalWrite(enable_pin_,LOW);
  }
}

void TMC26X::disable()
{
  if (enable_pin_ >= 0)
  {
    digitalWrite(enable_pin_,HIGH);
  }
}

void TMC26X::setMicrostepsPerStep(const size_t microsteps_per_step)
{
  size_t microsteps_per_step_shifted = constrain(microsteps_per_step,
                                                 MICROSTEPS_PER_STEP_MIN,
                                                 MICROSTEPS_PER_STEP_MAX);
  microsteps_per_step_shifted = microsteps_per_step >> 1;
  size_t exponent = 0;
  while (microsteps_per_step_shifted > 0)
  {
    microsteps_per_step_shifted = microsteps_per_step_shifted >> 1;
    ++exponent;
  }
  setMicrostepsPerStepPowerOfTwo(exponent);
}

size_t TMC26X::getMicrostepsPerStep()
{
  return 1 << microsteps_per_step_exponent_;
}

void TMC26X::setRunCurrent(const uint8_t percent)
{
  uint8_t run_current = percentToCurrentSetting(percent);
  setStallGuardRegister(run_current,
                        SGT_DEFAULT,
                        SFILT_FILTERED_MODE);
}

TMC26X::Status TMC26X::getStatus()
{
  return status_;
}

// private
void TMC26X::setEnablePin(const size_t enable_pin)
{
  enable_pin_ = enable_pin;

  pinMode(enable_pin_,OUTPUT);
  disable();
}

void TMC26X::setStepDirInput()
{
  configDriver(RDSEL_MICROSTEP,
               VSENSE_LOW_SENSE_POWER,
               SDOFF_STEP_DIR,
               TS2G_3200NS,
               DISS2G_ENABLE_SHORT_PROTECTION,
               SLP_MAXIMUM,
               SLP_MAXIMUM);
}

// void TMC26X::setSpiInput()
// {
//   configDriver(RDSEL_MICROSTEP,
//                VSENSE_LOW_SENSE_POWER,
//                SDOFF_STEP_DIR,
//                TS2G_3200NS,
//                DISS2G_ENABLE_SHORT_PROTECTION,
//                SLP_MAXIMUM,
//                SLP_MAXIMUM);
// }

void TMC26X::setMicrostepsPerStepPowerOfTwo(const uint8_t exponent)
{
  microsteps_per_step_exponent_ = exponent;

  switch (exponent)
  {
    case 0:
    {
      setDriverControlStepDir(MRES_001,
                              DEDGE_RISING,
                              INTPOL_DISABLE_INTERPOLATION);
      break;
    }
    case 1:
    {
      setDriverControlStepDir(MRES_002,
                              DEDGE_RISING,
                              INTPOL_DISABLE_INTERPOLATION);
      break;
    }
    case 2:
    {
      setDriverControlStepDir(MRES_004,
                              DEDGE_RISING,
                              INTPOL_DISABLE_INTERPOLATION);
      break;
    }
    case 3:
    {
      setDriverControlStepDir(MRES_008,
                              DEDGE_RISING,
                              INTPOL_DISABLE_INTERPOLATION);
      break;
    }
    case 4:
    {
      setDriverControlStepDir(MRES_016,
                              DEDGE_RISING,
                              INTPOL_DISABLE_INTERPOLATION);
      break;
    }
    case 5:
    {
      setDriverControlStepDir(MRES_032,
                              DEDGE_RISING,
                              INTPOL_DISABLE_INTERPOLATION);
      break;
    }
    case 6:
    {
      setDriverControlStepDir(MRES_064,
                              DEDGE_RISING,
                              INTPOL_DISABLE_INTERPOLATION);
      break;
    }
    case 7:
    {
      setDriverControlStepDir(MRES_128,
                              DEDGE_RISING,
                              INTPOL_DISABLE_INTERPOLATION);
      break;
    }
    case 8:
    default:
    {
      microsteps_per_step_exponent_ = MICROSTEPS_PER_STEP_EXPONENT_MAX;
      setDriverControlStepDir(MRES_256,
                              DEDGE_RISING,
                              INTPOL_DISABLE_INTERPOLATION);
      break;
    }
  }
  setChopperConfig();
}

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

void TMC26X::configDriver(const uint8_t rdsel,
                          const uint8_t vsense,
                          const uint8_t sdoff,
                          const uint8_t ts2g,
                          const uint8_t diss2g,
                          const uint8_t slpl,
                          const uint8_t slph)
{
  DrvConf drv_conf;
  drv_conf.uint32 = 0;
  drv_conf.fields.rdsel = rdsel;
  drv_conf.fields.vsense = vsense;
  drv_conf.fields.sdoff = sdoff;
  drv_conf.fields.ts2g = ts2g;
  drv_conf.fields.diss2g = diss2g;
  drv_conf.fields.slpl = slpl;
  drv_conf.fields.slph = slph;
  drv_conf.fields.test = 0;
  drv_conf.fields.address = ADDRESS_DRVCONF;
  writeRead(drv_conf.uint32);
}

void TMC26X::setDriverControlStepDir(const uint8_t mres,
                                     const uint8_t dedge,
                                     const uint8_t intpol)
{
  DrvContStepDir drv_cont;
  drv_cont.uint32 = 0;
  drv_cont.fields.mres = mres;
  drv_cont.fields.dedge = dedge;
  drv_cont.fields.intpol = intpol;
  drv_cont.fields.address = ADDRESS_DRVCTRL;
  writeRead(drv_cont.uint32);
}

void TMC26X::setDefaultChopperConfig()
{
  configChopper(1,
                0b011,
                0b0010,
                0b00,
                0,
                0,
                0b10);
}

void TMC26X::configChopper(const uint8_t toff,
                           const uint8_t hstrt,
                           const uint8_t hend,
                           const uint8_t hdec,
                           const uint8_t rndtf,
                           const uint8_t chm,
                           const uint8_t tbl)
{
  ChopConf chop_conf;
  chop_conf.uint32 = 0;
  chop_conf.fields.toff = toff;
  chop_conf.fields.hstrt = hstrt;
  chop_conf.fields.hend = hend;
  chop_conf.fields.hdec = hdec;
  chop_conf.fields.rndtf = rndtf;
  chop_conf.fields.chm = chm;
  chop_conf.fields.tbl = tbl;
  chop_conf.fields.address = ADDRESS_CHOPCONF;
  writeRead(chop_conf.uint32);
}

void TMC26X::disableCoolStep()
{
  setCoolStepRegister(SEMIN_DISABLED,
                      SEUP_1,
                      0b00,
                      SEDN_32,
                      SEIMIN_HALF);
}

// void TMC26X::enableCoolStep()
// {
//   setCoolStepRegister(SEMIN_DISABLED,
//                              SEUP_1,
//                              0b00,
//                              SEDN_32,
//                              SEIMIN_HALF);
// }

void TMC26X::setCoolStepRegister(const uint8_t semin,
                                 const uint8_t seup,
                                 const uint8_t semax,
                                 const uint8_t sedn,
                                 const uint8_t seimin)
{
  SmartEn smart_en;
  smart_en.uint32 = 0;
  smart_en.fields.semin = semin;
  smart_en.fields.seup = seup;
  smart_en.fields.semax = semax;
  smart_en.fields.sedn = sedn;
  smart_en.fields.seimin = seimin;
  smart_en.fields.address = ADDRESS_SMARTEN;
  writeRead(smart_en.uint32);
}

uint8_t TMC26X::percentToCurrentSetting(uint8_t percent)
{
  uint8_t current_percent = constrain(percent,PERCENT_MIN,PERCENT_MAX);
  uint8_t current_setting = map(current_percent,PERCENT_MIN,PERCENT_MAX,CS_REGISTER_MIN,CS_REGISTER_MAX);
  return current_setting;
}

void TMC26X::setStallGuardRegister(const uint8_t cs,
                                   const int8_t sgt,
                                   const uint8_t sfilt)
{
  SgcsConf sgcs_conf;
  sgcs_conf.uint32 = 0;
  sgcs_conf.fields.cs = cs;
  int8_t sgt_thresholded = sgt;
  if (sgt_thresholded > SGT_REGISTER_MAX)
  {
    sgt_thresholded = SGT_REGISTER_MAX;
  }
  if (sgt_thresholded < SGT_REGISTER_MIN)
  {
    sgt_thresholded = SGT_REGISTER_MIN;
  }
  sgcs_conf.fields.sgt = sgt_thresholded;
  sgcs_conf.fields.sfilt = sfilt;
  sgcs_conf.fields.address = ADDRESS_SGCSCONF;
  writeRead(sgcs_conf.uint32);
}
