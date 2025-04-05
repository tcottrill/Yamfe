// Porting shim emulates a subset of the old Allegro C API.

#include <windows.h>
#include "sndlib.h"
#include <xaudio2.h>

//#pragma comment(lib,"ole32.lib")
#pragma comment(lib, "xaudio2.lib")

#include  <assert.h> 
#include <map>
#include <memory>
using namespace std;
#include "wavfile.h"
#include "sys_fileio.h"
#include "log.h"
#include <thread>
#include <mutex>

struct Voice
{
    Voice()
      : voice(nullptr),
        sample(nullptr),
        isAllocated(false),
        isReleased(false),
        isPlaying(false),
        isLooped(false),
        frequency(DefaultFrequency),
        volume(255),
        pan(128)
    {
        ZeroMemory(&buffer, sizeof(buffer));
    }

    IXAudio2SourceVoice* voice;
    SAMPLE const* sample;
    XAUDIO2_BUFFER buffer;
    bool isAllocated;
    bool isReleased;
    bool isPlaying;
    bool isLooped;
    int frequency;
    int volume;
    int pan;

    static const int MaxVoices = 16;
    static const int DefaultFrequency = 22050;
};


static Voice voices[Voice::MaxVoices];

static IXAudio2  *xaudio2;
static IXAudio2MasteringVoice* masteringVoice;
static DWORD channelMask;

static mutex audioMutex;


int InstallSound()
{

	//must call this for COM
	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	//create the engine
	if( FAILED(XAudio2Create(&xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR) ) )
	{
		CoUninitialize();
		return -1;
	}

	//create the mastering voice
	if( FAILED( xaudio2->CreateMasteringVoice(&masteringVoice) ) )
	{
		CoUninitialize();
		return -1;
	}
/*
#if (_WIN32_WINNT >= 0x0602 //_WIN32_WINNT_WIN8)

  if( FAILED(      masteringVoice->GetChannelMask(&channelMask) )) //Only Windows 8
 	 {
		CoUninitialize();
		return -1;
	}
#endif
*/
    WAVEFORMATEX format =
    {
        WAVE_FORMAT_PCM,
        1,
        Voice::DefaultFrequency,
        Voice::DefaultFrequency,
        1,
        8,
        0
    };

    for (int i = 0; i < Voice::MaxVoices; i++)
    {
     if( FAILED( xaudio2->CreateSourceVoice(&voices[i].voice, &format) ))

	  {
		CoUninitialize();
		return -1;
	 }

	}
return 1;
}


SAMPLE *load_wav(const char *filename)
{
	uint8_t *wavdata=0;
	
	wavdata = loadFile(filename);

	if (WavFileLoadInternal(wavdata, getLastFileSize()))
	{
	  SAMPLE* result = new SAMPLE;
	  
	  result->data = Wave.data;
      result->freq = Wave.sampleRate;
      result->len = Wave.dataLength;
      
	  wrlog("sample rate %d",Wave.sampleRate);
	  wrlog("sample bps %d",Wave.bitPerSample);
	  return result;
	}
 //Failure
 LOG_DEBUG("Wave file load failure"); 
 return 0;
}

SAMPLE *create_sample(int bits, int stereo, int freq, int len)
{
    // This shim only bothers to support 8 bit mono format.
    assert(bits == 8);
    assert(!stereo);

    SAMPLE* result = new SAMPLE;

    result->data = malloc(len);
    result->freq = freq;
    result->len = len;

    return result;
}


void destroy_sample(SAMPLE* spl)
{
    free(spl->data);

    delete spl;
}


int play_sample(const SAMPLE* spl, int vol, int pan, int freq, int loop)
{
    int voice = allocate_voice(spl);

    if (voice >= 0)
    {
        voice_set_volume(voice, vol);
        voice_set_pan(voice, pan);
        voice_set_frequency(voice, spl->freq * freq / 1000);
        voice_set_playmode(voice, loop ? PLAYMODE_LOOP : PLAYMODE_PLAY);
        voice_start(voice);

        // Fire & forget.
        lock_guard<mutex> lock(audioMutex);
        voices[voice].isReleased = true;
    }

    return 0;
}


int allocate_voice(const SAMPLE *spl) 
{
    lock_guard<mutex> lock(audioMutex);
    
    for (int i = 0; i < Voice::MaxVoices; i++)
    {
        // If this is an active fire & forget voice, check whether it has finished playing.
        if (voices[i].isAllocated && voices[i].isReleased)
        {
            XAUDIO2_VOICE_STATE state;


            voices[i].voice->GetState(&state);//, XAUDIO2_VOICE_NOSAMPLESPLAYED);

            if (state.BuffersQueued == 0)
            {
                voice_stop(i);

                voices[i].isAllocated = false;
            }
        }
        
        // If this voice is free, allocate it now.
        if (!voices[i].isAllocated)
        {
            voices[i].sample = spl;
            voices[i].isAllocated = true;
            voices[i].isReleased = false;
            voices[i].isPlaying = false;
            voices[i].isLooped = false;
            voices[i].frequency = Voice::DefaultFrequency;
            voices[i].volume = 255;
            voices[i].pan = 128;

            return i;
        }
    }
    
    return -1;
}


void reallocate_voice(int voice, const SAMPLE *spl) 
{
    assert(!voices[voice].isPlaying);

    voices[voice].sample = spl;
}


void deallocate_voice(int voice) 
{
    lock_guard<mutex> lock(audioMutex);

    voice_stop(voice);

    voices[voice].isAllocated = false;
}


static void SetPan(IXAudio2SourceVoice* voice, float pan)
{
    float left = 0.5f - pan / 2;
    float right = 0.5f + pan / 2; 

    float outputMatrix[8] = { 0 };
    int nChannels = 0;

	outputMatrix[0] = left;
    outputMatrix[1] = right;
    nChannels = 2;

    voice->SetOutputMatrix(nullptr, 1, nChannels, outputMatrix);
}


void voice_start(int voice)
{
    Voice& v = voices[voice];

    assert(!v.isPlaying);

    v.buffer.AudioBytes = v.sample->len;
    v.buffer.pAudioData = (BYTE const*)v.sample->data;
    v.buffer.LoopCount = v.isLooped ? XAUDIO2_LOOP_INFINITE : 0;

    v.voice->SubmitSourceBuffer(&v.buffer);
    v.voice->SetFrequencyRatio((float)v.frequency / (float)v.sample->freq);
    v.voice->SetVolume((float)v.volume / 255.0f);

    SetPan(v.voice, (float)(v.pan - 128) / 128.0f);

    v.voice->Start();

    v.isPlaying = true;
}


void voice_stop(int voice)
{
    if (voices[voice].isPlaying)
    {
        voices[voice].voice->Stop();
        voices[voice].voice->FlushSourceBuffers();

        voices[voice].isPlaying = false;
    }
}


void voice_set_volume(int voice, int volume)
{
    assert(!voices[voice].isPlaying);

    voices[voice].volume = volume;
}


void voice_set_frequency(int voice, int frequency)
{
    assert(!voices[voice].isPlaying);

    voices[voice].frequency = frequency;
}


void voice_set_pan(int voice, int pan)
{
    assert(!voices[voice].isPlaying);

    voices[voice].pan = pan;
}


void voice_set_priority(int voice, int priority)
{
    // no-op
}


void voice_set_playmode(int voice, int playmode)
{
    assert(!voices[voice].isPlaying);

    voices[voice].isLooped = (playmode == PLAYMODE_LOOP);
}

