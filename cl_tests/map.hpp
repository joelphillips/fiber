/**
 * \file map.hpp
 * Implements a dummy map class
 */

#ifndef __MAP_HPP
#define __MAP_HPP

/**
 * \brief Dummy base class for mapping from reference to global coordinates
 */
class Map {
public:
    /**
     * \brief Constructor. Does nothing.
     */
    Map () {}

    /**
     * \brief Return CL mapping code as a string.
     * \note The code for each implementation must define the following
     *  two local device functions:
     *  - %devMapPoint(const Point3 *vtx, const Point3 *ref, Point3 *pt)
     *  - %devDetJac(const Point3 *vtx)
     */
    virtual std::string CLstr_map() const = 0;
};

#endif // !__MAP_HPP
