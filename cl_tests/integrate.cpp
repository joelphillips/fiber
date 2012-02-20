#include <stdio.h>
#include <iostream>

#include "cll.h"
#include "util.h"
#include "commontypes.h"


void CL::runKernel_bfprod ()
{
    // Create the kernel
    try{
        kernel = cl::Kernel(program, "clBasisfProd", &err);
    }
    catch (cl::Error er) {
        printf("ERROR: %s(%d)\n", er.what(), er.err());
    }

    // create device buffers
    printf("Creating OpenCL arrays\n");
    cl_bfprod = cl::Buffer (context, CL_MEM_WRITE_ONLY, sizeof(FLOAT)*np*9, NULL, &err);
    // note: buffers cl_refpt1 and cl_refpt2 are assumed to be allocated and initialised

    //set the arguements of our kernel
    err = kernel.setArg(0, cl_refpt1);
    err = kernel.setArg(1, cl_refpt2);
    err = kernel.setArg(2, np);
    err = kernel.setArg(3, cl_bfprod);

    //Wait for the command queue to finish these commands before proceeding
    queue.finish();
 
    printf("in runKernel_bfprod\n");
    err = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(np), cl::NullRange,
				     NULL, &event);
    printf("clEnqueueNDRangeKernel: %s\n", oclErrorString(err));
    queue.finish();

    //lets check our calculations by reading from the device memory and printing out the results

//#define CHECK_BFPROD
#ifdef CHECK_BFPROD
    // Note: we don't need to copy the basis function values back to device here
    // We leave them on the device for the integrator

    FLOAT *bfprod = new FLOAT[np*9];
    err = queue.enqueueReadBuffer(cl_bfprod, CL_TRUE, 0, sizeof(FLOAT)*np*9, bfprod, NULL,
    				  &event);
    printf("clEnqueueReadBuffer: %s\n", oclErrorString(err));

    for (int i = 0; i < np; i++) {
        for (int j = 0; j < 3; j++) {
	    for (int k = 0; k < 3; k++) {
	        std::cout << bfprod[i*9 + j*3 + k] << '\t';
	    }
	    std::cout << std::endl;
	}
	std::cout << std::endl;
    }
    delete []bfprod;
#endif
}




void CL::runKernel_integrate (const Point3 *vtx1, const Point3 *vtx2, int ntri, FLOAT *res, int nres)
{
    const int MAX_NTRI = 10000;
    int i, j;

    printf("in runKernel_integrate\n");

    try{
        kernel = cl::Kernel(program, "clIntegrate", &err);
    }
    catch (cl::Error er) {
        printf("ERROR: %s(%d)\n", er.what(), er.err());
    }

    FLOAT *weight = new FLOAT[np];

    for (i = 0; i < np; i++)
        weight[i] = 1.0;


    int ntri_pass = std::min (ntri, MAX_NTRI);
    int bufsize = ntri_pass*3;
    int passx, passy, npass = (ntri + MAX_NTRI - 1)/MAX_NTRI;
    int ofsx = 0, ofsy = 0, ntrix, ntriy;

    FLOAT *local_res;
    if (npass > 1)
        local_res = new FLOAT[bufsize*bufsize];
    else
        local_res = res;

    // copy data to device
    printf("Creating OpenCL arrays (bufsize=%d)\n", bufsize);
    cl_tri1 = cl::Buffer (context, CL_MEM_READ_ONLY, sizeof(Point3)*bufsize, NULL, &err);
    cl_tri2 = cl::Buffer (context, CL_MEM_READ_ONLY, sizeof(Point3)*bufsize, NULL, &err);
    cl_weight = cl::Buffer (context, CL_MEM_READ_ONLY, sizeof(FLOAT)*np, NULL, &err);
    cl_integral = cl::Buffer (context, CL_MEM_WRITE_ONLY, sizeof(FLOAT)*bufsize*bufsize, NULL, &err);
    // note cl_pt and cl_bfprod are assumed to be on the device already from previous kernel call

    // loop over processing packages
    for (passx = 0, ofsx = 0; passx < npass; passx++) {
        ntrix = std::min (ntri-ofsx, MAX_NTRI);
        err = queue.enqueueWriteBuffer(cl_tri1, CL_TRUE, 0, sizeof(Point3)*ntrix*3, vtx1+ofsx*3, NULL, &event);
	for (passy = 0, ofsy = 0; passy < npass; passy++) {
	    printf ("Loop (%d,%d)\n", passx, passy);
  	    ntriy = std::min (ntri-ofsy, MAX_NTRI);
	    err = queue.enqueueWriteBuffer(cl_tri2, CL_TRUE, 0, sizeof(Point3)*ntriy*3, vtx2+ofsy*3, NULL, &event);

	    //set the arguements of our kernel
	    err = kernel.setArg(0, cl_tri1);
	    err = kernel.setArg(1, ntrix);
	    err = kernel.setArg(2, cl_tri2);
	    err = kernel.setArg(3, ntriy);
	    err = kernel.setArg(4, cl_refpt1);
	    err = kernel.setArg(5, cl_refpt2);
	    err = kernel.setArg(6, cl_weight);
	    err = kernel.setArg(7, np);
	    err = kernel.setArg(8, cl_bfprod);
	    err = kernel.setArg(9, cl_integral);

	    //Wait for the command queue to finish these commands before proceeding
	    queue.finish();

	    err = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(ntrix,ntriy), cl::NullRange,
				     NULL, &event);
	    printf("clEnqueueNDRangeKernel: %s\n", oclErrorString(err));
	    queue.finish();

	    // copy results back to host memory
	    err = queue.enqueueReadBuffer(cl_integral, CL_TRUE, 0, sizeof(FLOAT)*bufsize*bufsize, local_res,
					  NULL, &event);
	    printf("clEnqueueReadBuffer: %s\n", oclErrorString(err));

	    if (npass > 1) {
	        for (i = 0; i < ntrix*3; i++) {
		    for (j = 0; j < ntriy*3; j++) {
		        res[(ofsx*3+i)*ntri*3 + (ofsy*3+j)] = local_res[i*bufsize+j];
		    }
		}
	    }
	    ofsy += MAX_NTRI;
	}
	ofsx += MAX_NTRI;
    }

    if (npass > 1) delete []local_res;
    delete []weight;

}




