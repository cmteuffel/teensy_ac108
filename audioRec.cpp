#include "audioRec.h"

ReSpeaker respeaker;

AudioInputTDMslave       tdm1(8);

AudioRecordQueue         queue1;
AudioRecordQueue         queue3;
AudioRecordQueue         queue5;
AudioRecordQueue         queue7;
AudioConnection          patchCord9(tdm1, 0, queue1, 0);
AudioConnection          patchCord2(tdm1, 1, queue3, 0);
AudioConnection          patchCord5(tdm1, 2, queue5, 0);
AudioConnection          patchCord7(tdm1, 3, queue7, 0);

int16_t tmp[128];
int blkcnt = 0;
bool initOK;

float curVol = 0;

bool recordingInitialized() {
  return initOK;
}

void setupRec() {
   if (respeaker.i2cOK()) {
    initOK = true;
    respeaker.initBoard();
    delay(1000);
    respeaker.startCapture22();
    delay(1000);

	  AudioMemory(64); 
	  AudioInterrupts();
  
    queue1.begin();
    queue3.begin();
    queue5.begin();
    queue7.begin();

    if (respeaker.captureStarted()) {
      Serial.println("started recording");
    } else {
      Serial.println("recording failed");
    }
  } else {
    initOK = false;
    Serial.println("I2C error");
  }
}

void audioRecPause() {
  queue1.end();
  queue3.end();
  queue5.end();
  queue7.end();
}

void audioRecResume() {
  queue1.begin();
  queue3.begin();
  queue5.begin();
  queue7.begin(); 
}


void clearBuffer() {
  queue1.clear();
  queue3.clear();
  queue5.clear();
  queue7.clear();
}

void freeBuffer() {
  queue1.freeBuffer();
  queue3.freeBuffer();
  queue5.freeBuffer();
  queue7.freeBuffer();
}


float getMicVolume() {
  return curVol;
}


void setMicVolume(float vol) {
  curVol = vol;
  uint8_t v = (uint8_t)(vol*255.0f);
  respeaker.setVolume(v);
}


int readAudio(objects *objs) {
  const int rd = HOP_SIZE;
  int avail = 0;
  avail = queue7.available();
  int16_t * b;
  if (avail > 0) { 
    memcpy(tmp, queue1.readBuffer(), rd*2);    //16 Bit int = 2 byte
    //ToDo: do something with tmp
    memcpy(tmp, queue3.readBuffer(), rd*2);    //16 Bit int = 2 byte
    //ToDo: do something with tmp
    memcpy(tmp, queue5.readBuffer(), rd*2);    //16 Bit int = 2 byte
    //ToDo: do something with tmp
    memcpy(tmp, queue7.readBuffer(), rd*2);    //16 Bit int = 2 byte
    //ToDo: do something with tmp
    freeBuffer();
  }
  return avail; 
}
