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

		char * outH = new char[hw.length()+1];

		cl::Buffer outCL(
			cw.getContext(),
			CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
			hw.length()+1,
			outH);

		cl::Program program = cw.getProgramFromFiles(std::vector<std::string>(1, "prototype/lesson1_kernels.cl"));

		cl::Kernel kernel(program, "hello");
		kernel.setArg(0, outCL);

		cl::CommandQueue queue = cw.getQueue();
		cl::Event event;
		queue.enqueueNDRangeKernel(
			kernel,
			cl::NullRange,
			cl::NDRange(hw.length()+1),
			cl::NDRange(1, 1),
			NULL,
			&event);

		event.wait();
		queue.enqueueReadBuffer(
			outCL,
			CL_TRUE,
			0,
			hw.length()+1,
			outH);
		std::cout << outH;
	}
	catch (cl::Error err) {
		std::cerr << err.what() << "(" << err.err() << ")" << std::endl;
    }
}
