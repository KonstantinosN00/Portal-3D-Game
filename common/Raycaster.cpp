#include <GL/glew.h>
#include "./lab07/Player.h"
#include "./lab07/Box.h"

using namespace glm;

class Raycaster {
	const int RECURSION_COUNT = 100;
	const int RAY_RANGE = 30;
	mat4 viewMat;
	mat4 projMat;
	vec3 ray = vec3(0.0f,0.0f,0.0f);
	Raycaster::Raycaster(Player player,Box box) {
		viewMat = player.viewMatrix;

	}
	void Raycaster::update() {

	}










};