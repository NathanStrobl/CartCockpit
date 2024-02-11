#include "Timer.h"

Timer::Timer(SystemTime* system) {
    systemTime = system;
    ResetTimer();
}

void Timer::ResetTimer() {
    thisTimerValue = systemTime -> GetSystemTime();
}

int Timer::GetTimeSince() {
    return (systemTime -> GetSystemTime() - thisTimerValue);
}