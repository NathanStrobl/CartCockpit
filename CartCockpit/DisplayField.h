#ifndef DISPLAYFIELD
#define DISPLAYFIELD
class IntegerDisplayField {
    public:
    IntegerDisplayField();
    bool ValueHasChanged(int newValue);

    private:
    int savedValue;
};

class FloatDisplayField {
    public:
    FloatDisplayField();
    bool ValueHasChanged(float newValue);

    private:
    float savedValue;
};

class UnsignedLongDisplayField {
    public:
    UnsignedLongDisplayField();
    bool ValueHasChanged(unsigned long newValue);

    private:
    unsigned long savedValue;
};
#endif