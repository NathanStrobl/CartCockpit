#ifndef SYSTEMTIME
#define SYSTEMTIME
class SystemTime {
    public:
    SystemTime();
    void UpdateSystemTime(unsigned long millis);
    unsigned long GetSystemTime();
    int GetSystemTimeInMinutes();
    
    private:
    unsigned long systemTime;
};
#endif