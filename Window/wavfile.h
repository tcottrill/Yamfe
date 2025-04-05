#pragma once

#ifndef WAVFILE_H
#define WAVFILE_H


typedef struct
{
	short int channels;		/**<  Number of channels */
	unsigned short sampleRate;	/**<  Sample rate */
	unsigned long sampleCount;	/**<  Sample count */
	short int blockAlign;
    unsigned long dataLength;	/**<  Data length */
	unsigned char *data;		/**< A pointer to the actual WAV data */
	short int bitPerSample;	/**<  The bit rate of the WAV */
		
}wavedef;

extern wavedef Wave;
int WavFileLoadInternal(unsigned char *wavfile, int size);

#endif