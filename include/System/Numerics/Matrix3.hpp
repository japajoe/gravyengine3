#ifndef GRAVYENGINE_MATRIX3_HPP
#define GRAVYENGINE_MATRIX3_HPP

#include "../../External/glm/glm.h"

namespace GravyEngine
{
    typedef glm::mat3 Matrix3;

    class Matrix3f
    {
    public:
        static glm::mat3 Identity();
    };
};

#endif