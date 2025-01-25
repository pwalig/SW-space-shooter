#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <pe/rigidbody.hpp>
#include "../renderer/model.hpp"
#include "../renderer/camera.hpp"

namespace game {
    class spaceship {
    public:
        pe::rigidbody3D rb;
        ren::model m;
        int hp;
        float radius;
        float speed;
        float rotationSpeed;

        spaceship(const glm::vec3& position, const glm::quat& rotation, const ren::mesh& mesh_);

        virtual void update(float deltaTime);
    };

    class playerSpaceship : public spaceship{
    public:
        ren::camera cam;
        playerSpaceship(const glm::vec3& position, const glm::quat& rotation);
        void update(float deltaTime) override;
    };
}