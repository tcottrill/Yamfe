
#include "log.h"
#include "romscan.h"
#include "sys_fileio.h"
#include <iostream>
#include <streambuf>
#include <algorithm>
#include "globals.h"

std::vector<glist> gamelist;

// ToDo: only scan if file changed. Cache data?

struct membuf :
	std::streambuf
{
	membuf(char* b, char* e) { this->setg(b, b, e); }
};

//Lambda
void sort_games_new()
{
	std::sort(gamelist.begin(), gamelist.end(), [](glist const& a, glist const& b) {return a.glname < b.glname; });
}

void GameList_Initalize(const char* listname)
{
	// To do, get rid of the dangerous c arrays
	// Change this to std:aaray and use iterator http://msdn.microsoft.com/en-us/library/bb982615.aspx
	// Also http://stackoverflow.com/questions/10900345/stdarraychar-n-to-stdstring

	char tempin[1024]; //current word - note fixed size
	int index = 0;  //index into the word
	int ch = 0;     //current character
	int loc = 1; //current location in the

	char* buffer = charloadFile(listname);

	membuf sbuf(buffer, buffer + strlen(buffer));
	std::istream in(&sbuf);
	glist tlist;

	while (!(in.eof()))
	{
		ch = in.get();

		if (isalpha(ch) || isdigit(ch) && (ch != '"'))
		{
			tempin[0] = ch;
			index = 1;
			do {
				ch = in.get();
				if (index < 1024) { tempin[index++] = ch; }
			} while (!isspace(ch) && !(in.eof()));
			tempin[index - 1] = '\0';

			if (loc == 1) {
				tlist.exename.assign(tempin);
				loc = 2;
			}
			else if (loc == 2)
			{
				loc = 1;
				tlist.gsname.assign(tempin);
			}
		}

		else if (ch == '"' && !(in.eof()))

		{
			index = 0;
			do {
				ch = in.get();
				if (index < 1024) { tempin[index++] = ch; }
			} while (ch != '"' && !(in.eof()));
			//Avoid doing something stupid
			if (index) tempin[index - 1] = '\0';
			else tempin[0] = '\0';

			tlist.glname.assign(tempin);
			tlist.extopt.assign("none.");
			gamelist.push_back(tlist);
		}
	}

	in.clear();
	free(buffer);

	//////////////////////////////////////////
	num_games = gamelist.size() - 1;
	wrlog("num games = %d", num_games);

	sort_games_new();

	//This adds a number identifier for each game. This should be removed and rethought.
	for (int loop = 1; loop < (num_games + 1); loop++)
	{
		gamelist[loop].next = (loop + 1);
		gamelist[loop].prev = (loop - 1);
		if (loop == 1) { gamelist[loop].prev = num_games; }
		if (loop == num_games) { gamelist[loop].next = 1; }
		//wrlog("loop is %d,next is %d, prev is %d",loop,gamelist[loop].next,gamelist[loop].prev);
		//wrlog("Name here is %s",gamelist[loop].glname.c_str());
	}
	wrlog("romscan completed");
}

void GameList_ShutDown()
{
	gamelist.clear();
}