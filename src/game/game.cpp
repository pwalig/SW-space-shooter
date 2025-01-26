#include "game.hpp"
#include <iostream>
#include "spaceship.hpp"
#include "projectile.hpp"
#include "background.hpp"
#include "powerups.hpp"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

template< typename T, typename Pred >
typename std::vector<T>::iterator
    insert_sorted( std::vector<T> & vec, T const& item, Pred pred )
{
    return vec.insert
        (
           std::upper_bound( vec.begin(), vec.end(), item, pred ),
           item
        );
}

int game::score = 0;
game::State game::state = game::State::menu;
float game::timeIn = 2.0f;

std::vector<std::pair<int, std::string>> game::scores;

void game::gameOver() {
    enemySpaceship::all.clear();
    projectile::all.clear();
    bckg::star::clear();

    // insert score
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d/%m/%Y-%H:%M:%S");
    std::pair<int, std::string> score(game::score, oss.str());
    if (scores.size() < 10 || score.first > scores.back().first) {
        insert_sorted(scores, score,
        [](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b) {return a > b;});
    }
    scores.resize(10);

    timeIn = 0.0f;
    state = State::over;
}

void game::start() {
    score = 0;
    player = playerSpaceship(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
    enemySpaceship::randomSpawn(player.rb.position(), 1.0f, 1.0f);
    bckg::star::scatter(player.rb.position(), glm::vec3(250.0f, 250.0f, 250.0f), 40);

    timeIn = 0.0f;
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
                    powerups::spawnNew(es.rb.position(), es.rb.rotation());
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
    std::ifstream fscores("/home/space-shooter/scores.txt");
    if (!fscores.is_open()) return;

    while (!fscores.eof() && scores.size() < 10) {
        std::pair<int, std::string> score;
        fscores >> score.second >> score.first;
        scores.push_back(score);
    }
}

void game::saveScores() {
    std::ofstream oscores("/home/space-shooter/scores.txt");

    for(auto& score : game::scores) {
        oscores << score.second << " " << score.first << "\n";
    }
}