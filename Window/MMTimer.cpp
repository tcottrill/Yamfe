// TimerEvent.cpp: implementation of the CTimerEvent class.
//
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "MMTimer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#pragma comment( lib, "winmm.lib") //timeGetTime

CTimerEvent::CTimerEvent()
{
        m_uTimerID=NULL;

}

CTimerEvent::~CTimerEvent()
{

        Kill();
}


BOOL CTimerEvent::Kill()
{

        MMRESULT mmResult;
    if (NULL==m_uTimerID) {
                return TRUE;
        }
        
        mmResult=timeKillEvent(m_uTimerID); 
        timeEndPeriod(1);
        if (TIMERR_NOERROR==mmResult) {
                m_uTimerID=NULL;
                return TRUE;
        }
        else {
                return FALSE;
        }

}

//Timer event type. The following values are defined: 

//TIME_ONESHOT 

//Event occurs once, after uDelay milliseconds. 

//TIME_PERIODIC 

BOOL CTimerEvent::Start(UINT uDelay, UINT uEventType)
{

        if (TIMERR_NOERROR==timeBeginPeriod(1)) {
                m_uTimerID=timeSetEvent(uDelay,0,TimerProcedure,(DWORD)this,uEventType); 
                if (NULL==m_uTimerID) {
                        timeEndPeriod(1);
                        return FALSE;
                }
                else {
                        return TRUE;
                }

        }
        else {
                return FALSE;
        }

}

void CALLBACK CTimerEvent::TimerProcedure(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CTimerEvent* pTimerObject=(CTimerEvent*)dwUser;
	pTimerObject->TimerFunction();
}


