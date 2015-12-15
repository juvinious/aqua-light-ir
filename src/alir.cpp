#include "alir.h"

using namespace AquaLightIR;
    
unsigned int INCREMENT_STEP = 25;
int INCREMENT_MIN = 0;
int INCREMENT_MAX = 250;


void Config::setIncrementStep(unsigned int value){
    INCREMENT_STEP = value;
}

unsigned int Config::getIncrementStep() {
    return INCREMENT_STEP;
}

void Config::setIncrementMinimum(int value) {
    INCREMENT_MIN = value;
}

int Config::getIncrementMinimum() {
    return INCREMENT_MIN;
}

void Config::setIncrementMaximum(int value){
    INCREMENT_MAX = value;
}

int Config::getIncrementMaximum() {
    return INCREMENT_MAX;
}
