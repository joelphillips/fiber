/**
 * \file main.cpp
 * Example application of OpenCL integrator invocation
 */

#include <stdio.h>
#include <iostream>

#include "commontypes.h.str"
#include "cll.h"
#include "mesh.h"
#include "integrator.hpp"
#include "kernel_laplace.hpp"
#include "map_affinebary.hpp"
#include "element_lintri.hpp"
#include "quadrature.hpp"

#define OPENCL_STRING_WRAP

const char *meshname = "../data/headsurf.dat";

int main(int argc, char** argv)
{
    int i, j, k;

    CL example;
    
    // Read a sample surface mesh
    Mesh mesh;
    mesh.Read (meshname);

    Integrator integrator;     // dummy integrator instance
    KernelLaplace kernel;      // dummy kernel instance
    Map_AffineBarycentric map; // dummy element map instance
    Element_LinTri element;    // dummy element instance
    const Quadrature quad;     // dummy quadrature instance

    // Collect the CL code strings from the required components
    std::vector<std::string> sources;
    sources.push_back (commontypes_h);
    sources.push_back (element.CLstr_basisf());
    sources.push_back (map.CLstr_map());
    sources.push_back (kernel.CLstr_kerneval());
    sources.push_back (integrator.CLstr_integratePairs());
    sources.push_back (integrator.CLstr_integrateBlock());

    // Build the CL program from the code fragments
    example.loadProgramFromFragments (sources);

    // Push the mesh geometry to device memory
    example.PushGeometry (mesh);

    // Push the quadrature rule to device memory
    example.PushQuadrature (quad);

    // Execute the kernel for computing basis function products
    // Uses the quadrature points on the device
    example.runKernel_bfprod ();

    //initialize CPU memory arrays, send them to the device and set the kernel arguements

    // Create a simple list of triangle pairs for processing
    {
        int ntri = std::min (10000, mesh.nEl());
	int *tri1 = new int[ntri];
	int *tri2 = new int[ntri];
        int nres = ntri*9;
	FLOAT *res = new FLOAT[nres];
	for (i = 0; i < ntri; i++) {
	    tri1[i] = 0;
	    tri2[i] = i;
	}

	example.runKernel_integratePairs (tri1, tri2, ntri, res, nres);
	
	delete []tri1;
	delete []tri2;
	delete []res;
    }

    // Now try integrating a block of elements
    {
        int ntri1 = 100;
	int ntri2 = 10;
	int *tri1 = new int[ntri1];
	int *tri2 = new int[ntri2];
	int nres = ntri1*ntri2*9;
	FLOAT *res = new FLOAT[nres];

	for (i = 0; i < ntri1; i++) tri1[i] = i;
	for (i = 0; i < ntri2; i++) tri2[i] = i;

	example.runKernel_integrateBlock (tri1, ntri1, tri2, ntri2, res, nres);

	delete []tri1;
	delete []tri2;

#define SHOWRES
#ifdef SHOWRES
	for (i = 0; i < ntri1; i++) {
	    for (j = 0; j < 3; j++) {
	        for (k = 0; k < 3; k++) {
		    std::cout << res[i*9 + j*3 + k] << '\t';
		}
		std::cout << std::endl;
	    }
	    std::cout << std::endl;
	}
#endif

	delete []res;
    }

    return 0;
}
