#include "Basket.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>

using namespace glm;

float Basket::size = 30.0f;
Basket::Basket(vec3 pos) {
    r = size * 0.1f; // from obj file
    basket = new Drawable("models/basket.obj");
    position = pos;
}

Basket::~Basket() {
    delete basket;
}

void Basket::draw(unsigned int drawable) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    basket->bind();
    basket->draw();
    glEnable(GL_CULL_FACE);
}

void Basket::update(float t, float dt) {
    mat4 translate = glm::translate(mat4(), position);
    mat4 scale = glm::scale(mat4(), vec3(size, 20.0f, size));
    mat4 rotate = glm::rotate(mat4(), radians(0.0f), vec3(0, 1, 0));
    modelMatrix = rotate * translate * scale;
}