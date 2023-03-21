// Include C++ headers
#include <iostream>
#include <string>
#include <vector>
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Shader loading utilities and other
#include <common/shader.h>
#include <common/util.h>
#include <common/camera.h>
#include <common/texture.h>

#include "Cube.h"
#include "Sphere.h"
#include "Box.h"
#include "MassSpringDamper.h"
#include "Collision.h"
#include "Portal.h"
#include "Player.h"
#include "Basket.h"
#include "Sky.h"

using namespace std;
using namespace glm;

// Function prototypes
void initialize();
void createContext();
void mainLoop();
void drawAll(mat4 viewMatrix, mat4 projectionMatrix, Portal* portal[2], int maxRecursionLevel, int recursionLevel);
void drawScene();
void free();

#define W_WIDTH 1024
#define W_HEIGHT 768
#define TITLE "Portals 2 Project"
#define g 9.80665f

// Global variables
GLFWwindow* window;
Camera* camera;
GLuint shaderProgram;
GLuint projectionMatrixLocation, viewMatrixLocation, modelMatrixLocation;
GLuint useTextureLocation;

GLuint floorTextureSampler, floorTexture;
GLuint skyTextureSampler, skyTexture;
GLuint specularColorSampler;

Player* player;
Cube* cube;
Box* box;
Basket* basket;
Sky* sky;
mat4 virtual_view, virtual_proj;
float texture_num=1;

void createContext() {
	shaderProgram = loadShaders(
		"StandardShading.vertexshader",
		"StandardShading.fragmentshader");

	projectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");
	viewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
	modelMatrixLocation = glGetUniformLocation(shaderProgram, "M");
	useTextureLocation = glGetUniformLocation(shaderProgram, "useTexture");
	//----TEXTURES----//	
	floorTextureSampler = glGetUniformLocation(shaderProgram, "floorTextureSampler");
	skyTextureSampler = glGetUniformLocation(shaderProgram, "skyTextureSampler");
	floorTexture = loadBMP("img\\basketballcourt.bmp");
	skyTexture = loadBMP("img\\sky.bmp");
	//glGenTextures(1, &floorTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, skyTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	box = new Box();
	player = new Player(window, vec3(0, 1, 20),box);
	basket = new Basket(vec3(36.0f,10.0f,0));
	sky = new Sky();
	Player::portals[0] = new Portal(vec3(-2, 3, -2), 0);
	Player::portals[1] = new Portal(vec3(1, 3, -2), 0);

}

void free() {
	delete cube;
	delete box;
	delete Player::portals[0];
	delete Player::portals[1];
	delete basket;
	delete player;
	glDeleteTextures(1,&floorTexture);
	glDeleteTextures(1,&skyTexture);
	

	glDeleteProgram(shaderProgram);
	glfwTerminate();
}

void mainLoop() {
	float t = glfwGetTime();

	do {
		// calculate dt
		float currentTime = glfwGetTime();
		float dt = (currentTime - t);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glUseProgram(shaderProgram);
		// camera
		player->update(currentTime, dt);
		mat4 projectionMatrix = Player::cam->projectionMatrix;
		mat4 viewMatrix = Player::cam->viewMatrix;
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

		//use texture
		glUniform1i(useTextureLocation, 1);
		// Activate texture0
		glActiveTexture(GL_TEXTURE0);
		// Bind our texture in the currently active texture unit (which is 0)
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		// Set our "textureSampler" sampler to use Texture Unit 1
		glUniform1i(floorTextureSampler, 0);
		// Activate texture1
		glActiveTexture(GL_TEXTURE1);
		// Bind our texture in the currently active texture unit (which is 0)
		glBindTexture(GL_TEXTURE_2D, skyTexture);
		// Set our "textureSampler" sampler to use Texture Unit 1
		glUniform1i(skyTextureSampler, 1);
		

		drawAll(viewMatrix, projectionMatrix, Player::portals, 3, 0);
		for (int i = 0; i < Player::balls.size(); i++) {
			handleBasketSphereCollision(*basket, *Player::balls[i]);
			bool b = false;
			if(Player::portals[0]->visible && Player::portals[1]->visible) b = handlePortalSphereCollision(*Player::balls[i]);
			if(!b) handleBoxSphereCollision(*box,*Player::balls[i]);
			for (int j = i+1; j < Player::balls.size(); j++) {
				handleSphereSphereCollision(*Player::balls[i],*Player::balls[j]);
			}
		}
		if (Player::portals[0]->visible && Player::portals[1]->visible) handlePortalPlayerCollision(*player,t,dt);
		

		t += dt;
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
}

void initialize() {
	// Initialize GLFW
	if (!glfwInit()) {
		throw runtime_error("Failed to initialize GLFW\n");
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		throw runtime_error(string(string("Failed to open GLFW window.") +
			" If you have an Intel GPU, they are not 3.3 compatible." +
			"Try the 2.1 version.\n"));
	}
	glfwMakeContextCurrent(window);

	// Start GLEW extension handler
	glewExperimental = GL_TRUE;

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		throw runtime_error("Failed to initialize GLEW\n");
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Hide the mouse and enable unlimited movement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, W_WIDTH / 2, W_HEIGHT / 2);

	// Gray background color
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	// glFrontFace(GL_CW);
	// glFrontFace(GL_CCW);
	//glEnable(GL_STENCIL_TEST);
	// enable point size when drawing points
	glEnable(GL_PROGRAM_POINT_SIZE);

	// Log
	logGLParameters();

	// Create camera
	//camera = new Camera(window);



}

