/**
 * \file element_lintri.hpp
 * Implements a dummy linear triangle class
 */

#ifndef __ELEMENT_LINTRI_HPP
#define __ELEMENT_LINTRI_HPP

#include "element.hpp"
#include "basisf_lintri.cl.h"

/**
 * \brief Dummy class for a linear triangle type
 */
class Element_LinTri: public Element {
public:
    /**
     * \brief Constructor. Does nothing.
     */
    Element_LinTri (): Element () {}

    /**
     * \brief Return CL code for generating products of basis functions
     *   at a set of reference points
     * \note Returns the code in basisf_lintri.cl, implementing the kernel function
     *   - clBasisfProd(__global const Point3 *g_p1, __global const Point3 *g_p2, int np, __global FLOAT *g_bfprod)
     */
    std::string CLstr_basisf() const
    { return std::string (basisf_lintri_cl, basisf_lintri_cl_len); }
};

#endif // !__ELEMENT_LINTRI_HPP
