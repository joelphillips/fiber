#include <stdlib.h>
#include "quadrature.hpp"

Quadrature::Quadrature ()
{
    // Dummy: Make up random points
    np = 100;
    absc = new Point3[np];
    wght = new FLOAT[np];

    for (int i = 0; i < np; i++) {
        FLOAT lambda12 = (FLOAT)rand()/(FLOAT)RAND_MAX;
	FLOAT lambda1 = (FLOAT)rand()/(FLOAT)RAND_MAX * lambda12;
	FLOAT lambda2 = lambda12 - lambda1;
	absc[i].x = lambda1;
	absc[i].y = lambda2;
	absc[i].z = (FLOAT)0;
	wght[i] = (FLOAT)1;
    }
}

Quadrature::~Quadrature ()
{
    delete []absc;
    delete []wght;
}

int Quadrature::AbscissaeAndWeights (const Point3 **pt1, const Point3 **pt2, const FLOAT **w) const
{
    *pt1 = absc;
    *pt2 = absc;
    *w  = wght;
    return np;
}
