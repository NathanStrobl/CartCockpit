#include "Odometer.h"

Odometer::Odometer() {
    odometerMeters = 0; 
}

void Odometer::Increment(unsigned long meters) {
    odometerMeters += meters;
}

unsigned long Odometer::GetRawOdometerValue() {
  return odometerMeters;
}

float Odometer::GetConvertedOdometerValue(float distanceConversion) {
    return (odometerMeters / distanceConversion);
}