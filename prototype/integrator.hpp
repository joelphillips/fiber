/*
 * integrator.h
 *
 *  Created on: Jan 11, 2012
 *      Author: joel
 */

#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

#include <vector>
#include <utility>
#include <assert.h>

#include "commontypes.hpp"

class Polynomial{
private:
	std::vector<FLOAT> _coeffs;
	int _order;
public:
	Polynomial(const std::vector<FLOAT>& coeffs, int order);
	double evaluate(const Point3 & p) const;
};

class AffineBarycentricMap{
private:
	std::vector<Point3> _vertices;
	double _detjac;
public:
	AffineBarycentricMap(const std::vector<Point3>& vertices);
	void map(const Point3 & refpoint, Point3& out) const;
	double detjac(const Point3 & refpoint)const {return _detjac;};
};

class LaplaceKernel{
public:
	double evaluate(const Point3& p1, Point3& p2) const;
};


template<typename MAP1, typename MAP2, typename FN1, typename FN2, typename KERNEL>
void integrate(const std::vector<std::pair<Point3, Point3> >& refpoints,
			const std::vector<FLOAT>& weights,
			const std::vector<MAP1*> map1,
			const std::vector<MAP2*> map2,
			const std::vector<FN1*>& fns1,
			const std::vector<FN2*>& fns2,
			const KERNEL& kernel,
			std::vector<FLOAT>& out){
	assert(refpoints.size() == weights.size());
	int np = refpoints.size();
	int nf1 = fns1.size();
	int nf2 = fns2.size();
	int nm1 = map1.size();
	int nm2 = map2.size();
	out.resize(nf1 * nf2 * nm1 * nm2);
	std::vector<double> fnvals(np*nf1*nf2);
	for(int i = 0; i < np; i++){
		for(int j = 0; j < nf1; j++){
			for(int k = 0; k < nf2; k++){
				fnvals[i *nf1*nf2 + j*nf2 +k] = fns1[j]->evaluate(refpoints[i].first)*fns2[k]->evaluate(refpoints[i].second);
			}
		}
	}
	for(int n1 = 0; n1 < nm1; n1++){
		for(int n2 = 0; n2 < nm2; n2++){
			int n = n1 *nm2 + n2;
			for(int i = 0; i < refpoints.size(); i++){
				const Point3& rp1 = refpoints[i].first;
				const Point3& rp2 = refpoints[i].second;
				Point3 p1,p2;
				map1[n1]->map(rp1,p1);
				map2[n2]->map(rp2,p2);
				double d1 = map1[n1]->detjac(rp1);
				double d2 = map2[n2]->detjac(rp2);
				double kval = kernel.evaluate(p1,p2) * d1 * d2* weights[i];
				for(int j = 0; j < nf1; j++){
					for(int k = 0; k < nf2; k++){
						out[n * nf1*nf2 + j*nf2 + k] += kval * fnvals[i*nf1*nf2 + j*nf2 + k];
					}
				}
			}
		}
	}
}


#endif /* INTEGRATOR_H_ */
