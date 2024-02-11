#include "DisplayUpdater.h"

DisplayUpdater::DisplayUpdater(int initialSelection) {
    currentScreen = initialSelection;
    forceUpdate = true;
}

void DisplayUpdater::ChangeScreenSelection() {
  currentScreen++;
  forceUpdate = true;

  //keeps currentScreen variable within bounds
  if(currentScreen >= NUMBER_OF_SCREENS) {
      currentScreen = 0;
  }
}

int DisplayUpdater::GetScreenSelection() {
    return currentScreen;
}

bool DisplayUpdater::ForceUpdateNeeded() {
    return forceUpdate;
}

void DisplayUpdater::DisableForceUpdate() {
  forceUpdate = false;
}
