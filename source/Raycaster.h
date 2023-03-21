#ifndef RAYCASTER_H
#define RAYCASTER_H
#include <glm/glm.hpp>
#include <glfw3.h>
#include "common/model.h"
using namespace glm;

class Raycaster {

public:
	Drawable* box;
	vec3 ray;
	vec3 position;
	vec3 closest_intersection;
	vec3 closest_norm;

	Raycaster(Drawable* b, vec3 pos, vec3 dir);
	void update(vec3 pos, vec3 dir);
	void calculateFaceNormals();
	void scaleBox();
};

#endif