/**
 * \file kernel_laplace.hpp
 * Dummy implementation of a Laplace kernel class
 */

#ifndef __KERNEL_LAPLACE_HPP
#define __KERNEL_LAPLACE_HPP

#include "kernel.hpp"
#include "laplace.cl.h"

/**
 * \brief Dummy Laplace kernel class
 */
class KernelLaplace: public Kernel {
public:
    /**
     * \brief Constructor. Does nothing.
     */
    KernelLaplace (): Kernel () {}

    /**
     * \brief Return CL code for Laplace kernel evaluation as a string.
     * \note Returns the CL code in laplace.cl, implementing the device function
     *  - devKerneval(const Point3 *pt1, const Point3 *pt2)
     */
    std::string CLstr_kerneval() const
    { return std::string (laplace_cl, laplace_cl_len); }
};

#endif // !__KERNEL_LAPLACE_HPP
