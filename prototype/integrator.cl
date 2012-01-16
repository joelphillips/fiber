/*
 * integrator.cl
 *
 *  Created on: Jan 12, 2012
 *      Author: joel
 */


void integrate(__global Point3Pair * refpoints,
			__global FLOAT * weights,
			uint np,
			__global Point3Triple * map1data,
			uint nm1,
			__global Point3Triple * map2data,
			uint nm2,
			__global FNDATA * fn1coeffs,
			unit nf1,
			__global FNDATA * fn2coeffs,
			unit nf2,
			__global FLOAT* out
			__local FLOAT* fnvals){
	int np = refpoints.size();

//	out.resize(nf1 * nf2 * nm1 * nm2);
//	std::vector<double> fnvals(np*nf1*nf2);
	size_t gid = get_global_id(0);
	size_t lid = get_local_id(0);
	size_t lsize = get_local_size(0);

	size_t nfnpts = nf1 * nf2 * np;
	for(size_t i = lid; i < nfnpts; i+=lsize){
		size_t fn1id = i % nf1;
		size_t fn2id = (i / nf1) % nf2;
		size_t ptid = (i / (nf1 * nf2));
		pt1 = refpoints[ptid]->first;
		pt2 = refpoints[ptid]->second;
		fnvals[i] = polyeval(fn1coeffs[fn1id], pt1.x, pt1.y) *
				polyeval(fn2coeffs[fn2id], pt2.x, pt2.x);
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


