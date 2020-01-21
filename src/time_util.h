#ifndef TIME_UTIL_INCLUDE
#define TIME_UTIL_INCLUDE
#include <time.h>

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td);
double get_delta_time(struct timespec init_time);

#endif
