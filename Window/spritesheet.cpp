/*
BASIC Sprite example implementation with Kerning, for C++ and OpenGL 2.0+

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
--------------------------------------------------------------------------------
This program uses textures and parses generic XML from this program:
 http://www.codeandweb.com/texturepacker
Currently it doesn't support sprite offset and rotation.

Although I'm giving this away, I'd appreciate an email with fixes or better code!

aaedev@gmail.com 2013
*/


#ifdef USING_ALLEGRO 
#include "allegro.h"
#include "winalleg.h"
#include "alleggl.h"
#else
#include "glew.h"
#include "wglew.h"
#endif


#include "log.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include "Spritesheet.h"
#include <iterator>       // std::distance
#include <stdexcept>      // std::out_of_range
#include "path_helper.h"


#pragma warning (disable : 4996 )


/**
Parses the Sprite Sheet definition file and reads the specified keywords.
This could be broken out in other, more robust token parsing code.
*/
bool Sprite::ParseDefinition(const std::string &fontfile)
{
	std::string Line;
	std::string Read, Key, Value;
	std::size_t i;

	wrlog("Sprite Def here is %s", fontfile.c_str());

	std::ifstream Stream(fontfile.c_str());
	//Temporary Sprite 
	SpriteDescriptor C;

	while (!Stream.eof())
	{
		std::stringstream LineStream;
		std::getline(Stream, Line);
		LineStream << Line;

		//read the line's type
		LineStream >> Read;

		if (Read == "<TextureAtlas")
		{
			//this holds common data
			while (!LineStream.eof())
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find('=');
				Key = Read.substr(0, i);
				Value = Read.substr(i + 1);

				if (Value.front() == '"') {
					Value.erase(0, 1); // erase the first character
					Value.erase(Value.size() - 1); // erase the last character
					//To remove the double quotes.
				}

				//assign the correct value
				Converter << Value;
				if (Key == "height")
				{
					Converter >> sheetHeight;
				}
				else if (Key == "width")
				{
					Converter >> sheetWidth;
				}
				else if (Key == "imagePath")
				{
					Converter >> Pngname;
				}
			}
		}
		else if (Read == "<sprite")
		{
			//This is data for each specific character.
			while (!LineStream.eof())
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find('=');
				Key = Read.substr(0, i);
				Value = Read.substr(i + 1);

				if (Value[Value.size() - 1] == '>')
				{
					Value.erase(0, 1); // erase the first character
					Value.erase(Value.size() - 3, 3); //erase the last 3 characters (xml def)
				}
				else if (Value.front() == '"')
				{
					Value.erase(0, 1); // erase the first character
					Value.erase(Value.size() - 1); // erase the last character
					  //To remove the double quotes.
				}

				//Assign the correct value
				Converter << Value;

				if (Key == "n")
				{
					Converter >> C.name;
					C.name.erase(C.name.size() - 4);
				}
				else if (Key == "x")
				{
					Converter >> C.x;
				}
				else if (Key == "y")
				{
					Converter >> C.y;
				}
				else if (Key == "w")
				{
					Converter >> C.Width;
				}
				else if (Key == "h")
				{
					Converter >> C.Height;
				}
			}

			//Sprites.insert(std::map<int,SpriteDescriptor>::value_type( count,C ));
			Sprites.push_back(C);
			count++;
		}
	}
	Stream.close();
	wrlog("Finished sprite load");
	return true;
}


/**
Simple loader that checks if the file exists and then loads it and the associated png
*/
bool  Sprite::LoadSheet(std::string fontfile)
{
	wrlog("Starting to parse Sprite Def file");
	/*
	if (!datapath.empty())
	{
		// Yes I realize this is ineffecient 
		fontfile = datapath + fontfile;
		wrlog("Path is %d", fontfile.c_str());
	}
	*/
	datapath = getpathM("data", fontfile.c_str());

	wrlog("datapath here is %s", datapath.c_str());

	std::ifstream Stream(datapath);
	if (!Stream.is_open())
	{
		wrlog("Cannot Find Sprite Definition File %s", datapath.c_str());
		return false;
	}
	Stream.close();

	wrlog("Passing Sprite Load");
	wrlog("Starting to Parse XML %s", fontfile.c_str());
	ParseDefinition(datapath);
	wrlog("Finished Parsing XML %s", fontfile.c_str());

	datapath = getpathM("data", Pngname.c_str());

	ftexid = LoadTexture(datapath);
	if (!ftexid)
	{
		wrlog("Cannot find font texture for loading %s", datapath.c_str());
		return false;
	}

	wrlog("File name %s", Pngname.c_str());
	wrlog("Sheetwidth %d", sheetWidth);
	wrlog("Sheetheight %d", sheetHeight);
	wrlog("Sprite count %d", count);

	wrlog("SPRITES:\n");

	for (int i = 0; i < count; ++i)
	{
		wrlog("Sprite name: %s", Sprites[i].name.c_str());
		wrlog("Sprite x:%d y %d", Sprites[i].x, Sprites[i].y);
		wrlog("Sprite width:%d height %d \n", Sprites[i].Width, Sprites[i].Height);
	}

	//Pre allocate enough memory in the Texture list to account for all the sprite data.
	txlist.reserve(count * 6);

	return true;
}

