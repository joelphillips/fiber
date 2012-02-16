function mesh2dat (hmesh, fname)

[vtx idx]=toastMeshData(hmesh);
nnd = size(vtx,1);
nel = size(idx,1);
dim = size(vtx,2);

fid = fopen(fname,'w');
fprintf (fid, '%d %d\n', nnd, nel);

fprintf (fid, '%g %g %g\n', vtx');
fprintf (fid, '%d %d %d\n', idx');

fclose (fid);
