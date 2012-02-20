/**
 * \file integrator.hpp
 * Dummy integrator class
 */

#ifndef __INTEGRATOR_HPP
#define __INTEGRATOR_HPP

#include "integrate.cl.str"
#include "integrate_pairs.cl.str"
#include "integrate_block.cl.str"

/**
 * \brief Dummy integrator class
 */
class Integrator {
public:
    Integrator () {}

    virtual std::string CLstr_integrate () const
    { return std::string (integrate_cl, integrate_cl_len); }

    /**
     * \brief Return CL code for integrating an array of element pairs
     * \note Implementations must define kernel function
     *   - %clIntegratePairs
     */
    virtual std::string CLstr_integratePairs () const
    { return std::string (integrate_pairs_cl, integrate_pairs_cl_len); }

    /**
     * \brief Return CL code for integrating a block of elements
     * \note Implementations must define kernel function
     *   - %clIntegrateBlock
     */
    virtual std::string CLstr_integrateBlock () const
    { return std::string (integrate_block_cl, integrate_block_cl_len); }
};

#endif // !__INTEGRATOR_HPP
