/*
 * integrator.cl
 *
 *  Created on: Jan 12, 2012
 *      Author: joel
 */


void integrate(__global Point3Pair * refpoints,
			__global double * weights,
			uint npoints,
			__global Point3Triple * map1data,
			__global Point3Triple * map2data,
			__global double * fn1coeffs,
			uint fn1order,
			__global
			const std::vector<FN2*>& fns2,
			const KERNEL& kernel,
			std::vector<double>& out){
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


