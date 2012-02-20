// -*-C++-*-

/**
 * \brief Integration kernel
 * \param g_vtx1 Global vertex coordinates for source triangles [ntri1*3]
 * \param ntri1 Number of source triangles
 * \param g_vtx2 Global vertex coordinates for target triangles [ntri2*3]
 * \param ntri2 Number of target triangles
 * \param g_refpt Quadrature points in barycentric coordinates [nquad]
 * \param g_weight Quadrature weights [nquad]
 * \param nquad Number of quadrature points
 * \param g_basisf Array of basis function values for reference element [nquad*nquad*3*3]
 * \param g_val Array of integration results [ntri1*ntri2*3*3]
 */
__kernel void clIntegrate (__global const Point3 *g_vtx1,
			   int ntri1,
			   __global const Point3 *g_vtx2,
			   int ntri2,
			   __global const Point3 *g_refpt1,
			   __global const Point3 *g_refpt2,
			   __global const FLOAT *g_weight,
			   int nquad,
			   __global const FLOAT *g_basisf,
			   __global FLOAT *g_val)
{
    int i, j, n, el1, el2, qpt;

    el1 = get_global_id(0);    // index source triangle
    el2 = get_global_id(1);    // index target triangle
    
    // range checks
    if (el1 > ntri1) return;
    if (el2 > ntri2) return;

    Point3 vtx1[3];
    Point3 vtx2[3];
    Point3 refpt1, refpt2;
    Point3 pt1, pt2;
    
    n = el1*ntri2 + el2;

    // copy vertices from global to local memory
    for (i = 0; i < 3; i++) {
        vtx1[i] = g_vtx1[el1*3+i];
	vtx2[i] = g_vtx2[el2*3+i];
    }

    // basis function mapper
    FLOAT d1 = devDetJac (vtx1);
    FLOAT d2 = devDetJac (vtx2);
    FLOAT d12 = d1*d2;
    FLOAT val[3*3];
    FLOAT kval;

    for (i = 0; i < 3*3; i++)
        val[i] = 0.0f;

    for (qpt = 0; qpt < nquad; qpt++) {
        refpt1 = g_refpt1[qpt];
	refpt2 = g_refpt2[qpt];

	// map points from reference to global coordinates
	devMapPoint (vtx1, &refpt1, &pt1);
	devMapPoint (vtx2, &refpt2, &pt2);

	kval = devKerneval (&pt1, &pt2) * d12 * g_weight[qpt];

	for (i = 0; i < 3; i++) {
	    for (j = 0; j < 3; j++) {
	        val[j + i*3] += kval * g_basisf[j + i*3 + qpt*3*3];
	    }
	}
    }

    // copy results back to global memory
    for (i = 0; i < 3*3; i++)
        g_val[n*9 + i] = val[i];
}
