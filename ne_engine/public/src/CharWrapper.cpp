#include "CharWrapper.hpp"

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
    return character;
}

