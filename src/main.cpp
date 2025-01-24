extern "C" {
    #include "examples/test.h"
    #include <wiringPi.h>
    #include <wiringPiSPI.h>
    #include <mcp3004.h>
}
#include <math.h>
#include <stdlib.h>     //exit()
#include <stdio.h>
#include "game/game.hpp"


// setup pin for SPI communication
void mcp3008_setup(){
    mcp3004Setup(100, 1);
    //wiringPiSPISetup(1, 4*1000*1000);
}

// read a channel
int mcp3008_read(unsigned short adcnum)
{ 
    return analogRead(100 + adcnum);
}


int main(int argc, char *argv[])
{
    if (argc != 2){
        printf("please input LCD type!\r\n");
        printf("example: sudo ./main -1.3\r\n");
        exit(1);
    }
	
	double size;
    sscanf(argv[1],"%lf",&size);
    size = (fabs(size));
	
	if(size<0.1||size>10){
		printf("error: bad size\r\n");
		exit(1);
	}
	else printf("%.2lf inch LCD Moudule\r\n",size);
	
    foo(34);

    
	if(size==0.96)LCD_0IN96_test();
	else if(size==1.14)LCD_1IN14_test();
	else if(size==1.28)LCD_1IN28_test();
	else if(size==1.3)LCD_1IN3_test();
	else if(size==1.54)LCD_1IN54_test();
	else if(size==1.8)LCD_1IN8_test();
	else if(size==2)LCD_2IN_test();
	else if(size==2.4)LCD_2IN4_test();
	else	printf("error: can not find the LCD\r\n");

    mcp3008_setup();
    delay(50);
    for (;;){
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", mcp3008_read(0), mcp3008_read(1), mcp3008_read(2), mcp3008_read(3), mcp3008_read(4), mcp3008_read(5), mcp3008_read(6), mcp3008_read(7));
        delay(50);
    }

    return 0;
}
