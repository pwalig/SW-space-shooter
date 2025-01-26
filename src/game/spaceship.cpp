#include "spaceship.hpp"

extern "C" {
    #include "GUI/GUI_Paint.h"
}
#include "../input/input.hpp"
#include "projectile.hpp"
#include <random>

game::spaceship::spaceship(const glm::vec3& position, const glm::quat& rotation, const ren::mesh * mesh_) :
rb(), m(ren::model(mesh_, glm::mat4(1.0f), WHITE)),
hp(100), radius(1.0f), speed(10.0f), rotationSpeed(5.0f)
{
    rb.position() = position;
    rb.rotation() = rotation;
    rb.angularDrag() = 5.0f;
    rb.drag() = 1.0f;
}

void game::spaceship::update(float deltaTime) {
    rb.update(deltaTime);
    m.M = rb.modelMatrix<glm::mat4>();
    if (cooldown > 0.0f) cooldown -= deltaTime;
}

void game::spaceship::shoot() {
    if (cooldown <= 0.0f && ammo > 0) {
        projectile::spawnNew(rb.position() + rb.rotation() * glm::vec3(0.0f, 1.1f, 0.0f), rb.rotation(), 50.0f, 10.0f);
        cooldown = 0.5f;
        ammo -= 1;
    }
}

// player spaceship

game::playerSpaceship::playerSpaceship(const glm::vec3& position, const glm::quat& rotation) :
spaceship(position, rotation, &ren::mesh::empty), cam() {
    cam.set_V(position, rotation * glm::quat(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f)));
    cam.set_P(90.0f, 320.0f / 240.0f, 0.01f, 1000.0f);
    hp = 200;
}

void game::playerSpaceship::update(float deltaTime) {
    
    rb.addTorque(glm::vec3(
        input::getAxisState(input::rightY),
        input::getAxisState(input::leftX),
        -input::getAxisState(input::rightX)
    ) * rotationSpeed);
    rb.addForce(rb.rotation() * glm::vec3(0.0f, 1.0f, 0.0f) * -input::getAxisState(input::leftY) * speed);

    this->spaceship::update(deltaTime);
    cam.set_V(rb.position(), rb.rotation() * glm::quat(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f)));

    if (input::getButtonPressed(input::leftButton) && ammo > 0) {
        shoot();
    }
}

// enemy spaceship

std::vector<game::enemySpaceship> game::enemySpaceship::all;

std::random_device rds;
std::mt19937 gens(rds());

game::enemySpaceship::enemySpaceship(const glm::vec3& position, const glm::quat& rotation) :
spaceship(position, rotation, &ren::mesh::prism) {
    m.color = RED;
    hp = 30;
}

void game::enemySpaceship::update(float deltaTime) {

    rb.addTorque(glm::normalize(
        glm::cross(rb.rotation() * glm::vec3(0.0f, 1.0f, 0.0f), player.rb.position() - rb.position())
    ) * rotationSpeed);

    if (glm::dot(rb.rotation() * glm::vec3(0.0f, 1.0f, 0.0f), player.rb.position() - rb.position()) >= 0.0f) {
        rb.addForce(rb.rotation() * glm::vec3(0.0f, 1.0f, 0.0f) * speed);
        shoot();
        ammo = 1;
    }

    this->spaceship::update(deltaTime);
}

void game::enemySpaceship::updateAll(float deltaTime) {
    for (enemySpaceship& es : all) es.update(deltaTime);
}

void game::enemySpaceship::drawAll() {
    for (const enemySpaceship& es : all) es.m.draw();
}

void game::enemySpaceship::randomSpawn(const glm::vec3& position, float deltaTime, float rate) {
    std::uniform_real_distribution<> chance(0.0f, 1.0f);
    std::uniform_real_distribution<> distrib(-100.0f, 100.0f);
    std::uniform_real_distribution<> rdistrib(0.0f, glm::pi<float>());
    float p = deltaTime * rate;
    if (chance(gens) <= p){
        glm::vec3 pos = position + glm::vec3(distrib(gens), distrib(gens), distrib(gens));
        all.push_back(enemySpaceship(pos, glm::quat(glm::vec3(rdistrib(gens), rdistrib(gens), rdistrib(gens)))));
    }
}

game::playerSpaceship game::player(glm::vec3(0.0f, 1.0f, -10.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
