#include "Sky.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>

using namespace glm;
const float width = 500.0f;
const float depth = 500.0f;
const std::vector<vec3> planeVertices = {
	vec3(-width / 2, 0, -depth / 2),
	vec3(-width / 2, 0, depth / 2),
	vec3(width / 2,  0, depth / 2),
	vec3(width / 2,  0, depth / 2),
	vec3(width / 2, 0, -depth / 2),
	vec3(-width / 2, 0, -depth / 2),
};
// plane normals
const std::vector<vec3> planeNormals = {
	vec3(0.0f, 1.0f, 0),
	vec3(0.0f, 1.0f, 0),
	vec3(0.0f, 1.0f, 0),
	vec3(0.0f, 1.0f, 0),
	vec3(0.0f, 1.0f, 0),
	vec3(0.0f, 1.0f, 0)
};
// plane uvs
const std::vector<vec2> planeUVs = {
	vec2(0.0f, 0.0f),
	vec2(0.0f, 1.0f),
	vec2(1.0f, 1.0f),
	vec2(1.0f, 1.0f),
	vec2(1.0f, 0.0f),
	vec2(0.0f, 0.0f),
};
Sky::Sky() {
    sky = new Drawable(planeVertices,planeUVs,planeNormals);
    
}

Sky::~Sky() {
    delete sky;
}

void Sky::draw(unsigned int drawable) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    sky->bind();
    sky->draw();
}

void Sky::update() {
    mat4 translate = glm::translate(mat4(), vec3(0, 70.0f, 0));
    mat4 scale = glm::scale(mat4(), vec3(1,1,1));
    mat4 rotate = glm::rotate(mat4(), radians(180.0f), vec3(0, 0, 1));
    modelMatrix = translate*rotate;
}