void CL::runKernel_integratePairs (const int *tri1, const int *tri2, int ntri, FLOAT *res, int nres)
{
    // Compute integrals for all triangle pairs (tri1[i], tri2[i])
    // Triangle indices refer to global mesh geometry data
    // Mesh geometry is assumed to be avaiable in CL buffers

    try{
        kernel = cl::Kernel(program, "clIntegratePairs", &err);
    }
    catch (cl::Error er) {
        printf("ERROR: %s(%d)\n", er.what(), er.err());
    }

    // Allocate device buffers
    cl_tri1 = cl::Buffer (context, CL_MEM_READ_ONLY, sizeof(int)*ntri, NULL, &err);
    cl_tri2 = cl::Buffer (context, CL_MEM_READ_ONLY, sizeof(int)*ntri, NULL, &err);
    cl_integral = cl::Buffer (context, CL_MEM_WRITE_ONLY, sizeof(FLOAT)*ntri*9, NULL, &err);

    // Copy data to device
    err = queue.enqueueWriteBuffer (cl_tri1, CL_TRUE, 0, sizeof(int)*ntri, tri1, NULL, &event);
    err = queue.enqueueWriteBuffer (cl_tri2, CL_TRUE, 0, sizeof(int)*ntri, tri2, NULL, &event);

    // Set kernel arguments
    err = kernel.setArg ( 0, cl_meshvtx);
    err = kernel.setArg ( 1, nmeshvtx);
    err = kernel.setArg ( 2, cl_meshels);
    err = kernel.setArg ( 3, nmeshels);
    err = kernel.setArg ( 4, cl_tri1);
    err = kernel.setArg ( 5, cl_tri2);
    err = kernel.setArg ( 6, ntri);
    err = kernel.setArg ( 7, cl_refpt1);
    err = kernel.setArg ( 8, cl_refpt2);
    err = kernel.setArg ( 9, cl_weight);
    err = kernel.setArg (10, np);
    err = kernel.setArg (11, cl_bfprod);
    err = kernel.setArg (12, cl_integral);

    //Wait for the command queue to finish these commands before proceeding
    queue.finish();

    err = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(ntri), cl::NullRange,
				     NULL, &event);
    printf("clEnqueueNDRangeKernel: %s\n", oclErrorString(err));
    queue.finish();

    err = queue.enqueueReadBuffer(cl_integral, CL_TRUE, 0, sizeof(FLOAT)*ntri*9, res,
					  NULL, &event);
    printf("clEnqueueReadBuffer: %s\n", oclErrorString(err));
}

