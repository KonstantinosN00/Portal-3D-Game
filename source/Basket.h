#ifndef BASKET_H
#define BASKET_H

#include <glm/glm.hpp>

class Drawable;

/**
 * Represents the bounding box
 */
class Basket {
public:
    static float size;
    float r;
    glm::vec3 position;
    Drawable* basket;
    glm::mat4 modelMatrix;

    Basket(glm::vec3 pos);
    ~Basket();

    void draw(unsigned int drawable = 0);
    void update(float t = 0, float dt = 0);
};

#endif
