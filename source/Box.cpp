#include "Box.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>

using namespace glm;
float Box::scaleXZ = 10.0f;
float Box::scaleY = 11.0f;
Box::Box() {
    cube = new Drawable("models/court4.obj");
    maxx = 4.0f * scaleXZ;
    maxy = 2.8f * scaleY;
    maxz = 2.5f * scaleXZ;
    //for(int i=0;i<cube->vertices.size();i++) printf("%f %f %f\n", cube->vertices[i].x, cube->vertices[i].y, cube->vertices[i].z);
    //for(int i=0;i<cube->normals.size();i++) printf("%f %f %f\n", cube->normals[i].x, cube->normals[i].y, cube->normals[i].z);
}

Box::~Box() {
}

void Box::draw(unsigned int drawable) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    cube->bind();
    cube->draw();
    glEnable(GL_CULL_FACE);
}

void Box::update(float t, float dt) {
    mat4 translate = glm::translate(mat4(), vec3(0,0,0));
    mat4 scale = glm::scale(mat4(), vec3(scaleXZ,scaleY,scaleXZ));
    mat4 rotate = glm::rotate(mat4(),radians(0.0f),vec3(0,1,0));
    modelMatrix = scale ;
}