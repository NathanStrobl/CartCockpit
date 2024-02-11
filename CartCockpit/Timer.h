#include "SystemTime.h"
#ifndef TIMER
#define TIMER
class Timer {
    public:
    Timer(SystemTime* system);
    void ResetTimer();
    int GetTimeSince();

    private:
    unsigned long thisTimerValue;
    SystemTime* systemTime;
};
#endif