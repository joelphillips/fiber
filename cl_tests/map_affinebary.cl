// -*-C++-*-

/**
 * \file map_affinebary.cl
 * OpenCL implementation of mapping functions for affine barycentric
 *    coordinates.
 */

/**
 * \brief Affine barycentric map of a point in a triangle
 * \param [in] vtx array of triangle vertices [3]
 * \param [in] ref pointer to reference point in barycentric coordinates
 * \param [out] pt pointer to mapped point in global coordinates
 */
void devMapPoint (const Point3 *vtx,
		  const Point3 *ref,
		  Point3 *pt)
{
    pt->x = ref->x*vtx[0].x + ref->y*vtx[1].x + ref->z*vtx[2].x;
    pt->y = ref->x*vtx[0].y + ref->y*vtx[1].y + ref->z*vtx[2].y;
    pt->z = ref->x*vtx[0].z + ref->y*vtx[1].z + ref->z*vtx[2].z;
}


/**
 * \brief Determinant of Jacobian for linear triangle
 * \param vtx array of triangle vertices [3]
 * \return Determinant of Jacobian
 */
FLOAT devDetJac (const Point3 *vtx)
{
    FLOAT ux = vtx[1].x - vtx[0].x;
    FLOAT uy = vtx[1].y - vtx[0].y;
    FLOAT uz = vtx[1].z - vtx[0].z;
    FLOAT vx = vtx[2].x - vtx[0].x;
    FLOAT vy = vtx[2].y - vtx[0].y;
    FLOAT vz = vtx[2].z - vtx[0].z;
    FLOAT nx = uy*vz - uz*vy;
    FLOAT ny = uz*vx - ux*vz;
    FLOAT nz = ux*vy - uy*vx;
    return sqrt (nx*nx + ny*ny + nz*nz);
}


