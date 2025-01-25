#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace ren {
    struct line {
        line(float x1, float y1, float z1, float x2, float y2, float z2);
        glm::vec3 a;
        glm::vec3 b;
    };

    class mesh {
    public:
        mesh(const std::vector<line>& lines_);

        const std::vector<line> lines;

        static mesh cube;
        static mesh prism;
        static mesh star;
        static mesh empty;
    };

    class model {
        public:
        model(const mesh& mesh_, const glm::mat4& M_, uint16_t color_);

        glm::mat4 M;
        const mesh& _mesh;
        uint16_t color;

        void draw();
    };
}