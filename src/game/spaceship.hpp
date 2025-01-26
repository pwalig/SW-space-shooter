#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
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
        float cooldown;
        int ammo = 40;

        spaceship(const glm::vec3& position, const glm::quat& rotation, const ren::mesh * mesh_);

        virtual void update(float deltaTime);

    protected:
        void shoot();
    };

    class playerSpaceship : public spaceship{
    public:
        ren::camera cam;
        float selfDestruct = 0.0f;
        playerSpaceship(const glm::vec3& position, const glm::quat& rotation);
        void update(float deltaTime) override;
    };

    class enemySpaceship : public spaceship {
    public:
        void update(float deltaTime) override;

        static std::vector<enemySpaceship> all;
        static void updateAll(float deltaTime);
        static void drawAll();
        static void randomSpawn(const glm::vec3& position, float deltaTime, float rate);

    private:
        enemySpaceship(const glm::vec3& position, const glm::quat& rotation);
    };

    extern playerSpaceship player;
}