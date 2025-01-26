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
#include "game/projectile.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


UWORD *BlackImage;

void freeProgram(){
	LCD_2IN4_Clear(BLACK);
	LCD_SetBacklight(0);
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

    ren::setP(game::player.cam.get_P());

    int subSteps = 4;

    game::enemySpaceship::randomSpawn(game::player.rb.position(), 1.0f, 1.0f);
    game::bckg::star::scatter(game::player.rb.position(), glm::vec3(250.0f, 250.0f, 250.0f), 40);

    auto start = std::chrono::steady_clock::now();
    for (;;){
        std::chrono::duration<float, std::chrono::seconds::period> dur(std::chrono::steady_clock::now() - start);
        float deltaTime = dur.count();
        start = std::chrono::steady_clock::now();
        float subDelta = deltaTime / (float)subSteps;

        // game logic
        if (game::state == game::State::playing) {
            game::enemySpaceship::randomSpawn(game::player.rb.position(), deltaTime, 1.0f / 10.0f);

            for (int i = 0; i < subSteps; ++i) {

                game::player.update(subDelta);

                game::enemySpaceship::updateAll(subDelta);
                game::projectile::updateAll(subDelta);
                game::spaceshipProjectileCollisions();
            }

            game::bckg::star::update(
                game::player.rb.position(),
                glm::vec3(250.0f, 250.0f, 250.0f),
                (int)(glm::length(game::player.rb.velocity()) / 5.0f)
            );
        }
        

        // drawing
        Paint_Clear(BLACK);

        if (game::state == game::State::playing) {
            ren::setV(game::player.cam.get_V());
            game::enemySpaceship::drawAll();
            game::projectile::drawAll();
            game::bckg::star::draw();

            // ui

            // cross hair
            Paint_QuickDrawLine(155, 115, 165, 125, WHITE);
            Paint_QuickDrawLine(165, 115, 155, 125, WHITE);

            // hp
            Paint_QuickDrawLine(0, 1, game::player.hp / 2.0f, 1, RED);
            Paint_QuickDrawLine(0, 2, game::player.hp / 2.0f, 2, RED);
            Paint_QuickDrawLine(0, 3, game::player.hp / 2.0f, 3, RED);

            
            // ammo
            Paint_QuickDrawLine(0, 7, game::player.ammo, 7, WHITE);
            Paint_QuickDrawLine(0, 8, game::player.ammo, 8, WHITE);
            Paint_QuickDrawLine(0, 9, game::player.ammo, 9, WHITE);

            Paint_DrawString_EN(0, 13, ("score: " + std::to_string(game::score)).c_str(), &Font12, BLACK, WHITE);
        } else {
            Paint_DrawString_EN(50, 50, "GAME OVER", &Font16, BLACK, WHITE);
            Paint_DrawString_EN(50, 100, ("your score: " + std::to_string(game::score)).c_str(), &Font12, BLACK, WHITE);
        }

        LCD_2IN4_Display((UBYTE *)BlackImage);
    }

    freeProgram();
    return 0;
}
