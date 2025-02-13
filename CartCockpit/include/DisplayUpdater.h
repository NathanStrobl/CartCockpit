#ifndef DISPLAYUPDATER
#define DISPLAYUPDATER
#define NUMBER_OF_SCREENS 5
class DisplayUpdater {
    public:
    DisplayUpdater(int initialSelection);
    void ChangeScreenSelection();
    int GetScreenSelection();
    bool ForceUpdateNeeded();
    void DisableForceUpdate();

    private:
    int currentScreen;
    bool forceUpdate;
};
#endif