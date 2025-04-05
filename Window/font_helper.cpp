
#include "tglfw.h"
#include "config.h"
#include "log.h"
#include "bmfont.h"

#include "globals.h"
#include <memory>


//std::unique_ptr<BMFont> mfont2;

BMFont *mfont2;

void font_init()
{
	//std::unique_ptr<BMFont> mfont2(new BMFont(SCREEN_W, SCREEN_H));
	mfont2 = new BMFont(SCREEN_W, SCREEN_H);
	mfont2->SetPath("data\\");
	mfont2->LoadFont("handel.fnt");
}

void font_set_angle(int angle)
{
	mfont2->SetAngle(angle);
}

void font_set_scale(float scale)
{
	mfont2->SetScale(scale);
}

void font_set_color(rgb_t color)
{
	mfont2->SetColor(color);
}

float font_get_string_width(std::string &string)
{
	return mfont2->GetStringWidth(string);
}

void font_print(float x, float y, std::string &string)
{
	mfont2->Print(x, y, string.c_str());
}

void font_print(float x, float y, rgb_t color, float scale, std::string &string)
{
	mfont2->Print(x, y, color, scale, string.c_str());
}

void font_render()
{
	mfont2->Render();
}

void font_destroy()
{
	delete mfont2;
}
