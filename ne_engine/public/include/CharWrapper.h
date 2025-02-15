#ifndef CHARWRAPPER_H
#define CHARWRAPPER_H

#include "Triangle.h"

class CharWrapper {
public:
    Triangle character;

    CharWrapper();
    CharWrapper(Triangle triangle);

    void moveDownWrapper();
    void moveRightWrapper();
    void moveLeftWrapper();
    void moveUpWrapper();

    Triangle getTriangle();
};



#endif //CHARWRAPPER_H
