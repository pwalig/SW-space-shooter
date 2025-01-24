#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace ren {
    struct line {
        line(float x1, float y1, float z1, float x2, float y2, float z2);
        glm::vec3 a;
        glm::vec3 b;
    };

    class model {
    public:
        model(const std::vector<line>& lines_);

        std::vector<line> lines;

        static model cube;
    };
}