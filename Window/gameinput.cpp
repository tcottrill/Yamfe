#include "tglfw.h"
 //Include for "key variable"
#include "gameinput.h"
#include "config.h"
#include "audio.h"
//#include "menu.h"
#include "inptport.h"
#include "log.h"

#include "globals.h"

extern volatile int logic_counter;

extern void savreset(void);
extern void run_game(void);
extern void show_double_buffer();

extern int num_items;
extern int num_values;

int KeyCheck(int keynum)
{
   static int keys[256];
 
   static int hasrun=0;
   int i;

   if (hasrun==0) //INIT
   {
      for ( i=0; i<256; i++)
	  {keys[i]=0;}
       hasrun=1;
   }
   if (!keys[keynum]&&key[keynum]) //Return True if not in que
   {
      keys[keynum]=1;
      return 1;
   }
   else if (keys[keynum]&&!key[keynum]) //Return False if in que
      keys[keynum]=0;
   return 0;
}

void get_input()
{
	static int lastup=0;
	static int lastdn=0;
	static int up=0;
	static int down=0;
	
   
   if (KeyCheck(fesetup.kmenu))  {showvol ^=1;}

   //Check for exit, or are we just using those keys to close something?

   //if (KeyCheck(fesetup.exit)    && have_error!=0)
   
 //  if (KeyCheck(KEY_ESC)) {end_game=1;}
   
 
 
   if ( readinputport(0)&0x01) {run_game();} //Up
   if ( readinputport(0)&0x02) {end_game++;} 
   
   // Keys are flipped for my cabinet, right and left are up and down since I 
   if ( readinputport(1)&0x01) {savreset();  keyin--; audio_play(A_MOVE,64,0);} //Up
   if ( readinputport(1)&0x02) {savreset();  keyin++; audio_play(A_MOVE,64,0);} //Down
   if ( readinputport(1)&0x04) {savreset();  keyin-=fesetup.gameskip; audio_play(A_MOVE,64,0);} //Left
   if ( readinputport(1)&0x08) {savreset();  keyin+=fesetup.gameskip; audio_play(A_MOVE,64,0);} //Right
  
					
}


void proc_input()
{
if (keyin > num_games) {keyin=1;}
if (keyin < 1) {keyin=num_games;}
}
