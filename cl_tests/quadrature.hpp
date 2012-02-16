/**
 * \file quadrature.hpp
 * Implements a dummy class for defining a quadrature rule
 */

#ifndef __QUADRATURE_HPP
#define __QUADRATURE_HPP

#include "commontypes.h"

/**
 * \brief Dummy quadrature class
 */
class Quadrature {
public:
    /**
     * \brief Constructor.
     * Allocates and initialises quadrature points and weights.
     */
    Quadrature ();

    /**
     * \brief Destructor.
     * Deallocates points and weights.
     */
    ~Quadrature ();

    /**
     * \brief Returns quadrature points and weights.
     * \param [out] pt1 Pointer to array of quadrature points for first
     *   triangle
     * \param [out] pt2 Pointer to array of quadrature points for second
     *   triangle
     * \param [out] w Pointer to array of weights
     * \return Number of quadrature points returned in the lists.
     * \note At the moment, this is a dummy implementation that returns 100
     *   random points in the reference triangle, with weights all 1.0.
     */
    int AbscissaeAndWeights (const Point3 **pt1, const Point3 **pt2,
        const FLOAT **w) const;

private:
    Point3 *absc;
    FLOAT *wght;
    int np;
};

#endif // !__QUADRATURE_HPP
