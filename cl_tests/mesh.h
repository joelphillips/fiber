// -*-C++-*-

/**
 * \file mesh.h
 * Dummy mesh class
 */

#ifndef __MESH_H
#define __MESH_H

#include "commontypes.h"

/**
 * \brief Simple mesh class.
 *
 * Holds Vertex and element index lists and can be initialised from a
 * text file.
 */
class Mesh {
public:
    /**
     * \brief Default constructor. Creates a zero-length mesh.
     */
    Mesh();

    /**
     * \brief Destructor.
     */
    ~Mesh();

    /**
     * \brief Read a mesh from file.
     * \param fname Mesh file name
     * \note Mesh file is a text file with format
     * \code
     *   <number vertices> <number triangles>
     *   <vtx0.x> <vtx0.y> <vtx0.z>
     *   <vtx1.x> <vtx1.y> <vtx1.z>
     *   ...
     *   <vtxN-1.x> <vtxN-1.y> <vtxN-1.z>
     *   <el0.idx0> <el0.idx1> <el0.idx2>
     *   <el1.idx0> <el1.idx1> <el1.idx2>
     *   ...
     *   <elM-1.idx0> <elM-1.idx1> <elM-1.idx2>
     * \endcode
     */
    void Read (const char *fname);

    /**
     * \brief Returns number of elements
     */
    inline int nEl() const { return nel; }

    /**
     * \brief Returns number of vertices.
     */
    inline int nVtx() const {return nvtx; }

    /**
     * \brief Returns triangle list
     */
    const Triangle *El() const { return el; }

    /**
     * \brief Returns vertex list
     */
    const Point3 *Vtx() const { return vtx; }

    /**
     * \brief returns a triangle from the list.
     * \param i Element index (>= 0)
     */
    Triangle El(int i) const { return el[i]; }

    /**
     * \brief Returns a vertex from the list.
     * \param i Vertex index (>= 0)
     */
    Point3 Vtx(int i) const { return vtx[i]; }

private:
    int nel;
    int nvtx;

    Triangle *el;
    Point3 *vtx;
};

#endif // !__MESH_H
