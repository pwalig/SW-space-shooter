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
#include "game/powerups.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


UWORD *BlackImage;

float shutdownHold = 0.0f;

void freeProgram(){
    game::saveScores();
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

    int subSteps = 4;

    game::loadScores();

    ren::setP(game::player.cam.get_P());

    auto start = std::chrono::steady_clock::now();
    for (;;){
        Paint_Clear(BLACK);

        std::chrono::duration<float, std::chrono::seconds::period> dur(std::chrono::steady_clock::now() - start);
        float deltaTime = dur.count();
        start = std::chrono::steady_clock::now();
        float subDelta = deltaTime / (float)subSteps;
        game::timeIn = std::min(game::timeIn + deltaTime, 300.0f);

        // game logic
        if (game::state == game::State::playing) {

            game::enemySpaceship::randomSpawn(game::player.rb.position(), deltaTime, 1.0f / 10.0f);

            for (int i = 0; i < subSteps; ++i) {

                game::player.update(subDelta);
                if (game::state != game::State::playing) continue;

                game::enemySpaceship::updateAll(subDelta);
                game::projectile::updateAll(subDelta);
                game::powerups::pickUp();
                game::spaceshipProjectileCollisions();
                if (game::state != game::State::playing) continue;
            }

            game::powerups::update(deltaTime);
            game::bckg::star::update(
                game::player.rb.position(),
                glm::vec3(250.0f, 250.0f, 250.0f),
                (int)(glm::length(game::player.rb.velocity()) / 5.0f)
            );


            ren::setV(game::player.cam.get_V());
            game::enemySpaceship::drawAll();
            game::projectile::drawAll();
            game::powerups::draw();
            game::bckg::star::draw();

            // ui

            // cross hair
            Paint_QuickDrawLine(155, 115, 165, 125, WHITE);
            Paint_QuickDrawLine(165, 115, 155, 125, WHITE);

            // hp
            Paint_QuickDrawLine(0, 1, game::player.hp, 1, RED);
            Paint_QuickDrawLine(0, 2, game::player.hp, 2, RED);
            Paint_QuickDrawLine(0, 3, game::player.hp, 3, RED);

            // ammo
            Paint_QuickDrawLine(0, 7, game::player.ammo, 7, WHITE);
            Paint_QuickDrawLine(0, 8, game::player.ammo, 8, WHITE);
            Paint_QuickDrawLine(0, 9, game::player.ammo, 9, WHITE);

            // shield
            Paint_QuickDrawLine(0, 13, game::player.shield * 20.0f, 13, BLUE);
            Paint_QuickDrawLine(0, 14, game::player.shield * 20.0f, 14, BLUE);
            Paint_QuickDrawLine(0, 15, game::player.shield * 20.0f, 15, BLUE);

            // self destruct
            Paint_QuickDrawLine(0, 236, game::player.selfDestruct * 20.0f, 236, WHITE);
            Paint_QuickDrawLine(0, 237, game::player.selfDestruct * 20.0f, 237, WHITE);
            Paint_QuickDrawLine(0, 238, game::player.selfDestruct * 20.0f, 238, WHITE);

            Paint_DrawString_EN(0, 19, ("score: " + std::to_string(game::score)).c_str(), &Font12, BLACK, WHITE);

        } else {

            Paint_DrawString_EN(0, 0, "Top Scores:", &Font16, BLACK, WHITE);
            int scorey = 0;
            for (auto& score : game::scores) {
                scorey += 15;
                Paint_DrawString_EN(0, scorey, (score.second + " : " + std::to_string(score.first)).c_str(), &Font12, BLACK, WHITE);
            }

            if (game::state == game::State::over) {
                Paint_DrawString_EN(200, 40, "GAME OVER", &Font16, BLACK, WHITE);
                Paint_DrawString_EN(200, 80, ("your score: " + std::to_string(game::score)).c_str(), &Font12, BLACK, WHITE);
                if (game::score == game::scores[0].first)
                    Paint_DrawString_EN(200, 120, "new high score!", &Font12, BLACK, WHITE);
            }

            if (game::timeIn > 2.0f) {
                if (input::getButtonPressed(input::leftButton)) game::start();
                if (input::getButtonPressed(input::rightButton)) break;
                if (input::getButtonHeld(input::rightJBT)) {
                    shutdownHold += deltaTime;
                    if (shutdownHold >= 3.0f) {
                        freeProgram();
                        system("shutdown now");
                    }
                }
                else shutdownHold = 0.0f;

                if (game::state == game::State::over)
                    Paint_DrawString_EN(20, 185, "press left trigger to try again", &Font12, BLACK, WHITE);
                else Paint_DrawString_EN(20, 185, "press left trigger to play", &Font12, BLACK, WHITE);

                Paint_DrawString_EN(20, 200, "press right trigger to quit", &Font12, BLACK, WHITE);
                Paint_DrawString_EN(20, 215, "hold right joystick to shutdown", &Font12, BLACK, WHITE);
            }
            

            // hold
            Paint_QuickDrawLine(0, 236, shutdownHold * 20.0f, 236, WHITE);
            Paint_QuickDrawLine(0, 237, shutdownHold * 20.0f, 237, WHITE);
            Paint_QuickDrawLine(0, 238, shutdownHold * 20.0f, 238, WHITE);
        }

        input::cacheButtonState();
        LCD_2IN4_Display((UBYTE *)BlackImage);
    }

    freeProgram();
    return 0;
}
