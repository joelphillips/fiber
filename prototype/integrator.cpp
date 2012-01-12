/*
 * integrator.cpp
 *
 *  Created on: Jan 10, 2012
 *      Author: joel
 */

#include "integrator.hpp"

#include <cmath>
#include <iostream>
using namespace std;



double polyeval(const double* coeffs, double x, double y, int order){
	double val = 0;
	double yy = 1;
	for(int i = 0; i <= order; i++){
		double xx = 1;
		for(int j = 0; j <=order - i; j++ ){
			val += (*coeffs++) * xx * yy;
			xx*=x;
		}
		yy *= y;
	}
	return val;
}

void affinebarycentricmap(const Point3& ref,
						const Point3& v1,
						const Point3& v2,
						const Point3& v3,
						Point3 & out){
	out.x = ref.x * v1.x + ref.y * v2.x + ref.z * v3.x;
	out.y = ref.x * v1.y + ref.y * v2.y + ref.z * v3.y;
	out.z = ref.x * v1.z + ref.y * v2.z + ref.z * v3.z;
}

double laplacekernel(const Point3& a, const Point3& b){
	double dx = a.x - b.x;
	double dy = a.y - b.y;
	double dz = a.z - b.z;
	return pow(dx*dx + dy * dy + dz * dz, -1.0/2);
}

Polynomial::Polynomial(const std::vector<double>& coeffs, int order):_coeffs(coeffs),_order(order){

	assert(coeffs.size() == ((order+1) * (order + 2)) / 2);
}

double
Polynomial::evaluate(const Point3 & p) const {
	return polyeval(&_coeffs.front(), p.x, p.y, _order);
}

AffineBarycentricMap::AffineBarycentricMap(const std::vector<Point3>& vertices):_vertices(vertices){}

void
AffineBarycentricMap::map(const Point3 & refpoint, Point3& out) const{
	affinebarycentricmap(refpoint, _vertices[0], _vertices[1], _vertices[2], out);
}

double
LaplaceKernel::evaluate(const Point3& p1, Point3& p2) const{
	return laplacekernel(p1, p2);
}
