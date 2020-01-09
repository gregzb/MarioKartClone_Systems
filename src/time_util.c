#include <time.h>
#include "time_util.h"

double get_delta_time(struct timespec init_time) {
  struct timespec current_time;
  clock_gettime(CLOCK_MONOTONIC, &current_time);

  struct timespec delta_timespec;
  sub_timespec(init_time, current_time, &delta_timespec);
  return delta_timespec.tv_sec + delta_timespec.tv_nsec/1000000000.0;
}

enum { NS_PER_SECOND = 1000000000 };

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td) {
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec  = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}
