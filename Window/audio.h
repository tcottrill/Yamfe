#ifndef AUDIO_H
#define AUDIO_H
//

extern void audio_init();
extern void audio_shutdown();
extern void audio_play(int num, int vol, int loop);


enum {A_RUNGAME = 0, A_INTRO, A_MOVE, A_END , A_ERROR};

#endif