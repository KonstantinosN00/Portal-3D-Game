#ifndef SKY_H
#define SKY_H

#include <glm/glm.hpp>
#include <common/model.h>

class Drawable;

class Sky  {
public:
    Drawable* sky;
    glm::mat4 modelMatrix;

    Sky();
    ~Sky();

    void draw(unsigned int drawable = 0);
    void update();
};

#endif