#include "game.hpp"
#include <iostream>
#include "spaceship.hpp"
#include "projectile.hpp"
#include "background.hpp"

int game::score = 0;
game::State game::state = game::State::playing;

void game::gameOver() {
    enemySpaceship::all.clear();
    projectile::all.clear();
    bckg::star::clear();
    player = playerSpaceship(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
    state = State::over;
}

void game::start() {
    score = 0;
    player = playerSpaceship(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
    enemySpaceship::randomSpawn(player.rb.position(), 1.0f, 1.0f);
    bckg::star::scatter(player.rb.position(), glm::vec3(250.0f, 250.0f, 250.0f), 40);

    state = State::playing;
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

        if (glm::length(player.rb.position() - es.rb.position()) < player.radius + (es.radius / 2.0f)) {
            spaceshipsToRemove.push_back(esid);
            esid--;

            if (!player.shielded) {
                player.hp -= es.hp;
                if (player.hp <= 0) {
                    gameOver();
                    return;
                }
            }
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

            if (!player.shielded) {
                player.hp -= proj.damage();
                if (player.hp <= 0) {
                    gameOver();
                    return;
                }
            }
        }
        projid++;
    }
    for (int id : projectilesToRemove) {
        projectile::all.erase(projectile::all.begin() + id);
    }
}

void game::loadScores() {

}