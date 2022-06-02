#ifndef AUDI0_REC_H
#define AUDIO_REC_H


#include <Audio.h>
#include "respeaker.h"
#include "input_tdm32.h"
#include <SD.h>
#include <SPI.h>

#define HOP_SIZE 128

int getMaxVal();
void setupRec();
void clearBuffer();
void freeBuffer();
int readAudio(objects *objs);
float getMicVolume();
void setMicVolume(float vol);
bool recordingInitialized();

bool isFilled();
void resetRawRec();
void audioRecPause();
void audioRecResume();


#endif
