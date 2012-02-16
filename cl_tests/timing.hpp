#ifndef __TIMING_H
#define __TIMING_H

double tic();
double toc();
double toc(double tic);

double walltic();
double walltoc();
double walltoc(double walltic);

#ifdef DBG_TIMING
#define TIC (tic())
#define TOC(t) (t=toc())
#define TOCADD(t) (t+=toc())
#else
#define TIC
#define TOC(t)
#define TOCADD(t)
#endif

#endif // !__TIMING_H
