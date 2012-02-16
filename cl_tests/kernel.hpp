/**
 * \file kernel.hpp
 * Dummy implementation of a kernel class
 */

#ifndef __KERNEL_HPP
#define __KERNEL_HPP

/**
 * \brief Dummy kernel class
 */
class Kernel {
public:
    /**
     * \brief Constructor. Does nothing.
     */
    Kernel () {}

    /**
     * \brief Return CL code for kernel evaluation as a string.
     * \note Implementations must define function
     *   - %devKerneval(const Point3 *pt1, const Point3 *pt2)
     */
    virtual std::string CLstr_kerneval() const = 0;
};

#endif // !__KERNEL_HPP
