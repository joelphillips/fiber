/*
 * point.hpp
 *
 *  Created on: Jan 12, 2012
 *      Author: joel
 */

#ifndef POINT_HPP_
#define POINT_HPP_

#define FLOAT float

typedef struct {
	FLOAT x,y,z;
} Point3;

typedef struct {
	Point3 first, second;
} Point3Pair;

typedef struct {
	Point3 p1,p2,p3;
} Point3Triple;

typedef struct {
    int idx[3];
} Triangle;

#define MAXORDER 6
typedef struct {
	int order;
	FLOAT *coeffs;
} PolyData;

//typedef struct {
//	int order;
//	__global FLOAT *coeffs;
//} gPolyData;

#endif /* POINT_HPP_ */
