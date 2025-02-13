#include "DisplayField.h"

IntegerDisplayField::IntegerDisplayField() {
    savedValue = 0;
}

bool IntegerDisplayField::ValueHasChanged(int newValue) {
    if(savedValue != newValue) {
        savedValue = newValue;
        return true;
    }
    return false;
}

FloatDisplayField::FloatDisplayField() {
    savedValue = 0;
}

bool FloatDisplayField::ValueHasChanged(float newValue) {
    if((savedValue - newValue) >= 0.1 || (savedValue - newValue) <= -0.1) {
        savedValue = newValue;
        return true;
    }
    return false;
}

UnsignedLongDisplayField::UnsignedLongDisplayField() {
    savedValue = 0;
}

bool UnsignedLongDisplayField::ValueHasChanged(unsigned long newValue) {
    if(savedValue != newValue) {
        savedValue = newValue;
        return true;
    }
    return false;
}