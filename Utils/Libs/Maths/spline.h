#ifndef __SPLINE_H__
#define __SPLINE_H__

#include "basetypes.h"

template <class T>
void spline(real x, int nknots, T *knot)
{
	const float cr[16]={-0.5,  1.5, -1.5, 0.5,
					  1.0, -2.5,  2.0, -0.5,
					  -0.5,  0.0,  0.5, 0.0,
					  0.0,  1.0,  0.0, 0.0};

	int nspans=nknots-3;

	if (nspans<1)
	{
		ASSERT("spline has to few knots");
		return 0;
	}

	if (x<0) x=0;
	if (x>1) x=1;
	x*=nspans;

	int span=(int)x;
	if (span>=nknots-3)
	span=nknots-3;

	x-=span;
	knot+=span;

	float c3 = cr[0]*knot[0]  + cr[1]*knot[1]  + cr[2]*knot[2]  + cr[3]*knot[3];
	float c2 = cr[4]*knot[0]  + cr[5]*knot[1]  + cr[6]*knot[2]  + cr[7]*knot[3];
	float c1 = cr[8]*knot[0]  + cr[9]*knot[1]  + cr[10]*knot[2] + cr[11]*knot[3];
	float c0 = cr[12]*knot[0] + cr[13]*knot[1] + cr[14]*knot[2] + cr[15]*knot[3];

	return ((c3*x + c2)*x + c1)*x + c0;        
}

template <class T>
void hermite(real x, T const &k0, T const &k1, T const &k2, T const &k3, T &out )
{
	// x of 0 = k1
	// x of 1 = k2

	T g0 = (k2 - k0) * 0.5f;
	T g1 = (k3 - k1) * 0.5f;

	real x2 = x * x;
	real x3 = x2 * x;
	real c0 = 2 * x3 - 3 * x2 + 1.f;
	real c1 = -2 * x3 + 3 * x2;
	real c2 = x3 - 2*x2 + x;
	real c3 = x3 - x2;
	out = (c0 * k1) + (c1 * k2) + (c2 * g0) + (c3 * g1);
}


#endif