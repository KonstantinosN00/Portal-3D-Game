#include "Sphere.h"
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>
#include "Player.h"

using namespace glm;

float Sphere::radius = 0.6f;
Sphere::Sphere(vec3 pos, vec3 vel)
    : RigidBody() {
    
    sphere = new Drawable("models/sphere.obj");
    r = radius;
    m = 1.0f;
    x = pos;
    v = vel;
    P = m * v;

    if (r == 0) throw std::logic_error("Sphere: radius != 0");
    mat3 I = mat3(
        2.0f / 5 * m*radius*radius, 0, 0,
        0, 2.0f / 5 * m*radius*radius, 0,
        0, 0, 2.0f / 5 * m*radius*radius);
    I_inv = inverse(I);
    forcing = [&](float t2, const vector<float>& y)->vector<float> {

        vector<float> f(6, 0.0f);
        f[1] = -m * 9.81;
        return f;
    };
}

Sphere::~Sphere() {
    delete sphere;
}

void Sphere::draw(unsigned int drawable) {
    sphere->bind();
    sphere->draw();
}

void Sphere::update(float t, float dt) {
    float d1 = abs(dot(x- Player::portals[0]->position, Player::portals[0]->normal));
    float d2 = abs(dot(x- Player::portals[1]->position, Player::portals[1]->normal));
    if (d1 > r && d2 > r) available = true;
    forcing = [&](float t2, const vector<float>& y)->vector<float> {

        vector<float> f(6, 0.0f);
        f[0] = -0.3f * v.x;
        f[1] = -m * 9.81 - 0.3f*v.y;
        f[2] = -0.3f * v.z;

        return f;
    };
    //integration
    advanceState(t, dt);
    P = m * v;
    // compute model matrix
    mat4 scale = glm::scale(mat4(), vec3(r, r, r));
    mat4 tranlation = translate(mat4(), vec3(x.x, x.y, x.z));
#ifdef USE_QUATERNIONS
    mat4 rotation = mat4_cast(q);
#else
    mat4 rotation = mat4(R);
#endif
    modelMatrix = tranlation * rotation * scale;
}


