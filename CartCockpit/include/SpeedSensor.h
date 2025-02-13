#ifndef SPEEDSENSOR
#define SPEEDSENSOR
class SpeedSensor {
    public:
    SpeedSensor(float wheelCircumfrenceInMeters, float magnetQuantity, int speedSensorIntervalMillis);
    void RecordSpeedSensorHit();
    float GetMetersTraveledDuringInterval();
    bool RunStoppedSequence();
    bool WriteOdometerToEEPROM();
    int GetSpeedIntervalDurationMillis();

    private:
    int speedSensorHitsInCurrentInterval;
    int consecutiveStoppedIntervals;
    float metersTraveledPerHit;
    int speedSensorIntervalDurationInMilliseconds;
};
#endif