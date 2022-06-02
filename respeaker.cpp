#include "respeaker.h"
#include <Arduino.h>

#include <Wire.h>

ReSpeaker::ReSpeaker() {
    Wire.begin();

    // Data init
    memset(databuf, 0, sizeof(databuf));
    count = 0;
}

ReSpeaker::~ReSpeaker() {
}


void ReSpeaker::sendI2C(uint32_t sz) {
      Wire.beginTransmission(addr);   // Slave address
      Wire.write(databuf,sz);
      Wire.endTransmission();         // Transmit to Slave
}

uint32_t ReSpeaker::readI2C() {
    uint32_t res = 0;
    Wire.requestFrom(addr, (size_t)1); // Read from Slave (string len unknown, request full buffer)

    res = Wire.available();
    databuf[0] = Wire.read();
    return res;
}

bool ReSpeaker::i2cOK() {
  databuf[0] = 0;
  sendI2C(1);
  if (readI2C() > 0) {
    if (databuf[0] == 0x4A) {
      return true;
    }
  }
  return false;
}

bool ReSpeaker::captureStarted() {
  databuf[0] = 0x30;
  sendI2C(1);
  if (readI2C() > 0) {
    if (databuf[0] == 0xF5) {
      return true;
    }
  }
  return false;
}

void ReSpeaker::initBuffer(uint8_t reg, uint8_t val) {
  databuf[0] = reg;
  databuf[1] = val;
}

void ReSpeaker::initBoard() {
  initBuffer(0x00, 0x12); sendI2C(2);//reset registers
  delay(200);
  initBuffer(0x20, 0x08); sendI2C(2);//sys clock 24000000 Hz
  initBuffer(0x30, 0x35); sendI2C(2);//i2s format
  initBuffer(0x30, 0xb5); sendI2C(2);//i2s format
  initBuffer(0x06, 0x01); sendI2C(2);//power up ac108
  initBuffer(0x07, 0x99); sendI2C(2);//power up ac108
  initBuffer(0x09, 0x09); sendI2C(2);//power up ac108
  initBuffer(0x32, 0x10); sendI2C(2);//i2s config LRCK_POLARITY
  initBuffer(0x34, 0x0d); sendI2C(2);//PCM_FORMAT
  initBuffer(0x36, 0x00); sendI2C(2);//MSB first
  initBuffer(0x66, 0x00); sendI2C(2);//disable high pass filter
  initBuffer(0x61, 0x0f); sendI2C(2);//enable adc
  initBuffer(0xa0, 0x01); sendI2C(2);//disable mic bias 1
  initBuffer(0xa7, 0x01); sendI2C(2);//disable mic bias 2
  initBuffer(0xae, 0x01); sendI2C(2);//disable mic bias 3
  initBuffer(0xb5, 0x01); sendI2C(2);//disable mic bias 4
  initBuffer(0xa0, 0x05); sendI2C(2);//disable dsm 1
  initBuffer(0xa7, 0x05); sendI2C(2);//disable dsm 2
  initBuffer(0xae, 0x05); sendI2C(2);//disable dsm 3
  initBuffer(0xb5, 0x05); sendI2C(2);//disable dsm 4
  initBuffer(0x61, 0x1f); sendI2C(2);//disable adc
  initBuffer(0xa0, 0x07); sendI2C(2);//enable PGA 1
  initBuffer(0xa7, 0x07); sendI2C(2);//enable PGA 2
  initBuffer(0xae, 0x07); sendI2C(2);//enable PGA 3
  initBuffer(0xb5, 0x07); sendI2C(2);//enable PGA 4
  initBuffer(0xba, 0x20); sendI2C(2);//disable dsm globally
  initBuffer(0xbb, 0x0f); sendI2C(2);//disable adc clk gating globally
  setVolume(0xde);
}

void ReSpeaker::setVolume(uint8_t vol) {
  initBuffer(0x70, vol); sendI2C(2);//volume mic 1
  initBuffer(0x71, vol); sendI2C(2);//volume mic 2
  initBuffer(0x72, vol); sendI2C(2);//volume mic 3
  initBuffer(0x73, vol); sendI2C(2);//volume mic 4
}

void ReSpeaker::startCapture() {
  initBuffer(0x33, 0x7f); sendI2C(2);//sample resolution 32 bit
  initBuffer(0x35, 0x77); sendI2C(2);//sample resolution 32 bit
  initBuffer(0x60, 0x03); sendI2C(2);//sample rate 16000 Hz
  initBuffer(0x66, 0x0f); sendI2C(2);//enable high pass filter
  
  initBuffer(0x14, 0x18); sendI2C(2);//pll divider config
  initBuffer(0x13, 0x80); sendI2C(2);//pll loop divider
  initBuffer(0x12, 0x00); sendI2C(2);//pll loop divider MSB
  initBuffer(0x11, 0x04); sendI2C(2);//pll pre divider
  initBuffer(0x18, 0x01); sendI2C(2);//pll clock lock en
  initBuffer(0x20, 0x89); sendI2C(2);//pll clock source MCLK

  initBuffer(0x31, 0x06); sendI2C(2);//bclk div

  initBuffer(0x38, 0x03); sendI2C(2);//set channels
  initBuffer(0x39, 0x0f); sendI2C(2);//tx config
  initBuffer(0x3a, 0x00); sendI2C(2);//tx config
  initBuffer(0x3c, 0x4e); sendI2C(2);//tx chip map
  initBuffer(0x3d, 0x00); sendI2C(2);//tx chip map
  initBuffer(0x3e, 0x00); sendI2C(2);//tx chip map
  initBuffer(0x3f, 0x00); sendI2C(2);//tx chip map
  
  initBuffer(0x21, 0x93); sendI2C(2);//mod clk en
  initBuffer(0x22, 0x93); sendI2C(2);//mod rst ctrl

  initBuffer(0x30, 0xb0); sendI2C(2);//disable all clocks
  initBuffer(0x30, 0xf0); sendI2C(2);//enable lrck clock
  initBuffer(0x10, 0x4b); sendI2C(2);//enable pll
  initBuffer(0x30, 0xf5); sendI2C(2);//enable global clock
}


