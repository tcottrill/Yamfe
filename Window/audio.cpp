#include "sndlib.h"
#include "audio.h"
#include "log.h"
#define MAXPATH 254

static int audio_enabled=1;

SAMPLE *samp[8];
char *sample_list[]={"rungame.wav","intro.wav","move.wav","end.wav","error.wav","NULL"};


static int samples_init()
{
 int i=0;
 char *path = "data\\";
 char *fullpath = new char[256];

 do
	{
     strncpy(fullpath, path, 256 - 1);
	 strncat(fullpath,sample_list[i], 256 - strlen(fullpath)-1);
	 samp[i] = load_wav(fullpath);

     if (!samp[i]) {
                     wrlog("Error reading WAV file '%s'\n",sample_list[i]);
		             return 0;
                    }
  	
	  i++;
    } while (strcmp(sample_list[i],"NULL"));

 free (fullpath);
 return 1;
}


void audio_init()
{
         /* install a digital sound driver */
	if (InstallSound() != 1)
	   { 
		   wrlog("Error initialising sound system. Sound Disabled"); 
	       audio_enabled = 0;
	   }

	if(!samples_init())
     {
      wrlog("Sample load routine failed, Sound Disabled.");
	  audio_enabled =0;
     }
}


void audio_shutdown()
{
	 destroy_sample(samp[0]);
	 destroy_sample(samp[1]);
	 destroy_sample(samp[2]);
	 destroy_sample(samp[3]);
	 destroy_sample(samp[4]);
}


void audio_play(int num, int vol, int loop)
{
	if (audio_enabled)
     play_sample(samp[num],vol, 128, 1000, loop);
}

