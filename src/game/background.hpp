#pragma once
#include "../renderer/model.hpp"

namespace game{
    namespace bckg {
        class star {
        public:
            star(const glm::vec3& position);
            
            static void scatter(const glm::vec3& position, const glm::vec3& extent, int amount);
            static void update(const glm::vec3& position, const glm::vec3& extent, int amount);
            static void draw();

        private:
            ren::model m;
        };

    }
}