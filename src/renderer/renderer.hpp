#pragma once
#include <glm/glm.hpp>
#include "model.hpp"

namespace ren {
    void setViewport(int xoffset, int yoffset, int width, int height);
    void setM(const glm::mat4& M);
    void setV(const glm::mat4& V);
    void setP(const glm::mat4& P);

    void draw(const line& l, uint16_t color);
    void draw(const std::vector<line>& lines, uint16_t color);
}