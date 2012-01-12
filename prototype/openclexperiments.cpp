/*
 * openclexperiments.cpp
 *
 *  Created on: Jan 5, 2012
 *      Author: joel
 */
#include <utility>
#define __CL_ENABLE_EXCEPTIONS
#define __NO_STD_VECTOR // Use cl::vector instead of STL version
#include <cl.hpp>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <unistd.h>
#include "openclutil.h"
inline void
checkErr(cl_int err, const char * name)
{
    if (err != CL_SUCCESS) {
        std::cerr << "ERROR: " << name << " (" << err << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
}

const std::string hw("Hello World\n");

void testopencl(){
	try{
		ContextWrapper cw;

		static const int N = 10;

		double * v1 = new double[N];
		double * v2 = new double[N];
		for(int i = 0; i < N; i++){
			v1[i] = 3;
			v2[i] = i;
		}

		cl::Buffer v1CL(cw.getContext(),
				CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
				N * sizeof(double),
				v1);

		cl::Buffer v2CL(cw.getContext(),
				CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
				N * sizeof(double),
				v2);

		cl::Buffer rCL(
			cw.getContext(),
			CL_MEM_WRITE_ONLY,
			N*sizeof(double));

		cl::Program program = cw.getProgramFromFiles(std::vector<std::string>(1, "prototype/lesson1_kernels.cl"));

		cl::Kernel kernel(program, "sum");
//		kernel.setArg(0, inCL);
		kernel.setArg(0, v1CL);
		kernel.setArg(1, v2CL);
		kernel.setArg(2, rCL);

		cl::CommandQueue queue = cw.getQueue();
		cl::Event event;
		queue.enqueueNDRangeKernel(
			kernel,
			cl::NullRange,
			cl::NDRange(N),
			cl::NDRange(1, 1),
			NULL,
			&event);

		event.wait();

		double * r = new double[N];

		queue.enqueueReadBuffer(rCL,CL_TRUE,0,N*sizeof(double),r);

		std::cerr <<" And the output is ... "<<std::endl;
		for(int i = 0; i < N; i++){
			std::cerr<<r[i]<<" ";
		}
	}
	catch (cl::Error err) {
		std::cerr << err.what() << "(" << err.err() << ")" << std::endl;
    }
}
