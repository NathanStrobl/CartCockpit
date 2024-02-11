#include "BatteryMonitoringSystem.h"

BatteryMonitoringSystem::BatteryMonitoringSystem(int savedAverageChargeDistance, int initialBatterySensorReading) {
    averageChargeDistance = savedAverageChargeDistance;
    UpdateBatterySensorReading(initialBatterySensorReading);
    UpdatePeriodicVoltageValue();
    initialStateOfCharge = GetStateOfCharge();
}

void BatteryMonitoringSystem::UpdateBatterySensorReading(int newReading) {
    batterySensorReading = newReading;
}

void BatteryMonitoringSystem::UpdatePeriodicVoltageValue() {
    periodicallyUpdatedVoltageValue = GetRealtimeVoltage();
}

unsigned int BatteryMonitoringSystem::RecordSprintDataAndGetNewAverageChargeDistance(float metersTraveled, float chargePercentageUsed) {
  averageChargeDistance = (metersTraveled / 5) - ((averageChargeDistance * chargePercentageUsed) / 500) + averageChargeDistance;
  return averageChargeDistance;
}

float BatteryMonitoringSystem::GetRealtimeVoltage() {
    return ((12.0 * batterySensorReading) / (1024.0/5.0)); //divides analogRead value by 204.8 to get true voltage, then multiplies value by 12; assumes that R1 = 22kΩ and R2 = 1kΩ in voltage divider
}

int BatteryMonitoringSystem::GetPWRValue() {
    return GetValidPercentage((periodicallyUpdatedVoltageValue - GetRealtimeVoltage()) / (highAndLowVoltageDifference * 2));
}

float BatteryMonitoringSystem::GetConvertedRange(float distanceConverstion, int metersTraveledSinceLastStop) {
    return ((((GetStateOfCharge() / 100) * averageChargeDistance) - metersTraveledSinceLastStop) / distanceConverstion);
}

float BatteryMonitoringSystem::GetStateOfCharge() {
    return GetValidPercentage(((periodicallyUpdatedVoltageValue - LOW_VOLTAGE_VALUE) / highAndLowVoltageDifference));
}

int BatteryMonitoringSystem::GetChargeUsedSinceStart() {
    return (initialStateOfCharge - GetStateOfCharge());
}

float BatteryMonitoringSystem::GetConvertedAverageChargeDistance(float distanceConversion) {
    return (averageChargeDistance / distanceConversion);
}

int BatteryMonitoringSystem::GetBatteryHealth() {
    if(GetRealtimeVoltage() < 49) {
        return LOW_VOLTAGE;
    } else if(GetRealtimeVoltage() > 55) {
        return HIGH_VOLTAGE;
    } else if (averageChargeDistance < (LOW_RANGE_METERS * 2)) {
        return SERVICE_SOON;
    }
    return HEALTHY;
}

float BatteryMonitoringSystem::GetValidPercentage(float value) {
    float returnValue = (value * 100);
    if(returnValue < 0) {
        return 0;
    } else if (returnValue > 100) {
        return 100;
    }
    return returnValue;
}