#ifndef ROMSCAN_H
#define ROMSCAN_H

#include <string>
#include <vector>


typedef struct {
      
    int next; // index of next entry in array
    int prev; // previous entry (if double-linked)    
	std::string exename;       //Name of exe to run the game as
	std::string gsname; 		//Short Name of game
    std::string glname;	    //Display name for Game  
	std::string extopt;   //Any extra options for each game  
} glist;                      //Only one gamelist at a time


extern std::vector<glist> gamelist;

extern void GameList_Initalize (const char *listname);
extern void GameList_ShutDown();


#endif