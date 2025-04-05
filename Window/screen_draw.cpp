#include "tglfw.h"
#include "romscan.h"
#include "screen_draw.h"
#include "wintimercode.h"
#include "config.h"
#include "log.h"

#include "spritesheet.h"
#include "animation.h"
#include "font_helper.h"
//#include "menu.h" //Going to get back to here eventually.
#include "globals.h"
#include <memory>

//std::unique_ptr<BMFont> mfont(new BMFont(SCREEN_W, SCREEN_H));
//std::unique_ptr<Sprite> sp(new Sprite());
//BMFont *mfont;
Sprite* sp;

//extern std::unique_ptr<Sprite> sp;

float getxscale(float val)
{
	return val * ((float)SCREEN_W / 640.0);
}

float getyscale(float val)
{
	return val * ((float)SCREEN_H / 480.0);
}

float get_scale()
{
	return 1.0f * (float)SCREEN_H / 480.0;
}

void draw_rect(float x, float y, float w, float h)
{
	glPointSize(6.0);
	glColor4ub(255, 250, 0, 128);
	glBegin(GL_QUADS);

	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();
}

void screen_init()
{
	font_init();
	//std::unique_ptr<Sprite> sp(new Sprite());
	//sp->setPath("data\\");
	sp = new Sprite("yamfegraphics.xml");
	

	fillstars(SCREEN_W, SCREEN_H);
}

void screen_shutdown()
{
	delete sp;
	font_destroy();
}

static void screen_animate()
{
	static int st = 6;
	static int stdir = 0;
	static int but = 0;
	static int butlp = 0;
	static int bug = 2;

	if (ani_counter)
	{
		//reset for next pass
		ani_counter = 0;

		//Set stick animation direction
		if (stdir) { st++; }
		else { st--; }

		//Animation frame range checking.
		if (st > 8) { stdir = 0; }
		if (st < 6) { stdir = 1; }

		//Cascading counters for stick and button animation
		bug++; if (bug > 4) { bug = 2; }
		butlp++;
		if (butlp > 1) {
			butlp = 0;
			but++;
			if (but > 1) { but = 0; }
		}
	}

	switch (rotation)
	{
	case 0:
		sp->setRotationAngle(bug, 0);
		sp->setRotationAngle(st, 0);
		sp->setRotationAngle(10, 0);
		sp->setRotationAngle(11, 0);
		sp->setRotationAngle(but, 0);
		sp->Add(get_scale(), getxscale(25.0), getyscale(252.0), bug); //Bug Add(float scale, float x, float y, int spritenum)
		sp->Add(get_scale(), getxscale(615.0), getyscale(252.0), bug); //Bug
		sp->Add(get_scale(), getxscale(234.0), getyscale(72.0), st); //Stick
		sp->Add(get_scale(), getxscale(320.0), getyscale(42.0), 10); //Bottom Text 300
		sp->Add(get_scale(), getxscale(241.0), getyscale(28.0), but); //Button //225
		sp->Add(get_scale() *.8, getxscale(320.0), getyscale(440.0), 11); //Title
		break;

	case 1:
		sp->setRotationAngle(bug, 90);
		sp->setRotationAngle(st, 90);
		sp->setRotationAngle(10, 90);
		sp->setRotationAngle(11, 90);
		sp->setRotationAngle(but, 90);
		sp->Add(get_scale(), getxscale(320.0), getyscale(25.0), bug); //Bug Add(float scale, float x, float y, int spritenum)
		sp->Add(get_scale(), getxscale(320.0), getyscale(460.0), bug); //Bug
		sp->Add(get_scale(), getxscale(568.0), getyscale(160.0), st); //Stick
		sp->Add(get_scale(), getxscale(600.0), getyscale(240.0), 10); //Bottom Text 300
		sp->Add(get_scale(), getxscale(612.0), getyscale(165.0), but); //Button //225
		sp->Add(get_scale(), getxscale(45.0), getyscale(240.0), 11); //Title
		break;

	case 2:
		sp->setRotationAngle(bug, 270);
		sp->setRotationAngle(st, 270);
		sp->setRotationAngle(10, 270);
		sp->setRotationAngle(11, 270);
		sp->setRotationAngle(but, 270);
		sp->Add(get_scale(), getxscale(320.0), getyscale(25.0), bug); //Bug Add(float scale, float x, float y, int spritenum)
		sp->Add(get_scale(), getxscale(320.0), getyscale(460.0), bug); //Bug
		sp->Add(get_scale(), getxscale(73.0), getyscale(320.0), st); //Stick
		sp->Add(get_scale(), getxscale(42.0), getyscale(240.0), 10); //Bottom Text 300
		sp->Add(get_scale(), getxscale(27.0), getyscale(315.0), but); //Button //225
		sp->Add(get_scale(), getxscale(598.0), getyscale(240.0), 11); //Title
		break;
	}
}