void drawAll(mat4 viewMatrix, mat4 projectionMatrix, Portal* portal[2], int maxRecursionLevel, int recursionLevel) {
	for (int i = 0; i < 2; i++) {

		// Disable color and depth drawing
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

		// Disable depth test
		glDisable(GL_DEPTH_TEST);

		// Enable stencil test, to prevent drawing outside
		// region of current portal depth
		glEnable(GL_STENCIL_TEST);

		// Fail stencil test when inside of outer portal
		// (fail where we should be drawing the inner portal)
		glStencilFunc(GL_NOTEQUAL, recursionLevel, 0xFF);

		// Increment stencil value on stencil fail
		// (on area of inner portal)
		glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);

		// Enable (writing into) all stencil bits
		glStencilMask(0xFF);

		// Draw portal into stencil buffer
		if (Player::portals[i]->visible) {
			Player::portals[i]->update();
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &Player::portals[i]->modelMatrix[0][0]);
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
			glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
			Player::portals[i]->draw();


			// Calculate view matrix as if the player was already teleported

			virtual_view = viewMatrix * Player::portals[i]->modelMatrix
				* rotate(mat4(1.0f), radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f))
				* inverse(Player::portals[1 - i]->modelMatrix);
			//mat4 clipped_proj = Player::portals[i]->clippedProjMat(virtual_view, projectionMatrix);
		}
		// Base case, render inside of inner Player::portals
		if (recursionLevel == maxRecursionLevel)
		{
			// Enable color and depth drawing
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glDepthMask(GL_TRUE);

			// Clear the depth buffer so we don't interfere with stuff
			// outside of this inner portal
			glClear(GL_DEPTH_BUFFER_BIT);

			// Enable the depth test
			// So the stuff we render here is rendered correctly
			glEnable(GL_DEPTH_TEST);

			// Enable stencil test
			// So we can limit drawing inside of the inner portal
			glEnable(GL_STENCIL_TEST);

			// Disable drawing into stencil buffer
			glStencilMask(0x00);

			// Draw only where stencil value == recursionLevel + 1
			// which is where we just drew the new portal
			glStencilFunc(GL_EQUAL, recursionLevel + 1, 0xFF);

			// Draw scene objects with destView, limited to stencil buffer
			// use an edited projection matrix to set the near plane to the portal plane
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &virtual_view[0][0]);
			drawScene();

		}
		else
		{
			// Recursion case
			// Pass our new view matrix and the clipped projection matrix (see above)
			drawAll(virtual_view, projectionMatrix, Player::portals, maxRecursionLevel, recursionLevel + 1);
		}
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

		// Disable color and depth drawing
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

		// Enable stencil test and stencil drawing
		glEnable(GL_STENCIL_TEST);
		glStencilMask(0xFF);

		// Fail stencil test when inside of our newly rendered
		// inner portal
		glStencilFunc(GL_NOTEQUAL, recursionLevel + 1, 0xFF);

		// Decrement stencil value on stencil fail
		// This resets the incremented values to what they were before,
		// eventually ending up with a stencil buffer full of zero's again
		// after the last (outer) step.
		glStencilOp(GL_DECR, GL_KEEP, GL_KEEP);

		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
		// Draw portal into stencil buffer
		if (Player::portals[i]->visible) {
			Player::portals[i]->update();
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &Player::portals[i]->modelMatrix[0][0]);
			Player::portals[i]->draw();
		}

	}
	// Disable the stencil test and stencil writing
	glDisable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	// Disable color writing
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	// Enable the depth test, and depth writing.
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	// Make sure we always write the data into the buffer
	glDepthFunc(GL_ALWAYS);

	// Clear the depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	// Draw Player::portals into depth buffer
	if (Player::portals[0]->visible) {
		Player::portals[0]->update();
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &Player::portals[0]->modelMatrix[0][0]);
		Player::portals[0]->draw();
	}
	if (Player::portals[1]->visible) {
		Player::portals[1]->update();
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &Player::portals[1]->modelMatrix[0][0]);
		Player::portals[1]->draw();
	}
	// Reset the depth function to the default
	glDepthFunc(GL_LESS);

	// Enable stencil test and disable writing to stencil buffer
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	// Draw at stencil >= recursionlevel
	// which is at the current level or higher (more to the inside)
	// This basically prevents drawing on the outside of this level.
	glStencilFunc(GL_LEQUAL, recursionLevel, 0xFF);

	// Enable color and depth drawing again
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	// And enable the depth test
	glEnable(GL_DEPTH_TEST);
	drawScene();
}

void drawScene() {
	for (int i = 0; i < Player::balls.size(); i++) {
		Player::balls[i]->update();
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &Player::balls[i]->modelMatrix[0][0]);
		Player::balls[i]->draw();
	}

	box->update();
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &box->modelMatrix[0][0]);
	box->draw();
	//use sky texture
	glUniform1i(useTextureLocation, 2);
	sky->update();
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &sky->modelMatrix[0][0]);
	sky->draw();
	//use court texture again
	glUniform1i(useTextureLocation, 1);
	basket->update();
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &basket->modelMatrix[0][0]);
	basket->draw();
	//player->update();
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &player->modelMatrix[0][0]);
	player->draw();
}

int main(void) {
	try {
		initialize();
		createContext();
		mainLoop();
		free();
	}
	catch (exception& ex) {
		cout << ex.what() << endl;
		getchar();
		free();
		return -1;
	}

	return 0;
}