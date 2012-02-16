#include <iostream>
#include <fstream>
#include "mesh.h"

Mesh::Mesh ()
{
    nel = 0;
    nvtx = 0;
    el = NULL;
    vtx = NULL;
}

Mesh::~Mesh()
{
    if (nel) delete []el;
    if (nvtx) delete []vtx;
}

void Mesh::Read (const char *fname)
{
  int i, j;

    if (nel) delete []el;
    if (nvtx) delete []vtx;

    std::ifstream ifs(fname);
    ifs >> nvtx >> nel;

    vtx = new Point3[nvtx];
    el = new Triangle[nel];

    for (i = 0; i < nvtx; i++)
        ifs >> vtx[i].x >> vtx[i].y >> vtx[i].z;

    for (i = 0; i < nel; i++)
        for (j = 0; j < 3; j++)
	    ifs >> el[i].idx[j];
}

