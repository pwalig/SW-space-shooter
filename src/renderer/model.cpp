#include "model.hpp"

ren::line::line(float x1, float y1, float z1, float x2, float y2, float z2) :
    a(x1, y1, z1), b(x2, y2, z2) {}

ren::model::model(const std::vector<line>& lines_) : lines(lines_) {}

ren::model ren::model::cube = ren::model({
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