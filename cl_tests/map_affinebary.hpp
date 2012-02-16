/**
 * \file map_affinebary.hpp
 * Implements a dummy affine barycentric map class
 */

#ifndef __MAP_AFFINEBARYCENTRIC
#define __MAP_AFFINEBARYCENTRIC

#include "map.hpp"
#include "map_affinebary.cl.h"

/**
 * \brief Dummy class for an affine barycentric mapper
 */
class Map_AffineBarycentric: public Map {
public:
    /**
     * \brief Constructor. Does nothing.
     */
    Map_AffineBarycentric(): Map() {}

    /**
     * \brief Return CL mapping code as a string.
     * \note Returns the CL code in map_affinebary.cl, implementing the two functions
     *  - devMapPoint(const Point3 *vtx, const Point3 *ref, Point3 *pt)
     *  - devDetJac(const Point3 *vtx)
     */
    std::string CLstr_map() const
    { return std::string (map_affinebary_cl, map_affinebary_cl_len); }
};

#endif // !__MAP_AFFINEBARYCENTRIC
