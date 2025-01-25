#include "spaceship.hpp"

extern "C" {
    #include "GUI/GUI_Paint.h"
}

game::spaceship::spaceship(const glm::vec3& position, const glm::quat& rotation) :
rb(), m(ren::model(ren::mesh::prism, glm::mat4(1.0f), WHITE)),
hp(100), radius(1.0f)
{
    rb.position() = position;
    rb.rotation() = rotation;
}

void game::spaceship::update(float deltaTime) {
    rb.update(deltaTime);
    m.M = rb.modelMatrix<glm::mat4>();
}


game::playerSpaceship::playerSpaceship(const glm::vec3& position, const glm::quat& rotation) :
spaceship(position, rotation), cam() {
    cam.set_V(position, rotation);
    cam.set_P(70.0f, 320.0f / 240.0f, 0.01f, 1000.0f);
}

void game::playerSpaceship::update(float deltaTime) {
    
    this->spaceship::update(deltaTime);
    cam.set_V(rb.position(), rb.rotation());
}