/*
 * openclutil.cpp
 *
 *  Created on: Jan 9, 2012
 *      Author: joel
 */

#include "openclutil.h"
#include <fstream>
#include <iostream>
#include <iterator>

cl::Platform getPlatform(){
	cl::vector< cl::Platform > platformList;
	cl::Platform::get(&platformList);
	std::cout<<"Number of platforms: "<<platformList.size()<<std::endl;
	std::string platformVendor;
	for(int i = 0; i < platformList.size(); i++){
		platformList[i].getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &platformVendor);
		std::cout << "Platform is by: " << platformVendor << "\n";
	}
	return platformList[0];
}

ContextWrapper::ContextWrapper(){
	cl::Platform platform = getPlatform();
	cl_context_properties cprops[3] =
			{CL_CONTEXT_PLATFORM, (cl_context_properties)(platform)(), 0};

	_context = cl::Context(CL_DEVICE_TYPE_GPU,
						cprops,
						NULL,
						NULL);
	_devices = _context.getInfo<CL_CONTEXT_DEVICES>();
//	platform.getDevices(CL_DEVICE_TYPE_GPU, &_devices);
	std::cout<<"Number of devices: "<<_devices.size()<<std::endl;
	std::string deviceinfo;
	for(int i = 0; i < _devices.size(); i++){
		_devices[i].getInfo(CL_DEVICE_NAME, &deviceinfo);
		std::cout<<deviceinfo<<std::endl;
		_devices[i].getInfo(CL_DEVICE_AVAILABLE, &deviceinfo);
		std::cout<<deviceinfo<<std::endl;
	}

}

cl::Program
ContextWrapper::getProgramFromFiles(const std::vector<std::string>& filenames){

	cl::Program::Sources sources(filenames.size());
	std::vector<std::string> progs(filenames.size());
	for(int i = 0; i < filenames.size(); i++){
		std::ifstream file(filenames[i].c_str());
		if (!file.is_open()) throw std::exception();
		std::string prog(std::istreambuf_iterator<char>(file),
						(std::istreambuf_iterator<char>()));
		progs[i] = prog;
		sources[i] = std::make_pair(prog.c_str(), prog.length()+1);
	}

	cl::Program program(_context, sources);
	program.build(_devices,"");
	return program;
}

cl::CommandQueue
ContextWrapper::getQueue(){
	return cl::CommandQueue(_context, _devices[0], 0);
}

