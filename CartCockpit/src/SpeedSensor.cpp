#include "SpeedSensor.h"

SpeedSensor::SpeedSensor(float wheelCircumfrenceInMeters, float magnetQuantity, int speedSensorIntervalMillis) {
    metersTraveledPerHit = (wheelCircumfrenceInMeters / magnetQuantity);
    speedSensorIntervalDurationInMilliseconds = speedSensorIntervalMillis;
    speedSensorHitsInCurrentInterval = 0;
    consecutiveStoppedIntervals = 0;
}

void SpeedSensor::RecordSpeedSensorHit() {
    speedSensorHitsInCurrentInterval++;
    consecutiveStoppedIntervals = 0;
}

float SpeedSensor::GetMetersTraveledDuringInterval() {
    float result = speedSensorHitsInCurrentInterval * metersTraveledPerHit;
    
    //counts how long the cart has been stationary for
    result == 0 ? consecutiveStoppedIntervals++ : consecutiveStoppedIntervals = 0;

    speedSensorHitsInCurrentInterval = 0;
    return result;
}

bool SpeedSensor::RunStoppedSequence() {
  return (consecutiveStoppedIntervals == 9);
}

bool SpeedSensor::WriteOdometerToEEPROM() {
  return (consecutiveStoppedIntervals == 1);
}

int SpeedSensor::GetSpeedIntervalDurationMillis() {
  return speedSensorIntervalDurationInMilliseconds;
}