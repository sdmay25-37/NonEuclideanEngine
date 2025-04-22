#ifndef TRANSFORMABLE_HPP
#define TRANSFORMABLE_HPP

#include <cmath>

class Transformable
{
    public:

    Transformable() = default;

    // Transformable(bool use_radian = true)
    // {
    //     this->use_radian = use_radian;
    // }

    ~Transformable() = default;

    // void set_unit_radian()
    // {
    //     use_radian = true;
    // }

    // void set_unit_degree()
    // {
    //     use_radian = false;
    // }

    float angle_unit_conv(float theta)
    {
        float theta_new = theta;

        // if(!use_radian)
        // {
        //     theta_new = theta * M_PI / 180.0f;
        // }

        return theta_new;
    }

    virtual void rot_x(float theta) = 0;
    virtual void rot_y(float theta) = 0;
    virtual void rot_z(float theta) = 0;
    virtual void translate(float x, float y, float z) = 0;

    virtual void rot_x_hyp(float theta) = 0;
    virtual void rot_y_hyp(float theta) = 0;
    virtual void rot_z_hyp(float theta) = 0;

    virtual void to_weirstrass() = 0;
    virtual void to_poincare() = 0;

    private:
    // bool use_radian;

};
#endif