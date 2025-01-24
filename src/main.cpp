extern "C" {
    #include "Config/DEV_Config.h"
    #include "LCD/LCD_2inch4.h"
    #include "GUI/GUI_Paint.h"
    #include "GUI/GUI_BMP.h"
    #include <wiringPi.h>
    #include <wiringPiSPI.h>
    #include <mcp3004.h>
}
#include <math.h>
#include <stdlib.h>     //exit()
#include <stdio.h>
#include <signal.h>     //signal()
#include <chrono>
#include "game/game.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


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


UWORD *BlackImage;

void freeProgram(){
    if (BlackImage != NULL) {
        free(BlackImage);
        BlackImage = NULL;
    }
    DEV_ModuleExit();
    DEBUG("succesfull clean\n");
}

void ctrl_c_handler(int signo){
    freeProgram();
    exit(0);
}


int main(int argc, char *argv[])
{
    // Exception handling:ctrl + c
    signal(SIGINT, ctrl_c_handler);
    
    /* Module Init */
	if(DEV_ModuleInit() != 0){
        DEV_ModuleExit();
        exit(0);
    }
	
    /* LCD Init */
	LCD_2IN4_Init();
	LCD_2IN4_Clear(BLACK);
	LCD_SetBacklight(1010);
	
    UDOUBLE Imagesize = LCD_2IN4_HEIGHT*LCD_2IN4_WIDTH*2;
    if((BlackImage = (UWORD *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        DEV_ModuleExit();
        exit(0);
    }
	
    // /*1.Create a new image cache named IMAGE_RGB and fill it with white*/
    Paint_NewImage(BlackImage, LCD_2IN4_WIDTH, LCD_2IN4_HEIGHT, ROTATE_270, BLACK, 16);

    // setup mcp3008
    mcp3008_setup();
    delay(50);


    unsigned int i = 0;
    glm::vec4 a(0.0f, -50.0f, 0.0f, 1.0f);
    glm::vec4 b(0.0f, 50.0f, 0.0f, 1.0f);
    glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3(290.0f, 25.0f, 0.0f));
    auto start = std::chrono::steady_clock::now();
    for (;;){
        std::chrono::duration<float, std::chrono::seconds::period> dur(std::chrono::steady_clock::now() - start);
        float deltaTime = dur.count();
        start = std::chrono::steady_clock::now();

        // game logic
        //printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", mcp3008_read(0), mcp3008_read(1), mcp3008_read(2), mcp3008_read(3), mcp3008_read(4), mcp3008_read(5), mcp3008_read(6), mcp3008_read(7));
        M = glm::rotate(M, (float)deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));

        // drawing
        Paint_Clear(BLACK);
        glm::vec4 resa = M * a;
        glm::vec4 resb = M * b;
        Paint_QuickDrawLine(resa.x, resa.y, resb.x, resb.y, WHITE);  
        LCD_2IN4_Display((UBYTE *)BlackImage);
    }

    freeProgram();
    return 0;
}
