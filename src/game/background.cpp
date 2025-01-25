#include "background.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <deque>

extern "C" {
    #include "GUI/GUI_Paint.h"
}

std::random_device rd;
std::mt19937 gen(rd());

std::deque<game::bckg::star> allStars;

game::bckg::star::star(const glm::vec3& position) :
m(ren::mesh::pointStar, glm::translate(glm::mat4(1.0f), position), WHITE) {}


void game::bckg::star::scatter(const glm::vec3& position, const glm::vec3& extent, int amount) {
    std::uniform_real_distribution<> distribX(position.x - extent.x, position.y + extent.x);
    std::uniform_real_distribution<> distribY(position.y - extent.y, position.y + extent.y);
    std::uniform_real_distribution<> distribZ(position.z - extent.z, position.z + extent.z);
    for (int i = 0; i < amount; ++i) {
        glm::vec3 pos;
        pos.x = distribX(gen);
        pos.y = distribY(gen);
        pos.z = distribZ(gen);
        allStars.push_back(star(pos));
    }

}

void game::bckg::star::update(const glm::vec3& position, const glm::vec3& extent, int amount) {
    scatter(position, extent, amount);

    for (int i = 0; i < amount; ++i) {
        allStars.pop_front();
    }
}

void game::bckg::star::draw() {
    for (const star& s : allStars) s.m.draw();
}