/**
 * \file element.hpp
 * Implements a dummy element class
 */

#ifndef __ELEMENT_HPP
#define __ELEMENT_HPP

/**
 * \brief Dummy base class for element types
 */
class Element {
public:
    /**
     * \brief Constructor. Does nothing.
     */
    Element () {}

    /**
     * \brief Return CL code for generating products of basis functions
     *   at a set of reference point pairs.
     * \note The code for each implementation must define the following
     *   kernel function:
     *   - %clBasisfProd(__global const Point3 *p1, __global const Point3 *p2, int np, __global FLOAT *g_bfprod)
     */
    virtual std::string CLstr_basisf() const = 0;
};

#endif // !__ELEMENT_HPP
