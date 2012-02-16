// -*-C++-*-

/**
 * \file laplace.cl
 * OpenCL implementation for Laplace kernel evaluation
 */

#include "commontypes.h"

/**
 * \brief Simple Laplace kernel
 * \param pt1 First point in global coordinates
 * \param pt2 Second point in global coordinates
 * \return %Kernel value
 */
FLOAT devKerneval (const Point3 *pt1, const Point3 *pt2)
{
    FLOAT dx = pt1->x - pt2->x;
    FLOAT dy = pt1->y - pt2->y;
    FLOAT dz = pt1->z - pt2->z;
    return pow (dx*dx + dy*dy + dz*dz, -0.5f);
}


