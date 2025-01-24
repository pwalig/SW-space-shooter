#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <mcp3004.h>
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <time.h>

// compile with: gcc -Wall  mcp3008_test2.c -lwiringPi -o mcptest2

// setup pin for SPI communication
void mcp3008_setup(){
    mcp3004Setup(100, 1) ;
    //wiringPiSPISetup(1, 4*1000*1000);
}

// read a channel
int mcp3008_read(unsigned short adcnum)
{ 
    return analogRead(100 + adcnum);
} 
 
int main(void)
{
    // setup GPIO, this uses actual BCM pin numbers 
    wiringPiSetupGpio();
    mcp3008_setup();
    delay(50);
    for (;;){
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", mcp3008_read(0), mcp3008_read(1), mcp3008_read(2), mcp3008_read(3), mcp3008_read(4), mcp3008_read(5), mcp3008_read(6), mcp3008_read(7));
        delay(50);
    }
    
    return 0;
}