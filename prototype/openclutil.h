/*
 * openclutil.h
 *
 *  Created on: Jan 9, 2012
 *      Author: joel
 */

#ifndef OPENCLUTIL_H_
#define OPENCLUTIL_H_

#define __CL_ENABLE_EXCEPTIONS
#define __NO_STD_VECTOR // Use cl::vector instead of STL version
#include <cl.hpp>
#include <string>
#include <vector>

class ContextWrapper{
private:
	cl::Context _context;
	cl::vector<cl::Device> _devices;
public:
	ContextWrapper();
	cl::Program getProgramFromFiles(const std::vector<std::string>& filenames);
	cl::Context getContext(){return _context;}
	cl::CommandQueue getQueue();
};



#endif /* OPENCLUTIL_H_ */
