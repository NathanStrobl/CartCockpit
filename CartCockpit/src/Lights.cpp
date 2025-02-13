#include "Lights.h"

Lights::Lights() {
    automaticLightsEnabled = true;
    lightsOn = false;
}

void Lights::SetLights(bool setting) {
    lightsOn = setting;
}

void Lights::DisableAutomaticLights() {
    automaticLightsEnabled = false;
}

void Lights::AutomaticLightsCheck(int photocellReading) {
    SetLights(photocellReading < 850);
}

bool Lights::AutomaticLightsAreEnabled() {
  return automaticLightsEnabled;
}

int Lights::GetLightsStatus() {
    if(automaticLightsEnabled && !lightsOn) {
        return LIGHTS_OFF_AUTO;
    } else if(automaticLightsEnabled && lightsOn) {
        return LIGHTS_ON_AUTO;
    } else if(!automaticLightsEnabled == !lightsOn) {
        return LIGHTS_OFF_MANUAL;
    }
    return LIGHTS_ON_MANUAL;
}