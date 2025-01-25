#include "spaceship.hpp"

extern "C" {
    #include "GUI/GUI_Paint.h"
}
#include "../input/input.hpp"

game::spaceship::spaceship(const glm::vec3& position, const glm::quat& rotation, const ren::mesh& mesh_) :
rb(), m(ren::model(mesh_, glm::mat4(1.0f), WHITE)),
hp(100), radius(1.0f), speed(10.0f), rotationSpeed(10.0f)
{
    rb.position() = position;
    rb.rotation() = rotation;
    rb.angularDrag() = 1.0f;
    rb.drag() = 1.0f;
}

void game::spaceship::update(float deltaTime) {
    rb.update(deltaTime);
    m.M = rb.modelMatrix<glm::mat4>();
}


game::playerSpaceship::playerSpaceship(const glm::vec3& position, const glm::quat& rotation) :
spaceship(position, rotation, ren::mesh::empty), cam() {
    cam.set_V(position, rotation);
    cam.set_P(90.0f, 320.0f / 240.0f, 0.01f, 1000.0f);
}

void game::playerSpaceship::update(float deltaTime) {
    
    rb.addTorque(glm::vec3(
        input::getAxisState(input::rightY),
        input::getAxisState(input::rightX),
        -input::getAxisState(input::leftX)
    ) * deltaTime * rotationSpeed);
    rb.addForce(rb.rotation() * glm::vec3(0.0f, 0.0f, 1.0f) * -input::getAxisState(input::leftY) * speed);

    this->spaceship::update(deltaTime);
    cam.set_V(rb.position(), rb.rotation());
}