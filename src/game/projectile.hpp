#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../renderer/model.hpp"
#include <pe/rigidbody.hpp>
#include <deque>

namespace game {
    class projectile {
    public:
        static void spawnNew(const glm::vec3& origin, const glm::quat& rotation, float speed, int damage);
        static void updateAll(float deltaTime);
        static void drawAll();

    private:
        projectile(const glm::vec3& origin, const glm::quat& rotation, float speed, int damage);

        static std::deque<projectile> all;
        pe::rigidbody3D rb;
        ren::model m;
        float lifetime;
    };
}
