#ifndef BMS
#define BMS

//battery health definitions
#define HEALTHY 0
#define LOW_VOLTAGE 1
#define HIGH_VOLTAGE 2
#define SERVICE_SOON 3
#define LOW_RANGE_METERS 5000

//battery voltage definitions
#define LOW_VOLTAGE_VALUE 49
#define HIGH_VOLTAGE_VALUE 52

class BatteryMonitoringSystem {
    public:
    BatteryMonitoringSystem(int savedAverageChargeDistance, int initialBatterySensorReading);
    void UpdateBatterySensorReading(int newReading);
    void UpdatePeriodicVoltageValue();
    unsigned int RecordSprintDataAndGetNewAverageChargeDistance(float metersTraveled, float chargePercentageUsed);
    float GetRealtimeVoltage();
    int GetPWRValue();
    float GetConvertedRange(float distanceConversion, int metersTraveledSinceLastStop);
    float GetStateOfCharge();
    int GetChargeUsedSinceStart();
    float GetConvertedAverageChargeDistance(float distanceConversion);
    int GetBatteryHealth();

    private:
    int batterySensorReading;
    unsigned int averageChargeDistance;
    int initialStateOfCharge;
    float periodicallyUpdatedVoltageValue;
    float GetValidPercentage(float value);
    const float highAndLowVoltageDifference = (HIGH_VOLTAGE_VALUE - LOW_VOLTAGE_VALUE);
};
#endif