#include <stdio.h>
#include <string>
#include <iostream>

#include "cll.h"
#include "util.h"

CL::CL()
{
    printf("Initialize OpenCL object and context\n");
    //setup devices and context
    
    //this function is defined in util.cpp
    //it comes from the NVIDIA SDK example code
    ///err = oclGetPlatformID(&platform);
    //oclErrorString is also defined in util.cpp and comes from the NVIDIA SDK
    ///printf("oclGetPlatformID: %s\n", oclErrorString(err));
    std::vector<cl::Platform> platforms;
    err = cl::Platform::get(&platforms);
    printf("cl::Platform::get(): %s\n", oclErrorString(err));
    printf("number of platforms: %d\n", platforms.size());
    if (platforms.size() == 0) {
        printf("Platform size 0\n");
    }
 

    // Get the number of GPU devices available to the platform
    // we should probably expose the device type to the user
    // the other common option is CL_DEVICE_TYPE_CPU
    ///err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
    ///printf("clGetDeviceIDs (get number of devices): %s\n", oclErrorString(err));


    // Create the device list
    ///devices = new cl_device_id [numDevices];
    ///err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, numDevices, devices, NULL);
    ///printf("clGetDeviceIDs (create device list): %s\n", oclErrorString(err));
 

    //for right now we just use the first available device
    //later you may have criteria (such as support for different extensions)
    //that you want to use to select the device
    deviceUsed = 0;
    //create the context
    ///context = clCreateContext(0, 1, &devices[deviceUsed], NULL, NULL, &err);
    //context properties will be important later, for now we go with defualts
    cl_context_properties properties[] =
        { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(), 0};

    context = cl::Context(CL_DEVICE_TYPE_GPU, properties);
    devices = context.getInfo<CL_CONTEXT_DEVICES>();
    printf("number of devices %d\n", devices.size());
    
    //create the command queue we will use to execute OpenCL commands
    ///command_queue = clCreateCommandQueue(context, devices[deviceUsed], 0, &err);
    try{
        queue = cl::CommandQueue(context, devices[deviceUsed], 0, &err);
    }
    catch (cl::Error er) {
        printf("ERROR: %s(%d)\n", er.what(), er.err());
    }

}

CL::~CL()
{
    /*
printf("Releasing OpenCL memory\n");
if(kernel)clReleaseKernel(kernel);
if(program)clReleaseProgram(program);
if(command_queue)clReleaseCommandQueue(command_queue);
//need to release any other OpenCL memory objects here
if(cl_a)clReleaseMemObject(cl_a);
if(cl_b)clReleaseMemObject(cl_b);
if(cl_c)clReleaseMemObject(cl_c);

if(context)clReleaseContext(context);
if(devices)delete(devices);
printf("OpenCL memory released\n");
*/
}


void CL::loadProgram(std::string kernel_source)
{
    //Program Setup
    int pl;
    //size_t program_length;
    printf("load the program\n");
    
    pl = kernel_source.size();
    printf("kernel size: %d\n", pl);
    //printf("kernel: \n %s\n", kernel_source.c_str());
    try
    {
        cl::Program::Sources source(1,
            std::make_pair(kernel_source.c_str(), pl));
        program = cl::Program(context, source);
    }
    catch (cl::Error er) {
        printf("ERROR: %s(%s)\n", er.what(), oclErrorString(er.err()));
    }

    printf("build program\n");
    try
    {
        err = program.build(devices);
    }
    catch (cl::Error er) {
        printf("program.build: %s\n", oclErrorString(er.err()));
    //if(err != CL_SUCCESS){
    }
    printf("done building program\n");
std::cout << "Build Status: " << program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(devices[0]) << std::endl;
std::cout << "Build Options:\t" << program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(devices[0]) << std::endl;
std::cout << "Build Log:\t " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << std::endl;

}


void CL::loadProgramFromFragments (std::vector<std::string> sources)
{
    //Program Setup
    int pl;
    //size_t program_length;
    printf("load the program\n");

    try {
        cl::Program::Sources source;
	for (int i = 0; i < sources.size(); i++) {
	  source.push_back (std::make_pair (sources[i].c_str(),
					      sources[i].size()));
	}
	program = cl::Program(context, source);
    }
    catch (cl::Error er) {
        printf("ERROR: %s(%s)\n", er.what(), oclErrorString(er.err()));
    }

    printf("build program\n");
    try
    {
        err = program.build(devices);
    }
    catch (cl::Error er) {
        printf("program.build: %s\n", oclErrorString(er.err()));
    //if(err != CL_SUCCESS){
    }
    printf("done building program\n");
std::cout << "Build Status: " << program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(devices[0]) << std::endl;
std::cout << "Build Options:\t" << program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(devices[0]) << std::endl;
std::cout << "Build Log:\t " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << std::endl;
}


void CL::PushGeometry (const Mesh &mesh)
{
    // Allocate the buffers
    nmeshvtx = mesh.nVtx();
    cl_meshvtx = cl::Buffer (context, CL_MEM_READ_ONLY,
        nmeshvtx*sizeof(Point3), NULL, &err);
    nmeshels = mesh.nEl();
    cl_meshels = cl::Buffer (context, CL_MEM_READ_ONLY,
        nmeshels*sizeof(Triangle), NULL, &err);
			     
    // Copy the geometry data to device memory
    err = queue.enqueueWriteBuffer (cl_meshvtx, CL_TRUE, 0,
        nmeshvtx*sizeof(Point3), mesh.Vtx(), NULL, &event);
    err = queue.enqueueWriteBuffer (cl_meshels, CL_TRUE, 0,
	nmeshels*sizeof(Triangle), mesh.El(), NULL, &event);

    queue.finish();
}

void CL::FreeGeometry ()
{
    // TODO
}

void CL::PushQuadrature (const Quadrature &quad)
{
    const Point3 *refpt1, *refpt2;
    const FLOAT *w;

    np = quad.AbscissaeAndWeights (&refpt1, &refpt2, &w);
    cl_refpt1 = cl::Buffer (context, CL_MEM_READ_ONLY,
        np*sizeof(Point3), NULL, &err);
    cl_refpt2 = cl::Buffer (context, CL_MEM_READ_ONLY,
        np*sizeof(Point3), NULL, &err);
    cl_weight = cl::Buffer (context, CL_MEM_READ_ONLY,
	np*sizeof(FLOAT), NULL, &err);

    err = queue.enqueueWriteBuffer(cl_refpt1, CL_TRUE, 0,
        sizeof(Point3)*np, refpt1, NULL, &event);
    err = queue.enqueueWriteBuffer(cl_refpt2, CL_TRUE, 0,
        sizeof(Point3)*np, refpt2, NULL, &event);
    err = queue.enqueueWriteBuffer(cl_weight, CL_TRUE, 0,
	sizeof(FLOAT)*np, w, NULL, &event);

    queue.finish();
}
