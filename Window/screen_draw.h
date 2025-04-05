#ifndef SCREEN_DRAW_H
#define SCREEN_DRAW_H

extern int stimeout;
extern void screen_update();
extern void screen_draw_loading(int rotation);
extern void ssaver();
extern void savreset();
extern void screen_init();
extern void screen_shutdown();

extern float getxscale(float val);
extern float getyscale(float val);
extern float get_scale();
extern void  draw_rect(float x, float y, float w, float h);

#endif