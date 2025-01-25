#include "model.hpp"
#include "renderer.hpp"

ren::line::line(float x1, float y1, float z1, float x2, float y2, float z2) :
    a(x1, y1, z1), b(x2, y2, z2) {}

ren::mesh::mesh(const std::vector<line>& lines_) : lines(lines_) {}

ren::mesh ren::mesh::cube = ren::mesh({
    ren::line(-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f),
    ren::line(-1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f),
    ren::line(-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f),

    ren::line(1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f),
    ren::line(1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f),
    ren::line(1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f),

    ren::line(-1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f),
    ren::line(-1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f),
    ren::line(-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),

    ren::line(1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f),
    ren::line(1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f),
    ren::line(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f)
});

ren::mesh ren::mesh::prism = ren::mesh({
    ren::line(-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f),
    ren::line(-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f),

    ren::line(1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f),
    ren::line(1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f),

    ren::line(0.0f, 1.0f, 0.0f, -1.0f, -1.0f, -1.0f),
    ren::line(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f),
    ren::line(0.0f, 1.0f, 0.0f, -1.0f, -1.0f, 1.0f),
    ren::line(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f)
});

ren::mesh ren::mesh::star = ren::mesh({
    ren::line(0.0f, 1.0f, 0.0f, 0.237764f, 0.327254f, 0.0f),
    ren::line(0.237764f, 0.327254f, 0.0f, 0.951057f, 0.309017f, 0.0f),
    ren::line(0.951057f, 0.309017f, 0.0f, 0.38471f, -0.125f, 0.0f),
    ren::line(0.38471f, -0.125f, 0.0f, 0.587785f, -0.809017f, 0.0f),
    ren::line(0.587785f, -0.809017f, 0.0f, 0.0f, -0.404509f, 0.0f),
    ren::line(0.0f, -0.404509f, 0.0f, -0.587785f, -0.809017f, 0.0f),
    ren::line(-0.587785f, -0.809017f, 0.0f, -0.38471f, -0.125f, 0.0f),
    ren::line(-0.38471f, -0.125f, 0.0f, -0.951057f, 0.309017f, 0.0f),
    ren::line(-0.951057f, 0.309017f, 0.0f, -0.237764f, 0.327254f, 0.0f),
    ren::line(-0.237764f, 0.327254f, 0.0f, 0.0f, 1.0f, 0.0f),
});

ren::model::model(const mesh& mesh_, const glm::mat4& M_, uint16_t color_) : _mesh(mesh_), M(M_), color(color_) {}

void ren::model::draw() {
    ren::setM(M);
    ren::draw(_mesh.lines, color);
}