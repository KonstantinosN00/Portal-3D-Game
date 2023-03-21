#include <GL/glew.h>
#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Portal.h"
#include "Player.h"
#define MAX_BALLS 5
using namespace glm;
typedef void(*GLFWmousebuttonfun) (GLFWwindow*, int, int, int);
vector<Sphere*> Player::balls;
Portal* Player::portals[2];
Camera* Player::cam;
vec3 Player::position;
vec3 Player::velocity;
mat4 Player::modelMatrix;
float Player::strength = 13.0f;
int Player::currentPortal = 0;
vec3 cam_elevation = vec3(0,5.0f,0);
void Player::MouseButtonCallback(GLFWwindow*, int button, int action, int /*mods*/)
{
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1 && balls.size() < MAX_BALLS) {
		Sphere* current_sphere = new Sphere(cam->position - vec3(0, 1, 0), strength * (cam->direction+vec3(0,0.5f,0)) + velocity);
		Player::balls.push_back(current_sphere);
	}
}

Player::Player(GLFWwindow* win, vec3 pos, Box* b) {
	player = new Drawable("models/pl3.obj");
	cam = new Camera(win, pos+cam_elevation);
	box = b;
	window = win;
	height = pos.y;
	m = 1.0f;
	x = pos;
	P = m * v;
	raycaster = new Raycaster(box->cube, cam->position, cam->direction);
	/*
	mat3 I = mat3(
		2.0f / 5 * m * 10.0f, 0, 0,
		0, 2.0f / 5 * m * 10.0f, 0,
		0, 0, 2.0f / 5 * m * 10.0f);
	I_inv = inverse(I);
*/
}

Player::~Player() {
	delete raycaster;
}

void Player::draw(unsigned int drawable) {
	player->bind();
	player->draw();
}


void Player::update(float t, float dt) {

	
	handleEvents(dt);
	x.x=clamp(x.x, -box->maxx +0.18f, box->maxx - 0.18f);
	x.z=clamp(x.z, -box->maxz+0.18f, box->maxz-0.18f);
	if (x.y - height < 5.0f) disable = false;
	//we need p when changing velocity
	P = m * v;
	position = x;
	velocity = v;
	cam_elevation = vec3(0,5.0f,0)+cam->direction*0.06f;
	cam->position = x + cam_elevation;
	cam->update();
	raycaster->update(cam->position,cam->direction);
	//-------------Deploy Portal-------------------------//
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		portals[currentPortal]->visible = true;
		portals[currentPortal]->position = raycaster->closest_intersection + 0.1f*raycaster->closest_norm;

		portals[currentPortal]->rotation = -abs(acos(dot(portals[currentPortal]->planeNormals[0], raycaster->closest_norm)));
		//printf("%f\n", portals[currentPortal]->rotation);
		if (dot(raycaster->closest_norm, vec3(0, 0, 1.0f)) < -0.98f) {
			portals[currentPortal]->rot_vector = vec3(0.0f, 1.0f, 0.0f);
			//printf("%f %f %f\n", raycaster->closest_norm.x, raycaster->closest_norm.y, raycaster->closest_norm.z);
			portals[currentPortal]->rotation = 0;
		}
		else if (dot(raycaster->closest_norm, vec3(0, 0, 1.0f)) > 0.98f) {
			portals[currentPortal]->rot_vector = vec3(0, 1.0f,0);
			portals[currentPortal]->rotation = radians(180.0f);
		}
		if (dot(raycaster->closest_norm, vec3(0, 1.0f, 0))>0.98f) {
			portals[currentPortal]->rot_vector = vec3(-1.0f,0,0);
		} 
		else if (dot(raycaster->closest_norm, vec3(0, 1.0f, 0)) < - 0.98f) {
			portals[currentPortal]->rot_vector = vec3(1.0f, 0, 0);
		}
		if (dot(raycaster->closest_norm, vec3(1.0f, 0, 0)) > 0.98f) {
			portals[currentPortal]->rot_vector = vec3(0, 1.0f, 0);
		}
		else if (dot(raycaster->closest_norm, vec3(1.0f, 0, 0)) < -0.98f) {
			portals[currentPortal]->rot_vector = vec3(0, -1.0f, 0);
		}
		portals[currentPortal]->normal = raycaster->closest_norm;
		portals[currentPortal]->update();
		
	}
	advanceState(t, dt);
	
	for (int i = 0; i < balls.size(); i++) {
		balls[i]->advanceState(t, dt);
	}
	mat4 scale = glm::scale(mat4(), vec3(1,1,1));
	mat4 translation = translate(mat4(), vec3(x.x, x.y, x.z));
	
#ifdef USE_QUATERNIONS
	mat4 rotation = mat4_cast(q);
#else
	mat4 rotation = mat4(R);
	rotation = rotate(mat4(), cam->horizontalAngle, vec3(0, 1.0f, 0));
#endif
	modelMatrix = translation * rotation * scale;
}

void Player::handleEvents(float dt) {
	// callback to process only one mouse tap and not or every frame (one ball at a time)
	glfwSetMouseButtonCallback(window, &Player::MouseButtonCallback);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		x += normalize(vec3(cam->direction.x, 0, cam->direction.z)) * dt * cam->speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		x -= normalize(vec3(cam->direction.x, 0, cam->direction.z)) * dt * cam->speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		x += cam->right * dt * cam->speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		x -= cam->right * dt * cam->speed;
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		currentPortal = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		currentPortal = 1;
	}
	if (x.y < height) {
		x.y = height;
		v.y = 0.0f;
		jumped = false;
		forcing = [&](float t2, const vector<float>& y)->vector<float> {
			vector<float> f1(6, 0.0f);
			f1[1] = 0.0f;
			return f1;
		};

	}
	//nested or else jump repeats twice
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (!jumped) {

			jumped = true;
			v = vec3(0.0f, 4.0f, 0.0f);
			//we need p when changing velocity
			forcing = [&](float t2, const vector<float>& y)->vector<float> {

				vector<float> f(6, 0.0f);
				f[1] = -m * 9.81;
				return f;
			};
		}
	}
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		Sphere::radius += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		if (Sphere::radius > 0.3f) Sphere::radius -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		balls.clear();
	}
	
}


