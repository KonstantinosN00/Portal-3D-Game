#ifndef PORTAL_H
#define PORTAL_H

#include <glm/glm.hpp>
#include <common/model.h>
#include <vector>
class Drawable;
using namespace glm;
class Portal {
public:
	Drawable* portal;
    bool visible = false;
    const float width = 6.0f;
    const float height = 8.0f;
    mat4 modelMatrix;
    vec3 position;
    vec3 normal;
    vec3 up;
    float rotation;
    vec3 rot_vector;
	const std::vector<vec3> planeVertices = {
		vec3(-width / 2, -height / 2, 0.0f),
		vec3(-width / 2, height / 2, 0.0f),
		vec3(width / 2,  height / 2, 0.0f),
		vec3(width / 2,  height / 2, 0.0f),
		vec3(width / 2, -height / 2, 0.0f),
		vec3(-width / 2, -height / 2, 0.0f),
	};
	// plane normals
	const std::vector<vec3> planeNormals = {
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.0f, 1.0f)
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
    Portal(glm::vec3 pos,float rot);
    ~Portal();
    void draw(unsigned int drawable = 0);
    void update(float t = 0, float dt = 0);

    //glm::mat4 const clippedProjMat(glm::mat4 const& viewMat, glm::mat4 const& projMat) const;



};







#endif