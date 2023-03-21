#include "Portal.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>


using namespace glm;

Portal::Portal(vec3 pos, float rot) {
	position = pos;
	rotation = 0.0f;
	rot_vector = vec3(0, 1.0f, 0);
	normal = planeNormals[0];
	up = vec3(0,1.0f,0);
	portal = new Drawable(planeVertices, planeUVs, planeNormals);\
}

Portal::~Portal() {
	delete portal;
}

void Portal::draw(unsigned int drawable) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	portal->bind();
	portal->draw();
	//glEnable(GL_CULL_FACE);
}

void Portal::update(float t, float dt) {
	mat4 translate = glm::translate(mat4(), position);
	mat4 rotate = glm::rotate(mat4(), rotation, rot_vector);
	modelMatrix = translate *rotate;
	//normal = vec3( modelMatrix*vec4(planeNormals[0],1.0f) );
}
/*
mat4 const Portal::clippedProjMat(glm::mat4 const& viewMat, glm::mat4 const& projMat) const
{
	float dist = glm::length(position);
	glm::vec4 clipPlane(position * glm::vec3(0.0f, 0.0f, -1.0f), dist);
	clipPlane = glm::inverse(glm::transpose(viewMat)) * clipPlane;

	if (clipPlane.w > 0.0f)
		return projMat;

	glm::vec4 q = glm::inverse(projMat) * glm::vec4(
		glm::sign(clipPlane.x),
		glm::sign(clipPlane.y),
		1.0f,
		1.0f
	);

	vec4 c = clipPlane * (2.0f / dot(clipPlane, q));

	mat4 newProj = projMat;
	// third row = clip plane - fourth row
	//newProj = row(newProj, 2, c - glm::row(newProj, 3));
	newProj[2] = c - newProj[3];

	return newProj;
}*/