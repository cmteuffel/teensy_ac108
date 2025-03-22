# teensy_ac108
This is a minimal implementation of the ac108 quad-channel ADC utilised by the respeaker 4Mic array board. My intention was to bring a cheap microphone array into the Teensy universe (https://www.pjrc.com/teensy) and to allow others to experiment with beamforming on an embedded system. 

The implementation details were scraped from https://github.com/respeaker/seeed-voicecard.
The TDM implementation is a shameless copy from https://github.com/yasmeensultana/ad1938_codec . See also https://www.creative-technologies.de/wp-content/uploads/2018/01/Open-Source-Audio-Platform-for-Embedded-Systems.pdf
AudioRec is a minimal example how to record data with a teensy development board. 

The code was tested on a Teensy 4.1 with Arduino 1.8.13 and Teensyduino 1.54.

To use this with the Arduino IDE just copy the files into the same folder as where your INO file resides. However, you may need to rename input_tdm or overwrite the existing file that resides in your Arduino installation folder under hardware/teensy/avr/libraries/Audio/ (at your own risk if you have a different Teensyduino version).

[Thanks](https://www.daslai.de)
