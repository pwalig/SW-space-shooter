#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace game {
    class spaceship {
    public:
        glm::vec3 posiiton;
        glm::quat rotation;
        float velocity;
    };
}