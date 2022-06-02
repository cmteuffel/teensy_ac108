#ifndef RESPEAKER_H
#define RESPEAKER_H

#include <stdint.h>


#define MEM_LEN 3

class ReSpeaker {
private:
  uint8_t databuf[MEM_LEN];
  int count;
  void sendI2C(uint32_t sz);
  void initBuffer(uint8_t reg, uint8_t val);
  uint32_t readI2C();
public:
  ReSpeaker();
  ~ReSpeaker();
  void initBoard();
  bool i2cOK();
  bool captureStarted();
  void startCapture();
  void startCapture22();
  void startCapture44();
  void stopCapture();
  void setVolume(uint8_t vol);
};

const uint8_t addr = 0x3b;

#endif // RESPEAKER_H
