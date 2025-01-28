#include "powerups.hpp"
#include <random>
#include "spaceship.hpp"
#include "../renderer/renderer.hpp"
#include "game.hpp"

extern "C" {
    #include "GUI/GUI_Paint.h"
}

std::random_device rdp;
std::mt19937 genp(rdp());

std::uniform_int_distribution<> pdistrib(0, 5);

std::vector<game::powerup> health;
std::vector<game::powerup> ammo;
std::vector<game::powerup> shield;
std::vector<game::powerup> scoreP;

void game::powerups::spawnNew(const glm::vec3& position, const glm::quat& rotation) {
    powerup p;
    p.rb.position() = position;
    p.rb.rotation() = rotation;
    p.rb.angularVelocity() = rotation * glm::vec3(0.0f, 1.0f, 0.0f);
    int type = pdistrib(genp);

    if (type <= 1) health.push_back(p);
    else if (type <= 3) ammo.push_back(p);
    else if (type == 4) shield.push_back(p);
    else if (type == 5) scoreP.push_back(p);
}

int game::powerups::pickUp(std::vector<powerup>& pups) {
    int id = 0;
    std::vector<int> toRemove;

    for (auto& p : pups) {
        if (glm::length(player.rb.position() - p.rb.position()) < player.radius * 2.0f) {
            toRemove.push_back(id);
            --id;
        }
        ++id;
    }

    for (int i : toRemove) pups.erase(pups.begin() + i);

    return toRemove.size();
}

void game::powerups::pickUp() {
    player.hp = std::min (player.hp + pickUp(health) * 10, 200);
    player.ammo = std::min (player.ammo + pickUp(ammo) * 10, 200);
    player.shield = std::min (player.shield + pickUp(shield) * 0.5f, 10.0f);
    game::score += pickUp(scoreP);
}

void game::powerups::update(std::vector<powerup>& pups, float deltaTime) {
    for (auto& p : pups) p.rb.update(deltaTime);
}

void game::powerups::update(float deltaTime) {
    update(health, deltaTime);
    update(ammo, deltaTime);
    update(shield, deltaTime);
    update(scoreP, deltaTime);
}

void game::powerups::draw(std::vector<powerup>& pups, uint16_t color) {
    for (auto& p : pups) {
        ren::setM(p.rb.modelMatrix<glm::mat4>());
        ren::draw(ren::mesh::star.lines, color);
    }
}

void game::powerups::draw() {
    draw(health, GREEN);
    draw(ammo, GRAY);
    draw(shield, BLUE);
    draw(scoreP, YELLOW);
}