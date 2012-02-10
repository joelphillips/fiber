/*
 * integrator.cl
 *
 *  Created on: Jan 12, 2012
 *      Author: joel
 */


/**
 * Expects the following functions to be defined
 * void map(MAPDATA1, const Point3& refpoint, Point3& mappedpoint)
 * void map(MAPDATA2, const Point3& refpoint, Point3& mappedpoint)
 * double weightedpullback(MAPDATA1, const Point3& refpoint, double refval)
 * double weightedpullback(MAPDATA2, const Point3& refpoint, double refval)
 *
 * map1data and map2data should have lengths nm1 and nm2, where nm1, nm2 = get_global_size([1,2]).
 * out should have size nf1 * nf2 * nm1 * nm2
 */
void integrate(__global Point3Pair * refpoints,
			__global FLOAT * weights,
			uint np,
			__global MAPDATA1 * map1data,
			__global MAPDATA2 * map2data,
			__global FNDATA * fn1data,
			unit nf1,
			__global FNDATA * fn2data,
			unit nf2,
			__global FLOAT* out
			__local FLOAT* fnvals){
	int np = refpoints.size();

	size_t im1 = get_global_id(0);
	size_t im2 = get_global_id(1);
	size_t nm1 = get_global_size(0)
	size_t nm2 = get_global_size(1)

	size_t n = im1 * nm2 + im2;

	double c = 0;
	for(int i = 0; i < refpoints.size(); i++){
		const Point3& rp1 = refpoints[i].first;
		const Point3& rp2 = refpoints[i].second;
		Point3 p1,p2;
		md1 = map1data[im1]
		md2 = map2data[im2]
		map(md1, rp1, p1);
		map(md2, rp2, p2);
		double kval = kernel.evaluate(p1,p2);

		for(int j = 0; j < nf1; j++){
			double f1val = weightedpullback(md1, rp1, fn1data[j])
			for(int k = 0; k < nf2; k++){
				double f2val = weightedpullback(md2, rp2, fn1data[k])
				out[n * nf1*nf2 + j*nf2 + k] += kval * fnvals[i*nf1*nf2 + j*nf2 + k];
			}
		}
	}
}


//	size_t nfnpts = nf1 * nf2 * np;
//	for(size_t i = lid; i < nfnpts; i+=lsize){
//		size_t fn1id = i % nf1;
//		size_t fn2id = (i / nf1) % nf2;
//		size_t ptid = (i / (nf1 * nf2));
//		pt1 = refpoints[ptid]->first;
//		pt2 = refpoints[ptid]->second;
//		fnvals[i] = polyeval(fn1coeffs[fn1id], pt1.x, pt1.y) *
//				polyeval(fn2coeffs[fn2id], pt2.x, pt2.x);
//	}