// Remove when you figure out how to get this
// to work in the rotated bmfont code
float get_width(std::string& string)
{
	float fwidth;
	float width;

	fwidth = (float)font_get_string_width(string) / (float)2.0; //mfont->GetStringWidth(string)
	if (rotation)
	{
		width = (float)SCREEN_H / (float)2.0f;
	}
	else
	{
		width = (float)SCREEN_W / (float)2.0f;
	}

	if ((rotation == 0) || (rotation == 1))
		width = width - fwidth;
	else width = width + fwidth;

	return width;
}

static void show_double_buffer()
{
	int i, b, x;
	int g = 1;

	//SCREEN VARIABLES
	float  spacing = 21.0;     //Spacing between text lines
	float center = 308;      //Center of Screen for Text Drawing
	int num_rows = 7;        //Number of rows of text to draw

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_TEXTURE_2D);
	movestars();
	placestars();

	font_set_color(RGB_WHITE);

	switch (rotation)
	{
	case 0:

		spacing = 24.0 * get_scale();
		center = 236.0 * get_scale();
		num_rows = 7;
		font_set_scale(get_scale());
		draw_rect(50.0f * get_scale(), 240.0f * get_scale(), 500.0 * get_scale(), spacing);
		break;
	case 1:
		draw_rect(310.0f, 50.0f, spacing, 385.0f);
		spacing = 21.0;
		center = 333.0;
		num_rows = 11;
		font_set_scale(.80f);
		break;
	case 2:
		draw_rect(310.0f, 50.0f, spacing, 385.0f);
		spacing = 21.0;
		center = 308.0;
		num_rows = 11;
		font_set_scale(.80f);
		break;
	case 3:
		draw_rect(50.0f, 240.0f, 540.0, spacing);
		spacing = 23.0;
		center = 237.0;
		num_rows = 7;
		font_set_scale(.85f);
		break;
	}

	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Update animation Routine
	screen_animate();

	//set vars for text display
	i = keyin;         //Variable for walking up gamelist
	b = keyin;         //Variable for walking downgamelist
	curr_game = keyin; //Current selected game, this is for the Createprocess

	//Output screen text

	font_set_scale(get_scale());

	//Center Line
	switch (rotation)
	{
	case 0: font_set_angle(0);
		font_print(get_width(gamelist[i].glname), center, gamelist[i].glname);
		break;
	case 1: font_set_angle(90);
		font_print(center, get_width(gamelist[i].glname), gamelist[i].glname);
		break;
	case 2: font_set_angle(270);
		font_print(center, get_width(gamelist[i].glname), gamelist[i].glname);
		break;
	case 3: font_set_angle(180);
		font_print(center, get_width(gamelist[i].glname), gamelist[i].glname);
		break;
	}
	//The rest of the lines of text
	for (x = 1; x < num_rows; x++)
	{
		if (num_games > (g + 1))
		{
			i = gamelist[i].next;

			switch (rotation)
			{
			case 0: font_print(get_width(gamelist[i].glname), center - (spacing * x), gamelist[i].glname);
				break;
			case 1: font_print(center - (spacing * x), get_width(gamelist[i].glname), gamelist[i].glname);
				break;
			case 2: font_print(center - (spacing * x), get_width(gamelist[i].glname), gamelist[i].glname);
				break;
			case 3: font_print(center - (spacing * x), get_width(gamelist[i].glname), gamelist[i].glname);
				break;
			}
		}
		if (num_games > g)
		{
			b = gamelist[b].prev;

			switch (rotation)
			{
			case 0: font_print(get_width(gamelist[b].glname), center + (spacing * x), gamelist[b].glname);
				break;
			case 1: font_print(center + (spacing * x), get_width(gamelist[b].glname), gamelist[b].glname);
				break;
			case 2: font_print(center + (spacing * x), get_width(gamelist[b].glname), gamelist[b].glname);
				break;
			case 3: font_print(center + (spacing * x), get_width(gamelist[b].glname), gamelist[b].glname);
				break;
			}
		}
		g++;
	}
}

