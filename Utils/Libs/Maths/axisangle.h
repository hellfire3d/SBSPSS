#ifndef __AXISANGLE_H__
#define __AXISANGLE_H__

#include "mathtypes.h"

class Quaternion;
struct Matrix4x4;

class AxisAngle
{
public:
	real x,y,z;
	real angle;

	void ToQuaternion( Quaternion &q ) const;
	void ToMatrix( Matrix4x4 &m ) const;
};




#endif