#pragma once

#include <Windows.h>
#include <string>
#include "rawinput.h"


//Defines for Window Creation Code
#define TGL_AUTO           0x0100000
#define TGL_RESIZEABLE     0x0010000
#define TGL_CENTERX        0x0001000	
#define TGL_CENTERY        0x0000100
#define TGL_BORDERLESS     0x0000010
#define TGL_FULLSCR        0x0000001	

extern void tglSwapBuffers();
extern int tglCreate_Window(int x, int y, int parm);
extern HWND win_get_window();
extern void win_set_vsync(bool enable);
extern void set_window_title( const char *title );
extern void allegro_message(const char *title, const char *message);
POINT get_mouse_pos();
extern void set_mouse_pos( int x, int y );
extern void scare_mouse();
extern void show_mouse();
extern int get_window_width();
extern int get_window_height();
extern void set_icon(const char * icon);
extern std::string GetLastErrorStdStr();
void AllowAccessibilityShortcutKeys(int bAllowKeys );
bool get_focus();
bool get_iconified();

extern int SCREEN_W;
extern int SCREEN_H;
extern MSG msg;
extern int quit;