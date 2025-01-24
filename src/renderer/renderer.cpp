#include "renderer.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

extern "C" {
    #include "GUI/GUI_Paint.h"
}

glm::ivec2 viewportDims = glm::ivec2(100, 100);
glm::ivec2 viewportOffset = glm::ivec2(0, 0);

glm::mat4 M = glm::mat4(1.0f);
glm::mat4 V = glm::lookAt(glm::vec3(0.0f, -10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 P = glm::perspective(glm::radians(70.0f), 16.0f / 9.0f, 0.1f, 100.0f);

void ren::setViewport(int xoffset, int yoffset, int width, int height){
    viewportDims.x = width;
    viewportDims.y = height;
    viewportOffset.x = xoffset;
    viewportOffset.y = yoffset;
}

void ren::setM(const glm::mat4& M_) {M = M_;}
void ren::setV(const glm::mat4& V_) {V = V_;}
void ren::setP(const glm::mat4& P_) {P = P_;}

glm::ivec2 toWindowSpace(const glm::vec3& vertex) {
    glm::vec4 res = P * V * M * glm::vec4(vertex, 1.0f);
    res /= res.w;

    return glm::ivec2(
        ( res.x + 1 ) * viewportDims.x * 0.5f + viewportOffset.x,
        ( -res.y + 1 ) * viewportDims.y * 0.5f + viewportOffset.y
    );
}

void ren::draw(const line& l, uint16_t color) {
    glm::ivec2 resa = toWindowSpace(l.a);
    glm::ivec2 resb = toWindowSpace(l.b);
    Paint_QuickDrawLine(resa.x, resa.y, resb.x, resb.y, color);  
}
void ren::draw(const std::vector<line>& lines, uint16_t color) {
    for (const line& l : lines) draw(l, color);
}