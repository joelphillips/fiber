/*
 * integrategpu.hpp
 *
 *  Created on: Jan 12, 2012
 *      Author: joel
 */

#ifndef INTEGRATEGPU_HPP_
#define INTEGRATEGPU_HPP_

#include "integrator.hpp"

void integrategpu(const std::vector<std::pair<Point3, Point3> >& refpoints,
			const std::vector<FLOAT>& weights,
			const std::vector<AffineBarycentricMap*> map1,
			const std::vector<AffineBarycentricMap*> map2,
			const std::vector<Polynomial*>& fns1,
			const std::vector<Polynomial*>& fns2,
			const LaplaceKernel & kernel,
			std::vector<double>& out);

#endif /* INTEGRATEGPU_HPP_ */