//Simple Vector Renderer Needs moved to it's own file and class for real world use. 
void Sprite::Render()
{
	//Select and enable the font texture. (With mipmapping.)
	UseTexture(&ftexid->texid, true, true);

    //Set type of blending to use with this font.
	SetBlendMode(fblend);

	//Enable Client States
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(stxdata), &txlist[0].x);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(stxdata), &txlist[0].tx);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(stxdata), &txlist[0].colors);
	glDrawArrays(GL_TRIANGLES, 0, txlist.size());

	// Finished Drawing, disable client states.
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	//Caching Check, if we are pre caching the text.
	if (!fcache) txlist.clear();
}


void Sprite::MathTableInit()
{
	for (int a = 0; a < 360; a++)
	{
		_sin[a] = (float)sin((float)a * 3.14159265358979323846264f / 180.0f);
		_cos[a] = (float)cos((float)a * 3.14159265358979323846264f / 180.0f);
	}
}


void Sprite::SetCaching(bool en)
{
	if (en) fcache = true;
	else fcache = false;
}


/**
This is the main printing process that scales and fills in the vector structure to be printed later.
*/
void Sprite::InternalAdd(float x, float y, int spriteid)
{
	//spoint point;              //Vector point
	SpriteDescriptor  *f;		   //Pointer to sprite.
	float MinX = 0.0f;
	float MinY = 0.0f;
	float MaxX = 0.0f;
	float MaxY = 0.0f;

	float s;
	float t;
	float w;
	float h;

	//Now scaled X and Y are the new origins.

	f = &Sprites[spriteid];

	//Get Origin point
	spoint center = MakePoint(x, y);

	//Makes sure the x and y placement doesn't move with scaling.
	x = x * (1.0f / f->scale);
	y = y * (1.0f / f->scale);

	//Is there adjustment needed?
	if (f->spOffset.x != 0 || f->spOffset.y != 0)
	{
		center.x += f->spOffset.x;
		center.y += f->spOffset.y;
	}

	//Set Origin to Center of Texture

	float mx = (float)f->Width / 2;
	float my = (float)f->Height / 2;

	MinX = x - mx;
	MinY = y - my;
	MaxX = x + mx;
	MaxY = y + my;

	//Scaling
	MinX *= f->scale;
	MinY *= f->scale;
	MaxX *= f->scale;
	MaxY *= f->scale;

	
	//Calculate texture coords for this sprite
	//This could be pre-calculated.
	s = (float)f->x / sheetWidth;
	t = (float) 1.0f - (f->y / (float)sheetHeight);
	w = (float)f->Width / sheetWidth;
	h = (float)f->Height / (float)sheetHeight;

	//If we are printing at an angle, there are a lot more calculations.
	//I havent figured out an elegant way to shorten this yet. So this is a quick fix.
	if (f->angle)
	{
		spoint p0 = RotateAroundPoint(MinX, MinY, center.x, center.y, _cos[f->angle], _sin[f->angle]);
		spoint p1 = RotateAroundPoint(MaxX, MinY, center.x, center.y, _cos[f->angle], _sin[f->angle]);
		spoint p2 = RotateAroundPoint(MaxX, MaxY, center.x, center.y, _cos[f->angle], _sin[f->angle]);
		spoint p3 = RotateAroundPoint(MinX, MaxY, center.x, center.y, _cos[f->angle], _sin[f->angle]);

		//V0
        txlist.emplace_back(p0.x, p0.y, s, t - h, fcolor);
		//V1
		txlist.emplace_back(p1.x, p1.y, s + w, t - h, fcolor);
		//V2
		txlist.emplace_back(p2.x, p2.y, s + w, t, fcolor);
		//V2
		txlist.emplace_back(p2.x, p2.y, s + w, t, fcolor);
		//V3
		txlist.emplace_back(p3.x, p3.y, s, t, fcolor);
		//V0
		txlist.emplace_back(p0.x, p0.y, s, t - h, fcolor);
	}
	else
	{
		//V0 0,0
		txlist.emplace_back(MinX, MinY, s, t - h, fcolor);
		//V1 1,0
		txlist.emplace_back(MaxX, MinY, s + w, t - h, fcolor);
		//V2 1,1
		txlist.emplace_back(MaxX, MaxY, s + w, t, fcolor);
		//V2 1,1
		txlist.emplace_back(MaxX, MaxY, s + w, t, fcolor);
		//V3 0,1
		txlist.emplace_back(MinX, MaxY, s, t, fcolor);
		//V0 0,0
		txlist.emplace_back(MinX, MinY, s, t - h, fcolor);
	}
}


