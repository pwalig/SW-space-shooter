# SW-space-shooter

3D space shooter video game running on Raspberry Pi

# Hardware

* Raspbery Pi 4B
* [Waveshare 2.4 inch LCD](https://www.waveshare.com/2.4inch-lcd-module.htm)
* MCP3008 analog to digital converter
* 2 x joystick
* 2 x button

# Setup

## Connect hardware

Connect [Waveshare 2.4 inch LCD](https://www.waveshare.com/2.4inch-lcd-module.htm) according to manufacturer documentation: https://www.waveshare.com/wiki/2.4inch_LCD_Module

Connect MCP3008 to Raspberry Pi SPI ports (MISO, MOSI SCLK, SPI CE1)

Connect joystick and buttons to MCP3008 ports:

| port | device |
| --- | --- |
| 0 | right thumbstick button |
| 1 | right thumbstick X |
| 2 | right thumbstick Y |
| 3 | right button |
| 4 | left thumbstick X |
| 5 | left thumbstick Y |
| 6 | left button |
| 7 | - |

All buttons connected via 10K pullup resistor

## Enable SPI interface

```
sudo raspi-config
# Choose Interfacing Options -> SPI -> Yes to enable the SPI interface.
sudo reboot
```

# Software

Get your copy from [release section](https://github.com/pwalig/SW-space-shooter/releases) or build from source.

## Building from source

Project depends on [WiringPi](https://github.com/WiringPi/WiringPi)

version 3.12 was used in development

```
wget https://github.com/WiringPi/WiringPi/releases/download/3.12/wiringpi_3.12_arm64.deb
sudo dpkg -i wiringpi_3.12_arm64.deb
```

[CMake](https://cmake.org/) build system is used to build this software

```
sudo apt-get install cmake
```

In order to build the software run:

```
git clone https://github.com/pwalig/SW-space-shooter.git
cd SW-space-shooter
mkdir build
cd build
cmake ..
make
```

## Running

Before running create folder where space-shooter can save top scores:

```
mkdir /home/space-shooter
```

Then run the program:

```
sudo ./space-shooter
```

# Gameplay

Survive as long as you can and shoot as many enemy spaceships as possible.

**Be mindful. You have limited ammunition and shield time.**

Enemy spaceships spawn radomly around the player and drop powerups when destroyed.

## Awailable powerups

* GREEN: +10 hp
* GREY: +10 ammo
* BLUE: +0.5s of shield time
* YELLOW: +1 to total score

Enemies have base hp of 30.  
Player starts with 100 hp 40 bullets and 1.5s of shield time.  
Each bullet deals 10 hp of damage.  
Each destroyed enemy adds 1 point to total score.

Whene spaceships deal damage equal to their remaining hp on contact with the player, enemies destroyed int this way will not count to total score nor will they drop powerups.

Ammunition regenerates automatically (+5 bullets every 10 seconds).

## Controls

* Left thumbstick Y axis - forwards / backwards movement
* Left thumbstick X axis - roll spaceship
* Right thumbstick  - look around
* Left button - shoot
* Right button - use shield
* Hold right thumbstick button - give up and go to main menu

## UI

In top left corner there are 3 bars:

* RED - hp
* WHITE - ammo
* BLUE - shield

When right thumbstick button is held white bar at bottom left will appear.