void ReSpeaker::startCapture22() {
  initBuffer(0x33, 0x3f); sendI2C(2);//sample resolution 16 bit
  initBuffer(0x35, 0x33); sendI2C(2);//sample resolution 16 bit
  initBuffer(0x60, 0x04); sendI2C(2);//sample rate 22050 Hz
  initBuffer(0x66, 0x0f); sendI2C(2);//enable high pass filter

  initBuffer(0x14, 0x18); sendI2C(2);//pll divider config
  initBuffer(0x13, 0x4c); sendI2C(2);//pll loop divider
  initBuffer(0x12, 0x02); sendI2C(2);//pll loop divider MSB
  initBuffer(0x11, 0x18); sendI2C(2);//pll pre divider
  initBuffer(0x18, 0x01); sendI2C(2);//pll clock lock en
  initBuffer(0x20, 0x89); sendI2C(2);//pll clock source MCLK

  initBuffer(0x31, 0x05); sendI2C(2);//bclk div

  initBuffer(0x38, 0x03); sendI2C(2);//set channels
  initBuffer(0x39, 0x0f); sendI2C(2);//tx config
  initBuffer(0x3a, 0x00); sendI2C(2);//tx config
  initBuffer(0x3c, 0x4e); sendI2C(2);//tx chip map
  initBuffer(0x3d, 0x00); sendI2C(2);//tx chip map
  initBuffer(0x3e, 0x00); sendI2C(2);//tx chip map
  initBuffer(0x3f, 0x00); sendI2C(2);//tx chip map
  
  initBuffer(0x21, 0x93); sendI2C(2);//mod clk en
  initBuffer(0x22, 0x93); sendI2C(2);//mod rst ctrl

  initBuffer(0x30, 0xb0); sendI2C(2);//disable all clocks
  initBuffer(0x30, 0xf0); sendI2C(2);//enable lrck clock
  initBuffer(0x10, 0x4b); sendI2C(2);//enable pll
  initBuffer(0x30, 0xf5); sendI2C(2);//enable global clock
}


void ReSpeaker::startCapture44() {
  initBuffer(0x33, 0x3f); sendI2C(2);//sample resolution 16 bit
  initBuffer(0x35, 0x33); sendI2C(2);//sample resolution 16 bit
  initBuffer(0x60, 0x07); sendI2C(2);//sample rate 44100 Hz
  initBuffer(0x66, 0x0f); sendI2C(2);//enable high pass filter
  
  initBuffer(0x14, 0x18); sendI2C(2);//pll divider config
  initBuffer(0x13, 0x4c); sendI2C(2);//pll loop divider
  initBuffer(0x12, 0x02); sendI2C(2);//pll loop divider MSB
  initBuffer(0x11, 0x18); sendI2C(2);//pll pre divider
  initBuffer(0x18, 0x01); sendI2C(2);//pll clock lock en
  initBuffer(0x20, 0x89); sendI2C(2);//pll clock source MCLK

  initBuffer(0x31, 0x03); sendI2C(2);//bclk div

  initBuffer(0x38, 0x03); sendI2C(2);//set channels
  initBuffer(0x39, 0x0f); sendI2C(2);//tx config
  initBuffer(0x3a, 0x00); sendI2C(2);//tx config
  initBuffer(0x3c, 0x4e); sendI2C(2);//tx chip map
  initBuffer(0x3d, 0x00); sendI2C(2);//tx chip map
  initBuffer(0x3e, 0x00); sendI2C(2);//tx chip map
  initBuffer(0x3f, 0x00); sendI2C(2);//tx chip map
  
  initBuffer(0x21, 0x93); sendI2C(2);//mod clk en
  initBuffer(0x22, 0x93); sendI2C(2);//mod rst ctrl

  initBuffer(0x30, 0xb0); sendI2C(2);//disable all clocks
  initBuffer(0x30, 0xf0); sendI2C(2);//enable lrck clock
  initBuffer(0x10, 0x4b); sendI2C(2);//enable pll
  initBuffer(0x30, 0xf5); sendI2C(2);//enable global clock
}

void ReSpeaker::stopCapture() {
  initBuffer(0x30, 0xf0); sendI2C(2);//disable global clock
  initBuffer(0x10, 0x48); sendI2C(2);//disable pll
  initBuffer(0x30, 0xb0); sendI2C(2);//disable all clocks
  initBuffer(0x21, 0x00); sendI2C(2);//disable mod clk
  initBuffer(0x22, 0x00); sendI2C(2);//disable mod rst
}
