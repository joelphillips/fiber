/*
 * integrator.cpp
 *
 *  Created on: Jan 10, 2012
 *      Author: joel
 */

#include <vector>
#include <assert.h>
#include <cmath>

using namespace std;

struct Point3{
	double x;
	double y;
	double z;
};

double polyeval(const double* coeffs, double x, double y, int order){
	double val = 0;
	double xx = 1;
	double yy = 1;
	for(int i = 0; i <= order; i++){
		for(int j = 0; j <=i; j++ ){
			val += (*coeffs++) * xx * yy;
			yy*=y;
		}
		xx *= x;
	}
	return val;
}

void affinebarycentricmap(const Point3& ref,
						const Point3& v1,
						const Point3& v2,
						const Point3& v3,
						Point3 & out){
	out = Point3(ref.x * v1.x + ref.y * v2.x + ref.z * v3.x,
			ref.x * v1.y + ref.y * v2.y + ref.z * v3.y,
			ref.x * v1.z + ref.y * v2.z + ref.z * v3.z);
}

double laplacekernel(const Point3& a, const Point3& b){
	double dx = a.x - b.x;
	double dy = a.y - b.y;
	double dz = a.z - b.z;
	return pow(dx*dx + dy * dy + dz * dz, -2);
}

class Polynomial{
private:
	vector<double> _coeffs;
	int _order;
public:
	Polynomial(const vector<double>& coeffs, int order):_coeffs(coeffs),_order(order){
		assert(coeffs.size() == (order * (order + 1)) / 2);
	}

	void evaluate(const vector<Point3> & refpoints, vector<double>& out) const {
		out.resize(refpoints.size());
		for(int i = 0; i < refpoints.size(); i++){
			Point3& p = refpoints[i];
			out[i] = polyeval(&_coeffs.front(), p.x, p.y, _order);
		}
	}
};

class AffineBarycentricMap{
private:
	vector<Point3> _vertices;
public:
	AffineBarycentricMap(const vector<Point3>& vertices):_vertices(vertices){}
	void map(const vector<Point3> & refpoints, vector<Point3>& out) const{
		out.resize(refpoints.size());
		for(int i = 0; i < refpoints.size(); i++){
			affinebarycentricmap(refpoints[i], _vertices[0], _vertices[1], _vertices[2], out[i]);
		}
	}
};

class LaplaceKernel{
public:
	void evaluate(const vector<Point3>& a, const vector<Point3>& b, vector<double>& out) const{
		assert(a.size()==b.size());
		out.resize(a.size());
		for(int i = 0; i < a.size(); i++){
			out[i] = laplacekernel(a[i], b[i]);
		}
	}
};

template<typename MAP1, typename MAP2, typename FN1, typename FN2, typename KERNEL>
void integrate(const vector<std::pair<Point3, Point3> >& refpoints,
			const vector<double>& weights,
			const MAP1& mapa,
			const MAP2& mapb,
			const vector<FN1*>& upolys,
			const vector<FN2*>& vpolys,
			const KERNEL& kernel,
			){
	assert(refpoints.size() == weights.size());
	vector<Point3> a;
	vector<Point3> b;
	for(int i = 0; i < refpoints.size(); i++){

	}
}
