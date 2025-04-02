#include "HypRotate.hpp"

HypRotate::HypRotate()
: HypRotate(false)
{
    // Set to Identity Matrix
}

HypRotate::HypRotate(bool input_radian)
{
    this->r_matrix = glm::mat4(1.0);
    this->input_radian = input_radian;
}

HypRotate::~HypRotate()
{

}

void HypRotate::rotateX(float theta)
{
    theta = thetaConversion(theta);

    glm::mat4 r_x = glm::mat4(
        {1, 0               , 0               , 0},
        {0, std::cosh(theta), std::sinh(theta), 0},
        {0, std::sinh(theta), std::cosh(theta), 0},
        {0, 0               , 0               , 1}
    );

    r_matrix = r_x * r_matrix;
}

void HypRotate::rotateY(float theta)
{
    theta = thetaConversion(theta);

    glm::mat4 r_y = glm::mat4(
        {std::cosh(theta), 0, std::sinh(theta), 0},
        {0               , 1, 0               , 0},
        {std::sinh(theta), 0, std::cosh(theta), 0},
        {0               , 0, 0               , 1}
    );

    r_matrix = r_y * r_matrix;
}
void HypRotate::rotateZ(float theta)
{
    theta = thetaConversion(theta);

    glm::mat4 r_z = glm::mat4(
        {std::cosh(theta), std::sinh(theta), 0, 0},
        {std::sinh(theta), std::cosh(theta), 0, 0},
        {0               , 0               , 1, 0},
        {0               , 0               , 0, 1}
    );

    r_matrix = r_z * r_matrix;
}

void HypRotate::offset(const glm::vec4& offset)
{
    glm::mat4 r_off = glm::mat4(
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {offset[0], offset[1], offset[2], 1}
    );

    r_matrix = r_off * r_matrix;
}

glm::mat4 HypRotate::getRotation()
{
    return r_matrix;
}

float HypRotate::thetaConversion(float theta)
{
    float theta_new = theta;

    if(!input_radian)
    {
        theta_new = theta * M_PI / 180.0f;
    }

    return theta_new;
}