#ifndef __lights
#define __lights

//lights status glyph definitons
#define LIGHTS_OFF_AUTO 0
#define LIGHTS_ON_AUTO 1
#define LIGHTS_OFF_MANUAL 2
#define LIGHTS_ON_MANUAL 3

class Lights {
    public:
    Lights();
    void SetLights(bool setting);
    void DisableAutomaticLights();
    void AutomaticLightsCheck(int photocellReading);
    bool AutomaticLightsAreEnabled();
    int GetLightsStatus();

    private:
    bool automaticLightsEnabled;
    bool lightsOn;
};
#endif