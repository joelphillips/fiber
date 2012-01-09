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
	std::cout<<"Number of platforms: "<<platformList.size()<<std::endl;
    std::string platformVendor;
    for(int i = 0; i < platformList.size(); i++){
    	platformList[i].getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &platformVendor);
    	std::cout << "Platform is by: " << platformVendor << "\n";
    }
    cl_context_properties cprops[3] =
            {CL_CONTEXT_PLATFORM, (cl_context_properties)(platformList[0])(), 0};

	cl::Context context(
	   CL_DEVICE_TYPE_ALL,
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

	cl::vector<cl::Device> devices;
	devices = context.getInfo<CL_CONTEXT_DEVICES>();
	std::cout<<"Number of devices: "<<devices.size()<<std::endl;
	std::string devicetype;
	for(int i = 0; i < devices.size(); i++){
		devices[i].getInfo(CL_DEVICE_NAME, &devicetype);
		std::cout<<devicetype<<std::endl;
	}

	std::ifstream file("prototype/lesson1_kernels.cl");
	checkErr(file.is_open() ? CL_SUCCESS:-1, "lesson1_kernel.cl");
	std::string prog(std::istreambuf_iterator<char>(file),
						(std::istreambuf_iterator<char>()));

	cl::Program::Sources source(1, std::make_pair(prog.c_str(), prog.length()+1));
	cl::Program program(context, source);
	err = program.build(devices,""); checkErr(err, "Program::build()");

    cl::Kernel kernel(program, "hello", &err);
    checkErr(err, "Kernel::Kernel()");
    err = kernel.setArg(0, outCL);
    checkErr(err, "Kernel::setArg()");

    cl::CommandQueue queue(context, devices[0], 0, &err);
	checkErr(err, "CommandQueue::CommandQueue()");
	cl::Event event;
	err = queue.enqueueNDRangeKernel(
		kernel,
		cl::NullRange,
		cl::NDRange(hw.length()+1),
		cl::NDRange(1, 1),
		NULL,
		&event);
	checkErr(err, "CommandQueue::enqueueNDRangeKernel()");

    event.wait();
    err = queue.enqueueReadBuffer(
        outCL,
        CL_TRUE,
        0,
        hw.length()+1,
        outH);
    checkErr(err, "ComamndQueue::enqueueReadBuffer()");
    std::cout << outH;
}
