// Dummy mesh reader

#ifndef __MESH_H
#define __MESH_H

#include "commontypes.h"

class Mesh {
public:
    Mesh();
    ~Mesh();
    void Read (const char *fname);
    inline int nEl() const { return nel; }
    inline int nVtx() const {return nvtx; }
    const Triangle *El() const { return el; }
    const Point3 *Vtx() const { return vtx; }
    Triangle El(int i) const { return el[i]; }
    Point3 Vtx(int i) const { return vtx[i]; }

private:
    int nel;
    int nvtx;

    Triangle *el;
    Point3 *vtx;
};

#endif // !__MESH_H
