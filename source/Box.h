#ifndef BOX_H
#define BOX_H

#include <glm/glm.hpp>

class Drawable;

/**
 * Represents the bounding box
 */
class Box {
public:
    static float scaleY;
    static float scaleXZ;
    float maxx;
    float maxy;
    float maxz;
    Drawable* cube;
    glm::mat4 modelMatrix;
    Box();
    ~Box();

    void draw(unsigned int drawable = 0);
    void update(float t = 0, float dt = 0);
};

#endif
