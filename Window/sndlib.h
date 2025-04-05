// Porting shim emulates a subset of the old Allegro C API.
// From Shawn Hargreves

#pragma once

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>

#pragma warning (disable: 4244 4305 4996)

#ifdef __cplusplus
extern "C" {
#endif


// Emulated Allegro audio methods.
enum PLAYMODE
{
    PLAYMODE_PLAY,
    PLAYMODE_LOOP,
};

typedef struct
{
    void* data;
    int freq;
    int len;
} SAMPLE;

int InstallSound();

SAMPLE *load_wav(const char *filename);

SAMPLE *create_sample(int bits, int stereo, int freq, int len);
void destroy_sample(SAMPLE* spl);
int play_sample(const SAMPLE* spl, int vol, int pan, int freq, int loop);
int allocate_voice(const SAMPLE *spl); 
void reallocate_voice(int voice, const SAMPLE *spl); 
void deallocate_voice(int voice); 
void voice_start(int voice);
void voice_stop(int voice);
void voice_set_volume(int voice, int volume);
void voice_set_frequency(int voice, int frequency);
void voice_set_pan(int voice, int pan);
void voice_set_priority(int voice, int priority);
void voice_set_playmode(int voice, int playmode);


// Emulated Allegro misc defines.
#define TRUE    1
#define FALSE   0

#define M_PI    3.14159265358979323846

#define MIN(a, b)   (((a) < (b)) ? (a) : (b))
#define MAX(a, b)   (((a) > (b)) ? (a) : (b))
#define ABS(a)      (((a) >= 0) ? (a) : -(a))
#define SGN(a)      (((a) > 0) ? 1 : (((a) < 0) ? -1 : 0))


#ifdef __cplusplus
}


#endif  // __cplusplus
