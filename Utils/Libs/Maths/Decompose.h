/**** Decompose.h - Basic declarations ****/
#ifndef _H_Decompose
#define _H_Decompose

#include "matrix4x4.h"
#include "vector3.h"
#include "quat.h"

struct AffineParts
{
	Vector3 t;
	Quaternion q;
	Quaternion u;
	Vector3	k;
	float	f;
};

void decomp_affine(Matrix4x4 const &A, AffineParts *parts);
void invert_affine(AffineParts *parts, AffineParts *inverse);

#endif
