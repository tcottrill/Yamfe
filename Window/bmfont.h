/*
BMFont example implementation with Kerning, for C++ and OpenGL 2.0+

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>

Version 3.01 2017 aaedev@gmail.com
*Back after an extended hiatus
*General Bug Fixes
*Added lots of comments
*Preparation for shader version
*/

#ifndef __BMFONT__
#define __BMFONT__

#include <vector>
#include <map>
#include <unordered_map>
#include "colordefs.h"
#include "gl_basics.h"
#include "simple_texture.h"


//This can and should be substituted by your own Vec2f implementation
class fpoint
{
public:
	float x;
	float y;
	fpoint(float x, float y) : x(x), y(y) {}
	fpoint() : x(0), y(0) {}
	~fpoint() {};
};


//This too can be replaced 
class txdata
{
public:

	float x, y;
	float tx, ty;
	rgb_t colors;

	txdata() : x(0), y(0), tx(0), ty(0), colors(0) { }
	txdata(float x, float y, float tx, float ty, rgb_t colors) : x(x), y(y), tx(tx), ty(ty), colors(colors) {}
};


class KearningInfo
{
public:
	short First;
	short Second;
	short Amount;

	KearningInfo() : First(0), Second(0), Amount(0) { }
};


//This is the actual Character class that hold the data for each char 
class CharDescriptor
{
public:
	short x, y;
	short Width;
	short Height;
	short XOffset;
	short YOffset;
	short XAdvance;
	short Page;

	CharDescriptor() : x(0), y(0), Width(0), Height(0), XOffset(0), YOffset(0),	XAdvance(0), Page(0) { }
};

//This is the main class file
class BMFont
{
public:
	//Enums for constants
	enum fontalign { AlignNone, AlignNear, AlignCenter, AlignFar };
	enum fontorigin { Top, Bottom };
	//Font load function
	bool  LoadFont(std::string fontfile);
	//Adjusters
	void  SetColor(int r, int g, int b, int a) { fcolor = MAKE_RGBA(r, g, b, a); }
	void  SetColor(rgb_t color) { fcolor = color; }
	void  SetBlend(int b) { fblend = b; }
	void  SetScale(float);
	void  SetCaching(bool en);
	void  ClearCache() { txlist.clear(); };
	void  SetAngle(int);
	void  SetAlign(fontalign align);
	void  SetOrigin(fontorigin origin);
	float GetHeight() { return LineHeight * fscale; }
	void  UseKerning(bool b) { UseKern = b; }
	//Pathing functions.
	void SetPath(std::string &path);
	void SetPath(const char *path);
	std::string GetPath();
	//Printing
	void  Print(float x, float y, const char *fmt, ...);
	void  Print(float scale, float x, float y, const char *fmt, ...);
	void  Print(float x, float y, rgb_t color, float scale, const char *fmt, ...);
	void  Print(float y, rgb_t color, float scale, int angle, const char *fmt, ...);
    //Screen related functions
	float GetStringWidth(const char *);
	float GetStringWidth(std::string &string);
	//Renderer Function
	void  Render();

	BMFont(int width, int height)
	{
		surface_width = width;              //Size of texture atlas width
		surface_height = height;			//Size of texture atlas height 
		MathTableInit();					//Math table needs to be initialized on first use
		SetColor(RGB_WHITE);			    //Default color is White
		KernCount = 0;						//Set the Total for Kerning Information
		fblend = 0;							//Blending Value for Depreciated GL 2 Functions.
		fscale = 1.0;						//Default scaling value
		fangle = 0;							//Default Rotation Angle
		fcache = false;						//Whether or not to use caching
		falign = fontalign::AlignNone;		//Default Drawing alignment. 
		forigin = fontorigin::Bottom;		//Default drawing Origin
		UseKern = true;						//To use the supplied kerning values or not
	};
	~BMFont();


private:
	//Character and page variables
	short LineHeight; 
	short Base;								//Captured but not used in my code. What is this used for?
	short Width;							//Width in pixels of the texture atlas. My code only supports 1 png currently.
	short Height;							//Height in pixels of the texture atlas. My code only supports 1 png currently.
	short Pages;							//Number of pages, currently I only support 1
	short Outline;							//TBD
	short KernCount;						//Total for Kerning Information
	std::map<int, CharDescriptor> Chars;    //Character Descriptor Map
	std::unordered_map<std::size_t, double> KernMap;    // For Kerning Lookup
	std::vector<txdata> txlist;             //Vertex Array Data
	std::string Pngname;                    //Png File name storage for loading

	//Modifiers
	bool UseKern;							//Whether or not to process kerning information 
	rgb_t fcolor;							//Font current color
	TEX *ftexid = nullptr;					//Font Texture ID for the Font Atlas
	float fscale;							//Current Font scaling factor
	int fblend;								//Current Rendering Blending Value "Do we need this?"
	int falign;								//Font Alignment Variable
	bool forigin;							//Set Font origin Top Or Bottom
	int fangle;								//Set font angle
	bool fcache;                            //Enable Text Caching. WARNING! Text must be pre-staged before entering main program loop! (This is unrefined code)
	int surface_width;                      //Width of the orthographic 2D surface the text is going on
	int surface_height;                     //Height of the orthographic 2D surface the text is going on
	std::string datapath;                   //Path for data files, if empty use the root directory    

	//Internal Functions
	bool ParseFont(const std::string &);
	int  GetKerningPair(int, int);
	void PrintKerningPairs();      //For Debug use only!
	void PrintString(float x, float y, char *textstr);
	
	//Math Functions
	void   MathTableInit();
	fpoint MakePoint(float x, float y) { fpoint p; p.x = x; p.y = y; return p; }
	fpoint RotateAroundPoint(float, float, float, float, float, float);

	//Math Table data for rotation
	float _sin[360];
	float _cos[360];
};


#endif
