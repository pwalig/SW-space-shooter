#include "game.hpp"
#include <iostream>
#include "spaceship.hpp"
#include "projectile.hpp"

int game::score = 0;
game::State game::state = game::State::playing;

namespace game {
    void gameOver() {
        state = State::over;
    }
}


void game::spaceshipProjectileCollisions() {

    // collisions with enemies
    int esid = 0;
    std::vector<int> spaceshipsToRemove;

    for (enemySpaceship& es : enemySpaceship::all) {

        int projid = 0;
        std::vector<int> projectilesToRemove;

        for (projectile& proj : projectile::all) {
            if (glm::length(proj.position() - es.rb.position()) < es.radius){
                projectilesToRemove.push_back(projid);
                --projid;

                es.hp -= proj.damage();
                if (es.hp <= 0) {
                    spaceshipsToRemove.push_back(esid);
                    esid--;
                    score++;
                }
            }
            projid++;
        }
    
        for (int id : projectilesToRemove) {
            projectile::all.erase(projectile::all.begin() + id);
        }

        if (glm::length(player.rb.position() - es.rb.position()) < player.radius + es.radius) {
            player.hp -= es.hp;
            if (player.hp <= 0) {
                gameOver();
                return;
            }
            spaceshipsToRemove.push_back(esid);
            esid--;
        }

        esid++;
    }

    for (int id : spaceshipsToRemove) {
        enemySpaceship::all.erase(enemySpaceship::all.begin() + id);
    }

    // collisions with player
    
    int projid = 0;
    std::vector<int> projectilesToRemove;
    for (projectile& proj : projectile::all) {
        if (glm::length(proj.position() - player.rb.position()) < player.radius){
            projectilesToRemove.push_back(projid);
            --projid;

            player.hp -= proj.damage();
            if (player.hp <= 0) {
                gameOver();
                return;
            }
        }
        projid++;
    }
    for (int id : projectilesToRemove) {
        projectile::all.erase(projectile::all.begin() + id);
    }
}