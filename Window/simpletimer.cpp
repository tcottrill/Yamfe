/////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "simpletimer.h"
#include <vector>

//OLD SCHOOL

#pragma comment( lib, "winmm.lib")

std::vector<UINT> timer_item(0x0f);

static bool initialized = false;

//void WINAPI onTimeFunc (UINT wTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2); 

//timeBeginPeriod(1)     timeEndPeriod(1);


 void install_timer_ex( void *callback, long time)
 {
	 if (!initialized)
	 {
	   timeBeginPeriod(1);
	   initialized = true;
	 }
	 timer_item.front() = timeSetEvent (time, 1, (LPTIMECALLBACK) callback, DWORD (1), TIME_PERIODIC); 
 }


 void stop_timer(UINT timer)
 {
 

 }

 
 void stop_all_timers()
 {
	 if (timer_item.size())
	 {
	   for (unsigned i=0; i < timer_item.size(); i++)
       timeKillEvent (timer_item[i]); 
	 }
	 timer_item.clear();
	 
	 if (initialized){ timeEndPeriod(1); initialized = false;}
 }

