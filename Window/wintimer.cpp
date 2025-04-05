#include <windows.h>
#include <map>


#include <memory>
#include <thread>
#include <mutex>
#include "wintimer.h"

using namespace std;

struct TimerInfo
{
    TimerInfo(void (*proc)(), int speed)
      : proc(proc),
        speed(speed)
    {
        stopEvent = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
        stoppedEvent = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
    }


    ~TimerInfo()
    {
        CloseHandle(stopEvent);
        CloseHandle(stoppedEvent);
    }


    void (*proc)();
    int speed;
    HANDLE stopEvent;
    HANDLE stoppedEvent;
};


static map<void (*)(), shared_ptr<TimerInfo>> activeTimers;
static mutex activeTimersMutex;

__declspec(thread) static TimerInfo* currentThreadTimerInfo = nullptr;


static void ThreadProc(shared_ptr<TimerInfo> info)
{
    currentThreadTimerInfo = info.get();

    LARGE_INTEGER freq;
    LARGE_INTEGER lastTime;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&lastTime);

    LONGLONG timerSpeed = freq.QuadPart * info->speed / TIMERS_PER_SECOND;
    LONGLONG accumulatedTime = 0;
    
    for (;;)
    {
        // Check the current time.
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        accumulatedTime += currentTime.QuadPart - lastTime.QuadPart;
        lastTime = currentTime;

        if (accumulatedTime < timerSpeed)
        {
            // Wait until this timer is next due.
            DWORD milliseconds = (timerSpeed - accumulatedTime) * 1000 / freq.QuadPart;

            if (WaitForSingleObjectEx(info->stopEvent, milliseconds, false) == 0)
            {
                // Quit if our stop event is signalled.
                SetEvent(info->stoppedEvent);
                return;
            }
        }
        else
        {
            // Execute the timer callback.
            while (accumulatedTime >= timerSpeed)
            {
                accumulatedTime -= timerSpeed;
        
                info->proc();
            }
        }
    }
}


static void RemoveIntLockless(void (*proc)())
{
    // Find this timer thread.
    auto it = activeTimers.find(proc);

    if (it != activeTimers.end())
    {
        auto& info = it->second;

        // Tell the thread to exit.
        SetEvent(info->stopEvent);

        // Wait for it to exit, except if remove is called from the timer thread itself (deadlocks are bad).
        if (info.get() != currentThreadTimerInfo)
        {
            WaitForSingleObjectEx(info->stoppedEvent, INFINITE, false);
        }

        activeTimers.erase(it);
    }
}


void remove_int(void (*proc)())
{
    lock_guard<mutex> lock(activeTimersMutex);

    RemoveIntLockless(proc);
}


int install_int_ex(void (*proc)(), int speed)
{
    auto info = make_shared<TimerInfo>(proc, speed);
    
    // Worker lambda.
    auto workItemHandler = ref new WorkItemHandler([=](IAsyncAction^)
    {
        ThreadProc(info);

    }, CallbackContext::Any);

    lock_guard<mutex> lock(activeTimersMutex);

    // Remove any previous instance of this timer.
    RemoveIntLockless(proc);

    activeTimers[proc] = std::move(info);

    // Start the worker thread.
    ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);

    return 0;
}


int install_int(void (*proc)(), int speed)
{
    return install_int_ex(proc, MSEC_TO_TIMER(speed));
}

