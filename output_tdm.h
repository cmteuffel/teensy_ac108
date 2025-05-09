/* Audio Library for Teensy 3.X
 * Copyright (c) 2017, Paul Stoffregen, paul@pjrc.com
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef output_tdm_h_
#define output_tdm_h_

#include "Arduino.h"
#include "AudioStream.h"
#include "DMAChannel.h"

class AudioOutputTDM : public AudioStream
{
public:
	AudioOutputTDM(void) : AudioStream(16, inputQueueArray) { begin(); }
	virtual void update(void);
	void begin(void);
	friend class AudioInputTDM;
protected:
	AudioOutputTDM(int dummy) : AudioStream(16, inputQueueArray) {  }
	static void config_tdm(void);
	static audio_block_t *block_input[16];
	static bool update_responsibility;
	static DMAChannel dma;
	static void isr(void);
private:
	audio_block_t *inputQueueArray[16];
};

class AudioOutputTDMslave : public AudioOutputTDM
{
public:
	AudioOutputTDMslave(void) : AudioOutputTDM(0) { begin(); } ;
	void begin(void);
	friend class AudioInputTDMslave;
	friend void dma_ch0_isr(void);
protected:
	static void config_tdm(void);
};

#endif
