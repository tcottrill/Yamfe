#pragma once

#include "tglfw.h"
#include "glew.h"
#include "wglew.h"

#include <cstdlib>
#include <cstdio>
#include <memory>
#include "log.h"
#include "fpsclass.h"
#include "gl_basics.h"
#include "sndlib.h"
#include "MathUtils.h"
#include "simpletimer.h"
#include "globals.h"
#include "bmfont.h"
#include "spritesheet.h"

#include "osdepend.h"
#include "inptport.h"
#include "romscan.h"
#include "config.h"
#include "screen_draw.h"
#include "wintimercode.h"
#include "gameinput.h"

#include "audio.h"

//BOOL quit = FALSE;
FpsClass *m_frame;
TEX *mytex;


//std::unique_ptr<Sprite> sp;

//int mmove = 0;
//int rmove = 0;


INPUT_PORTS_START(fe)
PORT_START /* IN0 */
PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_BUTTON1 | IPF_SPULSE) //Start Game
PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_EXIT | IPF_SPULSE) //Exit
PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_BUTTON2 | IPF_SPULSE) //Menu
PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_BUTTON3 | IPF_SPULSE)
PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_BUTTON4 | IPF_SPULSE)
PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_BUTTON5 | IPF_SPULSE)
PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_BUTTON6 | IPF_SPULSE)
PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_UNUSED)
//Note: Mask can be zero

PORT_START //IN1
/*/
PORT_BITX(0x01, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP | IPF_4WAY | IPF_COUNTER, "UP", OSD_KEY_UP, OSD_JOY_UP)
PORT_BITX(0x02, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN | IPF_4WAY | IPF_COUNTER, "DOWN", OSD_KEY_DOWN, OSD_JOY_DOWN)
PORT_BITX(0x04, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT | IPF_4WAY | IPF_SPULSE, "RIGHT", OSD_KEY_LEFT, OSD_JOY_LEFT)
PORT_BITX(0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT | IPF_4WAY | IPF_SPULSE, "LEFT", OSD_KEY_RIGHT, OSD_JOY_RIGHT)
*/
PORT_BITX(0x01, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT | IPF_4WAY | IPF_COUNTER, "LEFT", OSD_KEY_LEFT, OSD_JOY_LEFT)
PORT_BITX(0x02, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT | IPF_4WAY | IPF_COUNTER, "RIGHT", OSD_KEY_RIGHT, OSD_JOY_RIGHT)
PORT_BITX(0x04, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP | IPF_4WAY | IPF_SPULSE, "UP", OSD_KEY_UP, OSD_JOY_UP)
PORT_BITX(0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN | IPF_4WAY | IPF_SPULSE, "DOWN", OSD_KEY_DOWN, OSD_JOY_DOWN)

PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_UNUSED)
PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_UNUSED)
PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_UNUSED)
PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_UNUSED)


PORT_START	/* IN2 */
PORT_ANALOG(0xff, 0x80, IPT_PADDLE | IPF_PLAYER1 | IPF_REVERSE, 25, 10, 32, 0x00, 0xff)
//PORT_ANALOG(mask,def,type,sensitivity,delta,clip,min,max)
//PORT_ANALOGX( 0xff, 0x00, IPT_TRACKBALL_X, 50, 10, 0, 0, 0, IP_KEY_NONE, IP_KEY_NONE, IP_JOY_NONE, IP_JOY_NONE )  

//PORT_START	/* IN3 */														
//PORT_ANALOGX( 0xff, 0x00, IPT_TRACKBALL_Y, 50, 10, 0, 0, 0, IP_KEY_NONE, IP_KEY_NONE, IP_JOY_NONE, IP_JOY_NONE ) 
INPUT_PORTS_END


void draw()
{
	static int fpscnt = 0;
	char szBuff[255] = { 0 };
	int r, w;
	get_mouse_win(&r, &w);

	//New Input
	inputport_vblank_end();
	update_input_ports();
	//Input
	get_input();		   // get input 
	proc_input();          //Process Input

	screen_update();

	m_frame->Frame();
	tglSwapBuffers();

	fpscnt++;
	if (fpscnt > 60)
	{
		sprintf(szBuff, ("OpenGL - %hu FPS"), m_frame->GetFps());
		set_window_title(szBuff);
		fpscnt = 0;
	}
}


void data_init()
{
	GameList_Initalize("gamelist.txt");  //GET GAMELIST 
	wrlog("Passed Gamelist Load");

	wrlog("Passed game sort");

	wrlog("Mame Path %s", mamepath);
	//set_num_menu_items(); //Setup game menu root level.
}


/////////////////// MAIN() for program ///////////////////////////////////////////////////
int  main(int argc, char *argv[])
{

	config_init(); //SETUP and EXTRA OPTIONS and SYSTEM OPTIONS

	if (fesetup.startdelay)
	{
		Sleep(1000 * fesetup.startdelay);
	}

	int i=tglCreate_Window(1280, 1024, TGL_BORDERLESS);

	//int i = tglCreate_Window(640, 480, TGL_FULLSCR);//TGL_FULLSCR);

	int w = 1;
	win_set_vsync(FALSE);
	ViewOrtho(SCREEN_W, SCREEN_H);

	m_frame = new FpsClass;
	m_frame->Initialize();

	show_mouse();
	win_set_vsync(true);

	Machine.input_ports = input_ports_fe;
	load_input_port_settings();

	load_input_port_settings(); //Load saved keys and dips
	AllowAccessibilityShortcutKeys(false);

	//Load Game Resources
	data_init();
	keyin = 1;               //Setup key location
	alleg_init_input();
	timers_init();
	audio_init();
	screen_init();

	RECT _screen;

	//Build a RECT with the size of the complete window (Note: GetSystemMetrics only covers the main monitor, this won't work in a multi-monitor setup)
	_screen.left = 0;
	_screen.top = 0;
	_screen.right = GetSystemMetrics(SM_CXSCREEN);
	_screen.bottom = GetSystemMetrics(SM_CYSCREEN);

	//Unclip everything by using ClipCursor on the complete screen
	ClipCursor(&_screen);
	ShowCursor(FALSE);

	while (!key[KEY_ESC])
	{
		draw();
		ShowCursor(FALSE);
	}

	wrlog("Shutting Down, NOW!");
	screen_shutdown();
	save_input_port_settings();
	timers_shutdown();
	GameList_ShutDown();

	return 0;
}

