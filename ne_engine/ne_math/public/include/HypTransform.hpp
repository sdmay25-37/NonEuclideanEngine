#ifndef HYPCONV_HPP
#define HYPCONV_HPP

#include <vector>
#include "glm/glm.hpp"

class HypTransform
{
    public:

    static void poincare_to_hyperboloid(std::vector<glm::vec4>& points);

    static void to_plane_ortho(std::vector<glm::vec4>& points);

    static void to_hyperboloid_ortho(std::vector<glm::vec4>& points);

    // static void transform_3d_2d(std::vector<glm::vec4>& points);

    private:
};
#endif