void CL::runKernel_integrateBlock (const int *triy, int ntriy, const int *trix, int ntrix,
    FLOAT *res, int nres)
{
    const int MAX_NTRI = 10000;
    int i, j;

    printf("in runKernel_integrateBlock\n");

    try{
        kernel = cl::Kernel(program, "clIntegrateBlock", &err);
    }
    catch (cl::Error er) {
        printf("ERROR: %s(%d)\n", er.what(), er.err());
    }

    int blocky = std::min (ntriy, MAX_NTRI);       // processing block size y
    int blockx = std::min (ntrix, MAX_NTRI);       // processing block size x

    int nblocky = (ntriy+MAX_NTRI-1)/MAX_NTRI;     // block count x
    int nblockx = (ntrix+MAX_NTRI-1)/MAX_NTRI;     // block count y
    int passy, passx;
    int ofsy = 0, ofsx = 0, nbufy, nbufx;

    FLOAT *local_res;
    if (nblocky > 1 || nblockx > 1)
        local_res = new FLOAT[blocky*blockx*9];
    else
        local_res = res;


    // Allocate device buffers
    cl_tri1 = cl::Buffer (context, CL_MEM_READ_ONLY, sizeof(int)*blocky, NULL, &err);
    cl_tri2 = cl::Buffer (context, CL_MEM_READ_ONLY, sizeof(int)*blockx, NULL, &err);
    cl_integral = cl::Buffer (context, CL_MEM_WRITE_ONLY, sizeof(FLOAT)*blocky*blockx*9, NULL, &err);
    // note cl_pt and cl_bfprod are assumed to be on the device already from previous kernel call

    // loop over processing packages
    for (passy = 0, ofsy = 0; passy < nblocky; passy++) {
        nbufy = std::min (ntriy-ofsy, MAX_NTRI);
        err = queue.enqueueWriteBuffer(cl_tri1, CL_TRUE, 0, sizeof(int)*nbufy, triy+ofsy, NULL, &event);
	for (passx = 0, ofsx = 0; passx < nblockx; passx++) {
	    printf ("Loop (%d,%d)\n", passy, passx);
  	    nbufx = std::min (ntrix-ofsx, MAX_NTRI);
	    err = queue.enqueueWriteBuffer(cl_tri2, CL_TRUE, 0, sizeof(int)*nbufx, trix+ofsx, NULL, &event);

	    //set the arguements of our kernel
	    err = kernel.setArg ( 0, cl_meshvtx);
	    err = kernel.setArg ( 1, nmeshvtx);
	    err = kernel.setArg ( 2, cl_meshels);
	    err = kernel.setArg ( 3, nmeshels);
	    err = kernel.setArg ( 4, cl_tri1);
	    err = kernel.setArg ( 5, nbufy);
	    err = kernel.setArg ( 6, cl_tri2);
	    err = kernel.setArg ( 7, nbufx);
	    err = kernel.setArg ( 8, cl_refpt1);
	    err = kernel.setArg ( 9, cl_weight);
	    err = kernel.setArg (10, np);
	    err = kernel.setArg (11, cl_refpt2);
	    err = kernel.setArg (12, cl_weight);
	    err = kernel.setArg (13, np);
	    err = kernel.setArg (14, cl_bfprod);
	    err = kernel.setArg (15, cl_integral);

	    //Wait for the command queue to finish these commands before proceeding
	    queue.finish();

	    err = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(blocky,blockx), cl::NullRange,
					     NULL, &event);
	    printf("clEnqueueNDRangeKernel: %s\n", oclErrorString(err));
	    queue.finish();

	    // copy results back to host memory
	    err = queue.enqueueReadBuffer(cl_integral, CL_TRUE, 0, sizeof(FLOAT)*blocky*blockx*9, local_res,
					  NULL, &event);
	    printf("clEnqueueReadBuffer: %s\n", oclErrorString(err));

	    if (local_res != res) {
	        for (j = 0; j < nbufx*3; j++) {
	            for (i = 0; i < nbufy*3; i++) {
		        res[(ofsx*3+j)*ntriy*3 + ofsx*3 + i] = local_res[j*blocky*3 + i];
		    }
		}
	    }
	    ofsx += MAX_NTRI;
	}
	ofsy += MAX_NTRI;
    }

    if (local_res != res) delete []local_res;
}
