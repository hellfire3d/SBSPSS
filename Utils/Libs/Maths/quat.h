#ifndef __QUAT_H__
#define __QUAT_H__

#include "mathtypes.h"
#include "math.h"

struct Matrix4x4;
class AxisAngle;

class Quaternion
{
public:
	union
	{
		real m_Vec[4];

		struct
		{
			real x, y, z, w;
		};
	};

	void ToMatrix( Matrix4x4 &m ) const;
	void ToInverseMatrix( Matrix4x4 &m ) const;
	void ToAxisAngle( AxisAngle & a) const;

	inline real SquareMagnitude( void ) const;
	inline real Magnitude( void ) const;
	inline void Normalise( void );
	inline void Inverse( void );
	inline void UnitInverse( void );
	inline void Negate( void );
	inline void Log( void );
	inline void Exp( void );

	inline void Identity( void );

	inline real &operator[](int idx);
	inline real const &operator[](int idx) const;

	inline Quaternion& operator*=( Quaternion const &b );
	inline Quaternion& operator+=( Quaternion const &b );
	inline Quaternion& operator/=( float const b );

	inline friend Quaternion operator*( Quaternion const &a, Quaternion const &b );
	inline friend real DotProduct( Quaternion const &a, Quaternion const &b );
};




real Quaternion::SquareMagnitude( void ) const
{
	real magsq = x*x + y*y + z*z + w*w;

	return magsq;
}

real Quaternion::Magnitude( void ) const
{
	real magsq = x*x + y*y + z*z + w*w;

	if (magsq == 0.0f)
	{
		return 0.f;
	} else
	{
		real mag = (real)(sqrt( magsq ));
		return mag;
	}
}

void Quaternion::Normalise( void )
{
	float mag = Magnitude();
	float oomag;
	if (mag != 0.0f)
	{
		oomag = 1.f / mag;
	} else
	{
		oomag = 1.f;
	}
	x*=oomag;
	y*=oomag;
	z*=oomag;
	w*=oomag;
}

void Quaternion::Inverse( void )
{
	float d=SquareMagnitude();
	float ood;
	if (d != 0.0f)
	{
		ood = 1.f / d;
	} else
	{
		ood = 1.f;
	}

	x=-x*ood;
	y=-y*ood;
	z=-z*ood;
	w*=ood;

}

void Quaternion::UnitInverse( void )
{
	x = -x;
	y = -y;
	z = -z;
}

void Quaternion::Negate( void )
{
	float mag = Magnitude();
	float oomag;
	if (mag != 0.0f)
	{
		oomag = 1.f / mag;
	} else
	{
		oomag = 1.f;
	}

	x*=-oomag;
	y*=-oomag;
	z*=-oomag;
	w*=oomag;
}

void Quaternion::Identity( void )
{
	x = y = z = 0.f;
	w = 1.f;
}

void Quaternion::Log( void )
{
	float d=(float)sqrt(x*x+y*y+z*z);
	if (w!=0.0f) 
	{
		d=atanf(d/w); 
	} else 
	{
		d=3.1479f*2.0f;
	}

	x *= d;
	y *= d;
	z *= d;
	w  = 0.f;
}


void Quaternion::Exp( void )
{
	float d=(float)sqrt(x*x+y*y+z*z);
	float d1;

	if (d>0.0f) 
	{
		d1=sinf(d)/d; 
	} else 
	{
		d1=1.0f;
	}

	x *= d1;
	y *= d1;
	z *= d1;
	w = cosf( d );
}


real &Quaternion::operator[](int idx)
{
	return m_Vec[idx];
}

real const &Quaternion::operator[](int idx) const
{
	return m_Vec[idx];
}

inline void QuaternionMult(Quaternion &out, Quaternion const &a, Quaternion const &b)
{
	out.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
	out.y = a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z;
	out.z = a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x;
	out.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
}

Quaternion& Quaternion::operator*=(Quaternion const &other)
{
	Quaternion copy = *this;

	QuaternionMult( *this, copy, other);
	return *this;
}

Quaternion& Quaternion::operator+=( Quaternion const &b )
{
	x += b.x;
	y += b.y;
	z += b.z;
	w += b.w;

	return *this;
}

Quaternion& Quaternion::operator/=( float const b )
{
	x /= b;
	y /= b;
	z /= b;
	w /= b;

	return *this;
}


Quaternion operator*(Quaternion const &a, Quaternion const &b)
{
	Quaternion out;
	QuaternionMult( out, a, b);
	return out;
}

inline real DotProduct( Quaternion const &a, Quaternion const &b )
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}


inline void Slerp( Quaternion &out, Quaternion const &a, Quaternion const &b, real t)
{
	real ca = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	real flip;
	if (ca<0.f)
	{
		ca = -ca;
		flip = -1.f;
	} else
	{
		flip = 1.f;
	}


	real c0;
	real c1;
	if ( ca > 0.9999f )
	{
		c0 = 1.f - t;
		c1 = t * flip;
	} else
	{
		real angle = (real)acos( ca );
		real sa = (real)sin( angle );
		real oosa = 1.f / sa;
		c0 = (real)(sin( (1.f - t) * angle ) * oosa);
		c1 = (real)(sin( t * angle ) * oosa) * flip;
	}

	out.x = c0 * a.x + c1 * b.x;
	out.y = c0 * a.y + c1 * b.y;
	out.z = c0 * a.z + c1 * b.z;
	out.w = c0 * a.w + c1 * b.w;
}

void Squad( Quaternion &out, Quaternion const &a, Quaternion const &i0, Quaternion const &i1, Quaternion const &b, real t );

void InnerQuadPoint( Quaternion &out, Quaternion const &a0, Quaternion const &a1, Quaternion const &a2 );

void CubicInterp( Quaternion &out, Quaternion const &a0, Quaternion const &a1, Quaternion const &a2, Quaternion const &a3, real t );

#endif