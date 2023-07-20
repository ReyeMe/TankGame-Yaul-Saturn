#pragma once
#include <yaul.h>

namespace Utenyaa::Components
{
    /** @brief Transform component
     */
    struct Transform
    {
        /** @brief Transformation matrix
         */
        fix16_mat43_t Matrix;
    };
}