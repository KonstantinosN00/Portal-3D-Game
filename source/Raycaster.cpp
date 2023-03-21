#include <GL/glew.h>
#include "Box.h"
#include "Raycaster.h"
#include <common/model.h>

#define verts box->vertices
#define norms box->normals
using namespace glm;

Raycaster::Raycaster(Drawable* b, vec3 pos, vec3 dir) {
	box = b;
	scaleBox();
	position = pos;
	ray = dir;
	//vertices in drawable object are sorted so that a sequence of 3 vectors make a triangle
	// so we will check ray to plane collision for every triangle (general case - will work for every .obj) 
	/*for (int i = 0; i < verts.size(); i++) {
		printf("%f %f %f\n",verts[i].x, verts[i].y, verts[i].z);
		printf("%f %f %f\n",norms[i].x, norms[i].y, norms[i].z);
	}*/
	//calculateFaceNormals(); //manual way
	//manual way but normals in our case are also sorted correctly
}
void Raycaster::update(vec3 pos, vec3 dir) {
	ray = dir;
	position = pos;
	vec3 n;
	float d = 0;
	float t = 0;
	float tmin=1000.0f;	
	vec3 intersection;
	//for all triangles
	for (int i = 0; i < norms.size(); i += 3) {
		n = norms[i];
		d = abs(dot(verts[i] - position, n));
		t = -d/dot(ray, n);
		intersection = position + t * ray;
		if (dot(n, vec3(0, -1.0f, 0)) > 0.98 && intersection.x > -1.5f * Box::scaleXZ) continue;
		if (intersection.y > Box::scaleY*2.8f+0.1f) continue;
		if (t > 0 && t < tmin) {
			tmin = t;
			closest_intersection= intersection;
			closest_norm = n;
		}
	}

}

void Raycaster::calculateFaceNormals() {
	for (int i = 0; i < verts.size(); i += 3) {
		vec3 v1 = verts[i];
		vec3 v2 = verts[i + 1];
		vec3 v3 = verts[i + 2];
		vec3 cr = normalize(cross(v2 - v1, v3 - v2));
		if (dot(v1, cr) > 0) cr = -cr;
		printf("%f %f %f\n", cr.x, cr.y, cr.z);
	}

}

void Raycaster::scaleBox() {
	for (int i = 0; i < verts.size(); i++) {
		verts[i] = vec3(Box::scaleXZ * verts[i].x, Box::scaleY * verts[i].y, Box::scaleXZ * verts[i].z);
	}
}
