extern "C" {
    #include "Config/DEV_Config.h"
    #include "LCD/LCD_2inch4.h"
    #include "GUI/GUI_Paint.h"
    #include "GUI/GUI_BMP.h"
}
#include <math.h>
#include <stdlib.h>     //exit()
#include <stdio.h>
#include <signal.h>     //signal()
#include <chrono>
#include "game/game.hpp"
#include "game/spaceship.hpp"
#include "renderer/renderer.hpp"
#include "input/input.hpp"
#include "game/background.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


UWORD *BlackImage;

void freeProgram(){
	LCD_2IN4_Clear(BLACK);
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
	
    // /*1.Create a new image cache named IMAGE_RGB and fill it with black*/
    Paint_NewImage(BlackImage, LCD_2IN4_WIDTH, LCD_2IN4_HEIGHT, ROTATE_270, BLACK, 16);

    // setup mcp3008
    input::setup();

    ren::setViewport(0, 0, LCD_2IN4_HEIGHT, LCD_2IN4_WIDTH);

    ren::model m1(ren::mesh::prism, glm::mat4(1.0f), WHITE);
    ren::model m2(ren::mesh::cube, glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, 0.0f)), WHITE);
    ren::model m3(ren::mesh::star, glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f)), WHITE);
    game::spaceship spc(glm::vec3(0.0f, 0.0f, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), ren::mesh::prism);
    game::playerSpaceship pspc(glm::vec3(0.0f, 1.0f, -10.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
    ren::setP(pspc.cam.get_P());

    game::bckg::star::scatter(pspc.rb.position(), glm::vec3(250.0f, 250.0f, 250.0f), 40);

    auto start = std::chrono::steady_clock::now();
    for (;;){
        std::chrono::duration<float, std::chrono::seconds::period> dur(std::chrono::steady_clock::now() - start);
        float deltaTime = dur.count();
        start = std::chrono::steady_clock::now();

        // game logic
        m1.M = glm::rotate(m1.M, deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
        m2.M = glm::rotate(m2.M, deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
        m3.M = glm::rotate(m3.M, deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
        spc.rb.addTorque(glm::vec3(1.0f, 0.0f, 0.0f) * deltaTime);
        spc.rb.addForce(spc.rb.rotation() * glm::vec3(0.0f, 1.0f, 0.0f) * input::getAxisState(input::leftY));
        spc.update(deltaTime);
        pspc.update(deltaTime);
        game::bckg::star::update(pspc.rb.position(), glm::vec3(250.0f, 250.0f, 250.0f), (int)(glm::length(pspc.rb.velocity()) / 5.0f));

        // drawing
        Paint_Clear(BLACK);

        ren::setV(pspc.cam.get_V());

        m1.draw();
        m2.draw();
        m3.draw();
        spc.m.draw();
        game::bckg::star::draw();

        LCD_2IN4_Display((UBYTE *)BlackImage);
    }

    freeProgram();
    return 0;
}
