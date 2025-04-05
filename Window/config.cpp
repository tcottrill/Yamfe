#include "config.h"
//#include "animate.h"
#include "romscan.h"
#include "ini.h"
extern int rotation;
extern int spinvert;

const char *mamepath="NONE";             //Mame path variable
const char *savermess;
extern int stimeout;
extern int num_games;

FESETTINGS fesetup;

void config_init()
{   
	int loop=0;
	const char *tempin;
    const char *test;
	char *test2=NULL;

	set_config_file("./aafe.ini");
	fesetup.kleft = get_config_int("input", "keyleft", 1);
	fesetup.kright = get_config_int("input", "keyright", 1);
	fesetup.kup = get_config_int("input", "keyup", 1);
	fesetup.kdown = get_config_int("input", "keydown", 1);
	fesetup.kexit = get_config_int("input", "keyexit", 1);
	fesetup.kselect = get_config_int("input", "keyselect", 1);
	fesetup.kmenu = get_config_int("input", "keymenu", 1);
	fesetup.kshell = get_config_int("input", "keyshell", 1);
	fesetup.volume = get_config_int("main", "volume", 80);
    fesetup.msecdelay = get_config_int("input", "keydelay", 20);
	fesetup.gameskip = get_config_int("input", "gameskip", 5);
    fesetup.fontsize = get_config_int("main", "fontsize", 15);
	fesetup.debug = get_config_int("main", "debug", 0);
	spinvert = get_config_int("input", "minvert", 0);
	//num_stars = ReadInteger("stars", "number", 1);
    //max_star_speed = ReadInteger("stars", "maxspeed", 5);
	mamepath = get_config_string("main","mamedir", "NONE");
	fesetup.startdelay = get_config_int("main", "startdelay", 0);
    savermess= get_config_string("main","smessage", "ARCADE");
	stimeout = get_config_int("main", "stimeout", 5);
	test= get_config_string("main","rotate", "LEFT");
    //wrlog("STRING HERE %s", strupr(get_config_string("main","rotate", "LEFT")));
	fesetup.autorotate = get_config_int("main", "autorotategames", 1);
	test2=(char *) malloc(sizeof(test) * strlen(test));
	strcpy (test2,test);
	test2 = _strupr(test2);
	if (strcmp(test2,"NONE")==0) {rotation=0;}
	if (strcmp(test2,"RIGHT")==0){rotation=2;}
	if (strcmp(test2,"LEFT") ==0){rotation=1;}
	if (strcmp(test2,"FLIP") ==0){rotation=3;}

	free (test2);

	for (loop = 1; loop  < (num_games+1); loop++) //(num_games+1);
	{
	//allegro_message("name %s",gamelist[loop].gsname);
		tempin= get_config_string("extopts",(char *) gamelist[loop].gsname.c_str(), "EMPTY");
	 if (strcmp( tempin, "EMPTY" )!=0){
		//gamelist[loop].extopt = (char *) malloc( strlen(tempin) * sizeof(char));
		//strcpy(gamelist[loop].extopt,tempin);
		 gamelist[loop].extopt.assign(tempin);
		 //wrlog("Log Opened %s",gamelist[loop].extopt);
		//allegro_message("name %s",gamelist[loop].extopt);
	   }
	 else if (strcmp( tempin, "EMPTY" )==0){
	   // gamelist[loop].extopt = (char *) malloc( strlen(tempin) * sizeof(char));
	//	strcpy(gamelist[loop].extopt,tempin);
		 gamelist[loop].extopt.assign(tempin);
	   }
	 	
	}
 
}
