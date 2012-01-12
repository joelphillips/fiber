/*
 * integrategpu.cpp
 *
 *  Created on: Jan 12, 2012
 *      Author: joel
 */

#include "integrategpu.hpp"

#include "openclutil.h"


void integrategpu(const std::vector<std::pair<Point3, Point3> >& refpoints,
			const std::vector<double>& weights,
			const std::vector<AffineBarycentricMap*> map1,
			const std::vector<AffineBarycentricMap*> map2,
			const std::vector<Polynomial*>& fns1,
			const std::vector<Polynomial*>& fns2,
			const LaplaceKernel & kernel,
			std::vector<double>& out){

	ContextWrapper cw;


}
