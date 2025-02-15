#ifndef CHARWRAPPER_HPP
#define CHARWRAPPER_HPP

#include "Triangle.hpp"

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
