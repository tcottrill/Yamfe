// TimerEvent.h: interface for the CTimerEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEREVENT_H__733DF4C1_4095_11D1_8FE8_D6184AA51F0B__INCLUDED_)
#define AFX_TIMEREVENT_H__733DF4C1_4095_11D1_8FE8_D6184AA51F0B__INCLUDED_


//Multimidia functions  
#include <mmsystem.h>
#include "log.h"

class CTimerEvent  
{
public:
	BOOL Start(UINT uDelay, UINT uEventType);
	BOOL Kill();
	CTimerEvent();
	virtual ~CTimerEvent();

private:
	MMRESULT      m_uTimerID;
	static void   CALLBACK TimerProcedure(UINT uID,UINT uMsg,DWORD dwUser,DWORD dw1,DWORD dw2);
protected:
	virtual void  TimerFunction()=0;
};


class MMTimer:public CTimerEvent

{

   public:
	    MMTimer(){
			       callback_param=0;
				   callback=NULL;
		         }; 
	   ~MMTimer(){}; 

	   

	   void SetCallBack(void (*callback)(int), int param)
	   {
		   MMTimer::callback = callback;
		   MMTimer::callback_param = param;
	   }
     
      void TimerFunction()

      {
		  if (callback){(callback)(callback_param); }
		  else
              wrlog("Warning, timer called with no callback set!");

      }
private:

	 void (*callback)(int);
	 int callback_param;

};
#endif // !defined(AFX_TIMEREVENT_H__733DF4C1_4095_11D1_8FE8_D6184AA51F0B__INCLUDED_)
