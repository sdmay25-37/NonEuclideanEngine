#ifndef HYPROTATE_HPP
#define HYPROTATE_HPP

#include <glm/glm.hpp>

class HypRotate
{
    public:
    HypRotate();
    HypRotate(bool input_radian);
    ~HypRotate();

    void rotateX(float theta);
    void rotateY(float theta);
    void rotateZ(float theta);
    void offset(const glm::vec4& offset);

    glm::mat4 getRotation();

    glm::vec4 operator*(glm::vec4 vector)
    {
        glm::vec4 vector_new = r_matrix * vector;

        return vector_new;
    }

    private:
    float thetaConversion(float theta);

    glm::mat4 r_matrix;
    bool input_radian;
};

#endif