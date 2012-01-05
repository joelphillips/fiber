/*
 * openclexperiments.cpp
 *
 *  Created on: Jan 5, 2012
 *      Author: joel
 */
#include <utility>
#define __NO_STD_VECTOR // Use cl::vector instead of STL version
#include <cl.hpp>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>


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
	cl_int err;
	cl::vector< cl::Platform > platformList;
	cl::Platform::get(&platformList);
	std::cout<<platformList.size()<<std::endl;
    std::string platformVendor;
    for(int i = 0; i < platformList.size(); i++){
    	platformList[i].getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &platformVendor);
    	std::cout << "Platform is by: " << platformVendor << "\n";
    }
    cl_context_properties cprops[3] =
            {CL_CONTEXT_PLATFORM, (cl_context_properties)(platformList[0])(), 0};

	cl::Context context(
	   CL_DEVICE_TYPE_CPU,
	   cprops,
	   NULL,
	   NULL,
	   &err);

	checkErr(err, "Context::Context()");

	char * outH = new char[hw.length()+1];

	cl::Buffer outCL(
		context,
		CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
		hw.length()+1,
		outH,
		&err);
	checkErr(err, "Buffer::Buffer()");

}
