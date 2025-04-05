
#pragma once

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>

#pragma warning (disable: 4244 4305 4996)

#ifdef __cplusplus
extern "C" {
#endif

// Emulated Allegro timer methods.
int install_int(void (*proc)(), int speed);
int install_int_ex(void (*proc)(), int speed);
void remove_int(void (*proc)());

#define TIMERS_PER_SECOND   10000000
#define SECS_TO_TIMER(x)    ((x) * TIMERS_PER_SECOND)
#define MSEC_TO_TIMER(x)    ((x) * (TIMERS_PER_SECOND / 1000))
#define BPS_TO_TIMER(x)     (TIMERS_PER_SECOND / (x))


#ifdef __cplusplus

}
#endif  // __cplusplus