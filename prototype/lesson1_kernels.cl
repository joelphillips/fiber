#if defined(cl_khr_fp64)  // Khronos extension available?
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#elif defined(cl_amd_fp64)  // AMD extension available?
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#endif

#pragma OPENCL EXTENSION cl_amd_fp64 : enable

//__kernel void hello(__global char * in, __global char * out)
//{
//    size_t tid = get_global_id(0);
//    out[tid] = hw[tid] + in[tid];
//}
//
//
//__kernel void hello( __global char * out)
//{
//    size_t tid = get_global_id(0);
//    out[tid] = hw[tid];
//}

__kernel void sum(__global FLOAT * v1, __global FLOAT * v2, __global FLOAT * r){
	size_t tid = get_global_id(0);
	r[tid] = 4.0;//v1[tid] + v2[tid];
}
