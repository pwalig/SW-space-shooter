#pragma once

namespace input {
    void setup();

    extern const unsigned short leftX, leftY, leftJBT, leftButton, rightX, rightY, rightJBT, rightButton;

    int rawRead(unsigned short channel);
    float getAxisState(unsigned short channel);
    bool getButtonPressed(unsigned short channel);
    void print();
}