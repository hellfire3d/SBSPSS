#include "mathtypes.h"
#include "quat.h"

#include "matrix4x4.h"
#include "axisangle.h"

void Quaternion::ToMatrix( Matrix4x4 &m ) const
{
	real tx  = x * 2.f;
	real ty  = y * 2.f;
	real tz  = z * 2.f;
	real txx = (tx * x);
	real tyx = (ty * x);
	real tzx = (tz * x);
	real tyy = (ty * y);
	real tzy = (tz * y);
	real tzz = (tz * z);
	real txw = (tx * w);
	real tyw = (ty * w);
	real tzw = (tz * w);

	m._11 = 1.f - (tyy + tzz);
	m._21 = tyx - tzw;
	m._31 = tzx + tyw;
	m._12 = tyx + tzw;
	m._22 = 1.f - (txx + tzz);
	m._32 = tzy - txw;
	m._13 = tzx - tyw;
	m._23 = tzy + txw;
	m._33 = 1.f - (txx + tyy);
}

void Quaternion::ToInverseMatrix( Matrix4x4 &m ) const
{
	real tx  = -x * 2.f;
	real ty  = -y * 2.f;
	real tz  = -z * 2.f;
	real txx = (tx * -x);
	real tyx = (ty * -x);
	real tzx = (tz * -x);
	real tyy = (ty * -y);
	real tzy = (tz * -y);
	real tzz = (tz * -z);
	real txw = (tx * w);
	real tyw = (ty * w);
	real tzw = (tz * w);

	m._11 = 1.f - (tyy + tzz);
	m._21 = tyx - tzw;
	m._31 = tzx + tyw;
	m._12 = tyx + tzw;
	m._22 = 1.f - (txx + tzz);
	m._32 = tzy - txw;
	m._13 = tzx - tyw;
	m._23 = tzy + txw;
	m._33 = 1.f - (txx + tyy);
}


void Quaternion::ToAxisAngle( AxisAngle & a ) const
{
	real halfang = (real)acos(w);
	real angle   = 2.f * halfang;
	real sa = (real)sin(angle);
	real oosa = 1.f/sa;

	a.x = x * oosa;
	a.y = y * oosa;
	a.z = z * oosa;
	a.angle = angle;
}


void Squad( Quaternion &out, Quaternion const &a, Quaternion const &i0, Quaternion const &i1, Quaternion const &b, real t )
{
	Quaternion res0;
	Quaternion res1;

	Slerp( res0, a, b, t );
	Slerp( res1, i0, i1, t );
	Slerp( out, res0, res1, 2.f * t * (1.f - t));
}

void InnerQuadPoint( Quaternion &out, Quaternion const &a0, Quaternion const &a1, Quaternion const &a2 )
{

	Quaternion inva0 = a0;
	Quaternion inva1 = a1;

	inva0.UnitInverse();
	inva1.UnitInverse();

	Quaternion inva0a1 = inva0*a1;
	Quaternion inva1a2 = inva1*a2;

	inva0a1.Log();
	inva1a2.Log();

	inva0a1 += inva1a2;

	inva0a1 /= -4.f;

	inva0a1.Exp();

	out = a1 * inva0a1;
}

void CubicInterp( Quaternion &out, Quaternion const &a0, Quaternion const &a1, Quaternion const &a2, Quaternion const &a3, real t )
{
	Quaternion i0;
	Quaternion i1;

	InnerQuadPoint( i0, a0, a1, a2 );
	InnerQuadPoint( i1, a1, a2, a3 );

	Squad( out, a0, i0, i1, a3, t );
}
