#include "SystemTime.h"

SystemTime::SystemTime() {
    systemTime = 0;
}

void SystemTime::UpdateSystemTime(unsigned long millis) {
    systemTime = millis;
}

unsigned long SystemTime::GetSystemTime() {
    return systemTime;
}

int SystemTime::GetSystemTimeInMinutes() {
    return (systemTime / 60000);
}