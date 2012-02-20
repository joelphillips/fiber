// -*-C++-*-

/**
 * \file basisf_lintri.cl
 * OpenCL implementation for shape function computation on a linear
 * triangle type.
 */


/**
 * \brief Evaluate linear basis function on a reference triangle
 * \param [in] p pointer to reference point
 * \param [out] bf array of basis function values [3]
 * \note Definition of reference triangle: v = {{0,0,0},{1,0,0},{0,1,0}}
 * \note z-coordinates of all input points must be 0
 */
void devBasisfLinTri (const Point3 *p,
		      FLOAT *bf)
{
    FLOAT px = p->x;
    FLOAT py = p->y;
    bf[0] = (FLOAT)1 - px - py;
    bf[1] = px;
    bf[2] = py;
}


/**
 * \brief Kernel function: evaluate products of basis functions on
 *   reference triangle
 * \param [in] g_p1 Array of reference points on first triangle [np1]
 * \param [in] g_p2 Array of reference points on second triangle [np2]
 * \param [in] np Number of points in g_p1 and g_p2
 * \param [out] bfprod Array of basis function products [np*3*3]
 * \note Elements in bfprod are ordered such that
 *   bfprod[i*9 + n*3+m]
 *   refers to point i and basis function m in first triangle, point i and
 *   basis function n in second triangle,
 */
__kernel void clBasisfProd (__global const Point3 *g_p1,
			    __global const Point3 *g_p2,
			    int np,
			    __global FLOAT *g_bfprod)
{
    int i = get_global_id(0);

    // range check
    if (i >= np) return;
    
    Point3 p1 = g_p1[i];
    Point3 p2 = g_p2[i];
    FLOAT bf1[3];
    FLOAT bf2[3];

    devBasisfLinTri (&p1, bf1);
    devBasisfLinTri (&p2, bf2);

    int m, n, ofs = i * 9;
    for (n = 0; n < 3; n++)
        for (m = 0; m < 3; m++)
	    g_bfprod[ofs + n*3 + m] = bf1[m] * bf2[n];
}
