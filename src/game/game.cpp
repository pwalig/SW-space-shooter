#include "game.hpp"
#include <iostream>
#include "spaceship.hpp"
#include "projectile.hpp"

int game::score = 0;

void game::spaceshipProjectileCollisions() {
    int esid = 0;
    //int projid = 0;

    std::vector<int> spaceshipsToRemove;
    //std::vector<int> projectilesToRemove;

    for (enemySpaceship& es : enemySpaceship::all) {
        for (projectile& proj : projectile::all) {
            if (glm::length(proj.position() - es.rb.position()) < es.radius){
                //projectilesToRemove.push_back(projid);
                //--projid;

                es.hp -= proj.damage();
                if (es.hp <= 0) {
                    spaceshipsToRemove.push_back(esid);
                    esid--;
                    score++;
                }
            }
            //projid++;
        }
        esid++;
    }

    
    for (int id : spaceshipsToRemove) {
        enemySpaceship::all.erase(enemySpaceship::all.begin() + id);
    }
    /*
    for (int id : projectilesToRemove) {
        projectile::all.erase(projectile::all.begin() + id);
    }
    */
}