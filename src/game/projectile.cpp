#include "projectile.hpp"

extern "C" {
    #include "GUI/GUI_Paint.h"
}

std::deque<game::projectile> game::projectile::all;

void game::projectile::spawnNew(const glm::vec3& origin, const glm::quat& rotation, float speed, int damage){
    all.push_back(projectile(origin, rotation, speed, damage));
}

void game::projectile::updateAll(float deltaTime){

    int toRemove = 0;

    for (projectile& proj : all) {
        proj.lifetime -= deltaTime;
        if (proj.lifetime <= 0.0f) ++toRemove;
        proj.rb.update(deltaTime);
        proj.m.M = glm::scale(proj.rb.modelMatrix<glm::mat4>(), glm::vec3(0.2f));
    }

    all.erase(all.begin(), all.begin() + toRemove);
}

void game::projectile::drawAll() {
    for (const projectile& proj : all) {
        proj.m.draw();
    }
}


glm::vec3 game::projectile::position() { return rb.position(); }

int game::projectile::damage() { return _damage; }

game::projectile::projectile(const glm::vec3& origin, const glm::quat& rotation, float speed, int damage_) :
m(&ren::mesh::prism, glm::mat4(1.0f), MAGENTA), lifetime(5.0f), _damage(damage_) {
    rb.position() = origin;
    rb.rotation() = rotation;
    rb.velocity() = rotation * glm::vec3(0.0f, 1.0f, 0.0f) * speed;
    rb.drag() = 0.0f;
    m.M = glm::scale(rb.modelMatrix<glm::mat4>(), glm::vec3(0.2f));
}
