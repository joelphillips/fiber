/*
 * point.hpp
 *
 *  Created on: Jan 12, 2012
 *      Author: joel
 */

#ifndef POINT_HPP_
#define POINT_HPP_

#define FLOAT float

struct Point3{
	FLOAT x,y,z;
};

struct Point3Pair{
	Point3 first, second;
};

struct Point3Triple{
	Point3 p1,p2,p3;
};


#define MAXORDER 6
struct PolyData{
	int order;
	FLOAT coeffs[(MAXORDER+1)*(MAXORDER+2)/2];
};

#endif /* POINT_HPP_ */
