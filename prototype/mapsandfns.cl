/*
 * mapsandfns.cl
 *
 *  Created on: Jan 12, 2012
 *      Author: joel
 */


double polyeval(const PolyData& data, FLOAT x, FLOAT y){
	FLOAT val = 0;
	FLOAT yy = 1;
	for(int i = 0; i <= data.order; i++){
		FLOAT xx = 1;
		for(int j = 0; j <=data.order - i; j++ ){
			val += (*data.coeffs++) * xx * yy;
			xx*=x;
		}
		yy *= y;
	}
	return val;
}

void affinebarycentricmap(const Point3& ref,
						const Point3Triple& vs,
						Point3 & out){
	out.x = ref.x * vs.p1.x + ref.y * vs.p2.x + ref.z * vs.p3.x;
	out.y = ref.x * vs.p1.y + ref.y * vs.p2.y + ref.z * vs.p3.y;
	out.z = ref.x * vs.p1.z + ref.y * vs.p2.z + ref.z * vs.p3.z;
}

FLOAT laplacekernel(const Point3& a, const Point3& b){
	FLOAT dx = a.x - b.x;
	FLOAT dy = a.y - b.y;
	FLOAT dz = a.z - b.z;
	return pow(dx*dx + dy * dy + dz * dz, -1.0f/2);
}
