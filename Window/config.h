#ifndef CONFIG_H
#define CONFIG_H
//


typedef struct {
	int kleft;
	int kright;
	int kup;
	int kdown;
	int kselect;
	int kexit;
	int kmenu;
	int kshell;
	int volume;
	int musicon;
	int msecdelay;
	int gameskip;
    int fontsize;
	int soundcard;
	int debug;
	int autorotate;
	int startdelay;
    
} FESETTINGS;

 extern FESETTINGS fesetup;

extern void config_init();
extern const char *savermess;            //Screen saver message value
extern const char *mamepath;
#endif