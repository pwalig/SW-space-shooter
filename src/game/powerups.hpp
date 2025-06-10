#pragma once
#include <pe/rigidbody.hpp>
#include <vector>

namespace game {
    struct powerup {
        pe::rigidbody3D rb;
    };

    namespace powerups {

        void spawnNew(const glm::vec3& position, const glm::quat& rotation);
        int pickUp(std::vector<powerup>& pups);
        void pickUp();
        void update(std::vector<powerup>& pups, float deltaTime);
        void update(float deltaTime);
        void draw(std::vector<powerup>& pups, uint16_t color);
        void draw();
        void clear();
    }
}