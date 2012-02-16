#ifndef ADVCL_CLL_H_INCLUDED
#define ADVCL_CLL_H_INCLUDED

/**
 * \file cll.h
 * Implementation of class CL. OpenCL handler for CL memory management and kernel
 * launchers.
 * This class contains methods for pushing data for mesh geometry and quadrature rules
 * to device memory, and for launching integration kernels.
 */

#define __CL_ENABLE_EXCEPTIONS

#include <vector>
#include "CL/cl.hpp"
#include "commontypes.h"
#include "mesh.h"
#include "quadrature.hpp"

/**
 * \brief OpenCL wrapper class for BEM integrations.
 */
class CL {
    public:

        cl::Buffer cl_bfprod;
	cl::Buffer cl_tri1;
	cl::Buffer cl_tri2;
	cl::Buffer cl_integral;

	/**
	 * \brief Default constructor.
	 * Initializes OpenCL context and automatically chooses platform and device
	 */
        CL();

	/**
	 * Default destructor.
	 * Releases OpenCL objects and frees device memory
	 */
        ~CL();

	/**
	 * \brief Load an OpenCL program from a string.
         * Pass in the kernel source code as a string.
	 * \param kernel_source OpenCL kernel source
	 */
        void loadProgram(std::string kernel_source);

	/**
	 * \brief Load an OpenCL program as a concatenation of multiple strings
	 * \param kernel_sources Vector of source strings
	 */
	void loadProgramFromFragments (std::vector<std::string> kernel_sources);

	/**
	 * \brief Compute products of shape functions for an array of
	 *    reference point pairs
	 * \note Calls kernel 'clBasisfProd' which is assumed to be available
	 *    in the loaded CL program.
	 * \note The reference points are assumed to be available in buffers cl_refpt1
	 *    and cl_refpt2, each of size np.
	 * \note The results are left in GPU global memory in buffer cl_bfprod.
	 */
        void runKernel_bfprod ();

	// execute the integration kernel
	void runKernel_integrate (const Point3 *vtx1, const Point3 *vtx2,
	    int ntri, FLOAT *res, int nres);

	/**
	 * \brief Execute the clIntegratePairs kernel
	 * \param tri1 Array of element indices for the first element of each pair
	 * \param tri2 Array of element indices for the second element of each pair
	 * \param ntri Number of element pairs
	 * \param res Matrix of integration results
	 * \param nres length of res array
	 * \note \ref PushGeometry must have been called to provide global mesh
	 *    geometry data
	 * \note \ref PushQuadrature must have been called to provide quadrature data
	 * \note \ref runKernel_bfprod must have been called to provide basis function
	 *   products
	 * \note Array res must have been allocated to contain at least ntri*9
	 *   elements.
	 * \note Elements in res are arranged so that res[i*9 + j*3 + k] is the
	 *   integral of element pair i, for basis functions j of the first and k of
	 *   the second element.
	 */
	void runKernel_integratePairs (const int *tri1, const int *tri2,
	    int ntri, FLOAT *res, int nres);

	/**
	 * \brief Execute the clIntegrateBlock kernel
	 * \param triy Array of element indices for the first dimension of the block
	 * \param ntriy Number of elements in the triy array
	 * \param trix Array of element indices for the second dimension of the block
	 * \param ntrix Number of elements in the trix array
	 * \param res Matrix of integration results
	 * \param nres length of res array
	 * \note \ref PushGeometry must have been called to provide global mesh
	 *   geometry data
	 * \note \ref PushQuadrature must have been called to provide quadrature data
	 * \note \ref runKernel_bfprod must have been called to provide basis function
	 *   products
	 * \note Array res must have been allocated to contain at least ntrix*ntriy*9
	 *   elements.
	 * \note Elements in res are arranged so that
	 *   res[i*ntriy*9 + j*ntriy*3 + k*3 + l] is the
	 *   integral of element pair (i,k), for basis functions j of the first and l
	 *   of the second element.
	 */
	void runKernel_integrateBlock (const int *triy, int ntriy,
	    const int *trix, int ntrix,
	    FLOAT *res, int nres);

	/**
	 * \brief Push the mesh geometry to device memory
	 * \param mesh mesh instance
	 * \note This method populates the cl_meshvtx and cl_meshidx buffers
	 */
	void PushGeometry (const Mesh &mesh);

	/**
	 * \brief Deallocate mesh geometry buffers on the device
	 */
	void FreeGeometry ();

	/**
	 * \brief Push quadrature points and weights to device memory
	 * \param quad Quadrature instance
	 * \note This method populates the cl_refpt1, cl_refpt2 and cl_weight
	 *   buffers on the device
	 */
	void PushQuadrature (const Quadrature &quad);

    private:

	cl::Buffer cl_meshvtx; ///< Mesh geometry: vertex list
	int nmeshvtx;          ///< Length of vertex list
	cl::Buffer cl_meshels; ///< Mesh geometry: element index list
	int nmeshels;          ///< Length of index list

        cl::Buffer cl_refpt1;  ///< Reference quadrature abscissae for first element
	cl::Buffer cl_refpt2;  ///< Reference quadrature abscissae for second element
	cl::Buffer cl_weight;  ///< Quadrature weights
	int np;   // number of quadrature points

        unsigned int deviceUsed;
        std::vector<cl::Device> devices;
        
        cl::Context context;

        cl::CommandQueue queue;
        cl::Program program;
        cl::Kernel kernel;
        

        //debugging variables
        cl_int err;
        cl::Event event;
};

#endif