std::string Sprite::numToName(int spritenum)
{
	try 
	{
		auto it = Sprites.at(spritenum);      // vector::at throws an out-of-range
		return it.name;
	}
	catch (const std::out_of_range &err)
	{
		wrlog("Out of Range error: get spritename: %s \n", err.what());
	}
	return ("notfound");
}


int Sprite::nameToNum(std::string name)
{
	auto it = Sprites.begin();
	while (it != Sprites.end())
	{
		if (name == it->name) { return it - Sprites.begin(); }
		++it;
	}

	//	std::map<int,SpriteDescriptor> Sprites;
	//C++ Lambda Expression
	//
	//std::find_if(Sprites.begin(), Sprites.end(), [name](Sprite::SpriteDescriptor const& n1){  return n1.name == name;});
	wrlog("Warning: Sprite number requested not found, returning 0");
	return 0;
}


/**
Apply a sprite Origin offset for rotation/placement purposes.
**/
void Sprite::setOriginOffset(float x, float y, int spritenum)
{
	if (spritenum < count + 1)
	{
		Sprites[spritenum].spOffset.x = x;
		Sprites[spritenum].spOffset.y = y;
	}
}


void Sprite::setOriginOffset(spoint a, int spritenum)
{
	if (spritenum < count + 1)
	{
		Sprites[spritenum].spOffset.x = a.x;
		Sprites[spritenum].spOffset.y = a.y;
	}
}


void Sprite::SetScale(int spritenum, float newscale)
{
	if (spritenum < count + 1)
	{
		if (newscale > 0.1f && newscale < 10.0f)
		{
			Sprites[spritenum].scale = newscale;
		}
	}
}


void Sprite::setRotationAngle(int spritenum, int sangle)
{
	if (spritenum < count + 1)
	{
		if (sangle > 359 || sangle < -359) sangle = 0;
		Sprites[spritenum].angle = sangle;
	}
}


void Sprite::Add(float x, float y, int spritenum)
{
	SetScale(spritenum, 1.0f);
	InternalAdd(x, y, spritenum);
}


void Sprite::Add(float scale, float x, float y, int spritenum)
{

	SetScale(spritenum, scale);
	InternalAdd(x, y, spritenum);
}


void Sprite::Add(float x, float y, rgb_t color, float scale, int spritenum)
{
	SetScale(spritenum, scale);
	SetColor(color);
	InternalAdd(x, y, spritenum);
}


void Sprite::Add(float x, float y, rgb_t color, float scale, int angle, int spritenum)
{
	setRotationAngle(spritenum, angle);
	SetScale(spritenum, scale);
	SetColor(color);
	InternalAdd(x, y, spritenum);
}


Sprite::~Sprite()
{
	Sprites.clear();
	txlist.clear();
	//FreeTexture(ftexid->texid);
	wrlog("Sprite Library Instance Freed");
}


Sprite::spoint Sprite::RotateAroundPoint(float x, float y, float cx, float cy, float cosTheta, float sinTheta)
{
	spoint p;
	p.x = ((((x)-(cx)) * (cosTheta)) - (((y)-(cy)) * (sinTheta))) + (cx);
	p.y = ((((x)-(cx)) * (sinTheta)) + (((y)-(cy)) * (cosTheta))) + (cy);
	return p;
}


void Sprite::setPath(const char *path)
{
	datapath.assign(path);
}

void Sprite::setPath(std::string &path)
{
	datapath.assign(path);
}


std::string Sprite::getPath()
{
	return datapath;
}