#include "axisangle.h"
#include "mathtypes.h"
#include "quat.h"
#include "matrix4x4.h"


void AxisAngle::ToQuaternion( Quaternion &q ) const
{
	real ca = (real)cos(angle*0.5f);
	real sa = (real)sin(angle*0.5f);
	q.x = sa * x;
	q.y = sa * y;
	q.z = sa * z;
	q.w = ca;
}

void AxisAngle::ToMatrix( Matrix4x4 &m ) const
{
	real sa=(real)sin(angle);
	real ca=(real)cos(angle);
	real t = 1.0f - ca;

	real sx = sa * x;
	real sy = sa * y;
	real sz = sa * z;
	real tx = t * x;
	real ty = t * y;
	real tz = t * z;

	m._11 = (tx * x) + ca;
	m._21 = (tx * y) + sz;
	m._31 = (tx * z) - sy;

	m._12 = (tx * y) - sz;
	m._22 = (ty * y) + ca;
	m._32 = (ty * z) + sx;

	m._13 = (tx * z) - sy;
	m._23 = (ty * z) + sx;
	m._33 = (tz * z) + ca;
}
