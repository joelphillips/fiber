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

#include "mapsandfns.cl"

Polynomial::Polynomial(const std::vector<FLOAT>& coeffs, int order):_coeffs(coeffs),_order(order){
	assert(coeffs.size() == ((order+1) * (order + 2)) / 2);
}

double
Polynomial::evaluate(const Point3 & p) const {
	PolyData data;
	data.order = _order;

	return polyeval(&_coeffs.front(), p.x, p.y, _order);
}

AffineBarycentricMap::AffineBarycentricMap(const std::vector<Point3>& vertices):_vertices(vertices){
	const std::vector<Point3>& vs = vertices;
	double ux,uy,uz,vx,vy,vz,nx,ny,nz;
	ux = vs[1].x - vs[0].x;
	uy = vs[1].y - vs[0].y;
	uz = vs[1].z - vs[0].z;
	vx = vs[2].x - vs[0].x;
	vy = vs[2].y - vs[0].y;
	vz = vs[2].z - vs[0].z;
	nx = uy*vz - uz*vy;
	ny = uz*vx - ux*vz;
	nz = ux*vy - uy*vx;
	_detjac = sqrt(nx*nx + ny*ny + nz*nz);
}

void
AffineBarycentricMap::map(const Point3 & refpoint, Point3& out) const{
	Point3Triple vs = {_vertices[0], _vertices[1], _vertices[2]};
	affinebarycentricmap(refpoint, vs, out);
}

double
LaplaceKernel::evaluate(const Point3& p1, Point3& p2) const{
	return laplacekernel(p1, p2);
}
