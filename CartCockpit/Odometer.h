#ifndef ODOMETER
#define ODOMETER
class Odometer {
    public:
    Odometer();
    void Increment(unsigned long meters);
    unsigned long GetRawOdometerValue();
    float GetConvertedOdometerValue(float distanceConversion);

    protected:
    unsigned long odometerMeters;
};
#endif