void screen_update()
{
	//if (showmenu){do_the_menu();}
	//else

	if (sav_counter) { ssaver(); }
	else
	{
		show_double_buffer();
	}  //Display Scene

	font_render();
	sp->Render();

	/*
	if (showmenu){do_the_menu();}//{show_menu();}
	else
	show_double_buffer();  //Display Scene
	*/
}

void screen_draw_loading(int rotation)
{
	string a = "LOADING GAME";
	string b = "PLEASE WAIT !!";

	font_print(160 * 2, 150 * 2, a);
	font_print(160 * 2, 130 * 2, b);

	/*
	////Do Video OUT
	alfont_set_font_size(my_font, (fesetup.fontsize+5)); //Set font size for selected game

	if (rotation==1 || rotation ==2){
	alfont_textout_centre_aa_ex(tempbg, my_font, "LOADING GAME", 120 * 2, 100 * 2, WHITE ,0);
	alfont_textout_centre_aa_ex(tempbg, my_font, "PLEASE WAIT !!", 120 * 2, 130 * 2, WHITE ,0);
	}
	else {alfont_textout_centre_aa_ex(tempbg, my_font, "LOADING GAME", 160 * 2, 150 * 2, WHITE ,0);
		  alfont_textout_centre_aa_ex(tempbg, my_font, "PLEASE WAIT !!", 160 * 2, 130 * 2, WHITE ,0);
		 }

	clear_bitmap(dblbuffer);
		if (rotation==0){blit(tempbg, dblbuffer, 0, 0, 0, 0,SCREEN_W, SCREEN_H);}
		if (rotation==1){rotate_sprite(dblbuffer, tempbg, 40, -40, itofix(64));} //Rotate Right
		if (rotation==2){rotate_sprite(dblbuffer, tempbg, 40, -40, itofix(-64));} //Rotate Left
		if (rotation==3){rotate_sprite(dblbuffer, tempbg, 0, 0, itofix(128));} //Flip
		blit(dblbuffer, screen, 0, 0, 0, 0, 640, 480); //dblbuffer
	vsync();
	*/
}

void savreset()
{
	sav_counter = 0;
}

void ssaver()
{
	int k = 0;
	static int x, y;
	static unsigned char r, g, b;
	static int last_ssaver = -1;

	std::string message;
	message.assign(savermess);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//Render Objects
	glEnable(GL_TEXTURE_2D);

	float length = get_width(message);

	if (last_ssaver != sav_counter)
	{
		r = rand() % 255;
		g = rand() % 255;
		b = rand() % 255;

		if (rotation == 1 || rotation == 2)
		{
			y = rand() % SCREEN_W;
			x = rand() % (SCREEN_H - (int)length);
		}
		else {
			y = rand() % SCREEN_H;
			x = rand() % (SCREEN_W - int(length));
		}
		last_ssaver = sav_counter;
	}
	font_print((float)x, (float)y, MAKE_RGBA(r, g, b, 200), 1.0f, message);
}