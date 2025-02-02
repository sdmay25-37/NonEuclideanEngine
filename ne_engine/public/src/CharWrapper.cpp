#include "CharWrapper.h"

#include <iostream>

CharWrapper::CharWrapper() {
}

CharWrapper::CharWrapper(Triangle triangle) {
    CharWrapper::character = triangle; 
}

void CharWrapper::moveRightWrapper() {
    character.moveRight();
}

void CharWrapper::moveLeftWrapper() {    
    character.moveLeft();
}

void CharWrapper::moveUpWrapper() {
    character.moveUp();
}

void CharWrapper::moveDownWrapper() {
    character.moveDown();
}

Triangle CharWrapper::getTriangle() {
    std::cout << "Returning Character with coords: (" << character.getVerts().at(0).pos[0] << ", " <<  character.getVerts().at(0).pos[1] << ")" << std::endl;
    return character;
}

