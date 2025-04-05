#include <stdio.h>
#include <stdlib.h>
#include "wavfile.h"
#include "string.h"
#include "log.h"

#pragma warning (disable : 4018 ) //Signed Unsigned mismatch

wavedef Wave;

int WavFileLoadInternal(unsigned char *wavfile, int size)
{
    int i;

	if(memcmp(wavfile, "RIFF", 4) != 0)
	{
	    LOG_ERROR( "Not a wav file");
		return false;
	}

	Wave.channels = *(short *)(wavfile+0x16);
	Wave.sampleRate = *(unsigned short *)(wavfile+0x18);
	Wave.blockAlign = *(short *)(wavfile+0x20);
	Wave.bitPerSample  = *(short *)(wavfile+0x22);

	for(i = 0; memcmp(wavfile + 0x24 + i, "data", 4) != 0; i++)
	{
		if(i == 0xFF)
		{return false;}
	}

	Wave.dataLength = *(unsigned long *)(wavfile + 0x28 + i);

	if(Wave.dataLength + 0x2c > size)
	{return false;}

	if(Wave.channels != 2 && Wave.channels != 1)
	{return false;}

	if(Wave.sampleRate > 100000 || Wave.sampleRate < 2000)
	{return false;}

	if(Wave.channels == 2)
	{Wave.sampleCount = Wave.dataLength/(Wave.bitPerSample>>2);}
	else
	{Wave.sampleCount = Wave.dataLength/((Wave.bitPerSample>>2)>>1);}

	if(Wave.sampleCount <= 0)
	{return false;}
   
	Wave.data = wavfile + 0x2c;
 	return true;
}

