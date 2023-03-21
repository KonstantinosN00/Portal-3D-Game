#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <vector>
#include <glm/glm.hpp>
#include <glfw3.h>
#include <vector>
#include "RigidBody.h"
#include "Raycaster.h"
#include "Portal.h"
#include "Sphere.h"
#include "Box.h"
#include "common/model.h"
#include "common/camera.h"

class Player :public RigidBody{
public:
    static vector <Sphere*> balls;
    static Portal* portals[2];
    static Camera* cam;
    static int currentPortal;
    static glm::vec3 position;
    static glm::vec3 velocity; //z-axis velocity
    static float strength;
    static glm::mat4 modelMatrix;
    bool disable = false;
    Raycaster* raycaster;
    Drawable* player;
    Box* box;
    GLFWwindow* window;
    
    float height=4.0f;
    bool jumped = false;
    void draw(unsigned int drawable = 0);
    static void MouseButtonCallback(GLFWwindow*, int button, int action, int);
    Player(GLFWwindow* window,glm::vec3 pos, Box* b);
    ~Player();
    void update(float t, float dt);
    void handleEvents(float dt);
};

#endif
