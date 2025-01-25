#include "input.hpp"

extern "C" {
    #include <wiringPi.h>
    #include <wiringPiSPI.h>
    #include <mcp3004.h>
}
#include <algorithm>
#include <iostream>

const unsigned short input::leftX = 4;
const unsigned short input::leftY = 5;
const unsigned short input::rightX = 1;
const unsigned short input::rightY = 2;
const unsigned short input::leftJBT = 3;
const unsigned short input::rightJBT = 0;
const unsigned short input::leftButton = 6;
const unsigned short input::rightButton = 7;

void input::setup(){
    mcp3004Setup(100, 1);
}

int input::rawRead(unsigned short channel) {
    return analogRead(100 + channel);
}

float input::getAxisState(unsigned short channel){
    int raw = analogRead(100 + channel) - 512;
    float out = (float)raw / 512.0f;
    if (std::abs(out) < 0.1f) return 0.0f;

    return out;
}

bool input::getButtonPressed(unsigned short channel){
    int raw = analogRead(100 + channel);
    return raw < 400;
}

void input::print() {
    printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
    rawRead(0), rawRead(1), rawRead(2), rawRead(3), rawRead(4), rawRead(5), rawRead(6), rawRead(7));
}