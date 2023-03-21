#ifndef COLLISION_H
#define COLLISION_H
#include "Box.h"
#include "Basket.h"
#include "Sphere.h"
#include "Portal.h"
#include "Player.h"
#include <glm/glm.hpp>

class Box;
class Sphere;
void handleBoxSphereCollision(Box& box, Sphere& sphere);
void handleBasketSphereCollision(Basket& basket, Sphere& sphere);
void handleSphereSphereCollision(Sphere& sphere1, Sphere& sphere2);
bool handlePortalSphereCollision(Sphere& s);
void handlePortalPlayerCollision(Player& pl,float t, float dt);

bool checkForPortalSphereCollision(Sphere& s, Portal& p);
bool checkForPortalPlayerCollision(Portal& p,Player& player);
bool checkForBasketSphereCollision(Basket& basket, Sphere& s, glm::vec3& n);
bool checkForBoxSphereCollision(glm::vec3& pos, const float& r, const float& maxx, const float& maxy, const float& maxz, glm::vec3& n);
bool checkForSphereSphereCollision(glm::vec3& pos1, glm::vec3& pos2, glm::vec3& v1, glm::vec3& v22,const float& r1, const float& r2);

#endif
