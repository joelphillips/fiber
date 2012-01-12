/*
 * integrator.h
 *
 *  Created on: Jan 11, 2012
 *      Author: joel
 */

#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

#include <vector>
#include <assert.h>

struct Point3{
	double x,y,z;
};

class Polynomial{
private:
	std::vector<double> _coeffs;
	int _order;
public:
	Polynomial(const std::vector<double>& coeffs, int order);
	double evaluate(const Point3 & p) const;
};

class AffineBarycentricMap{
private:
	std::vector<Point3> _vertices;
public:
	AffineBarycentricMap(const std::vector<Point3>& vertices);
	void map(const Point3 & refpoint, Point3& out) const;
};

class LaplaceKernel{
public:
	double evaluate(const Point3& p1, Point3& p2) const;
};


template<typename MAP1, typename MAP2, typename FN1, typename FN2, typename KERNEL>
void integrate(const std::vector<std::pair<Point3, Point3> >& refpoints,
			const std::vector<double>& weights,
			const std::vector<MAP1*> map1,
			const std::vector<MAP2*> map2,
			const std::vector<FN1*>& fns1,
			const std::vector<FN2*>& fns2,
			const KERNEL& kernel,
			std::vector<double>& out){
	assert(refpoints.size() == weights.size());
	assert(map1.size() == map2.size());
	int np = refpoints.size();
	int nf1 = fns1.size();
	int nf2 = fns2.size();
	int nm = map1.size();
	out.resize(nf1 * nf2 * nm);
	std::vector<double> fnvals(np*nf1*nf2);
	for(int i = 0; i < np; i++){
		for(int j = 0; j < nf1; j++){
			for(int k = 0; k < nf2; k++){
				fnvals[i *nf1*nf2 + j*nf2 +k] = fns1[j].evaluate(refpoints[i].first)*fns2[k].evaluate(refpoints[i].second);
			}
		}
	}
	for(int n = 0; n < nm; n++){
		for(int i = 0; i < refpoints.size(); i++){
			Point3& rp1 = refpoints[i].first;
			Point3& rp2 = refpoints[i].second;
			Point3 p1,p2;
			map1[n](rp1,p1);
			map2[n](rp2,p2);
			double kval = kernel.evaluate(p1,p2);
			for(int j = 0; j < nf1; j++){
				for(int k = 0; k < nf2; k++){
					out[n * nf1*nf2 + j*nf2 + k] = kval * fnvals[i*nf1*nf2 + j*nf2 + k];
				}
			}
		}
	}
}

#endif /* INTEGRATOR_H_ */
