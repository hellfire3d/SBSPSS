#ifndef __MATRIX4X4_H__
#define __MATRIX4X4_H__

#include "mathtypes.h"
#include "vector3.h"
#include "plane.h"
//#include "user.h"

class Quaternion;

//AF - added this to suppress warning for nonstandard [unnamed union]
#pragma warning(push)
#pragma warning(disable:4201)

#if defined(__USER_Mike__)
	#define USE_INTRINSICS
#endif

#if defined(USE_INTRINSICS)
#include "xmmintrin.h"
#endif

// there should be _NO_ constructor 
struct Matrix4x4
{
	union
	{
		real m_M[4][4];
		struct
		{
			real _11, _12, _13, _14;
			real _21, _22, _23, _24;
			real _31, _32, _33, _34;
			real _41, _42, _43, _44;
		};
	};

	inline real & operator()(int ix, int iy)
	{
		return m_M[ix][iy];
	}

	inline real const & operator()(int ix, int iy) const
	{
		return m_M[ix][iy];
	}

	inline void Identity( void )
	{
		m_M[0][0] = 1.f;
		m_M[0][1] = 0.f;
		m_M[0][2] = 0.f;
		m_M[0][3] = 0.f;
		m_M[1][0] = 0.f;
		m_M[1][1] = 1.f;
		m_M[1][2] = 0.f;
		m_M[1][3] = 0.f;
		m_M[2][0] = 0.f;
		m_M[2][1] = 0.f;
		m_M[2][2] = 1.f;
		m_M[2][3] = 0.f;
		m_M[3][0] = 0.f;
		m_M[3][1] = 0.f;
		m_M[3][2] = 0.f;
		m_M[3][3] = 1.f;
	}
	
	inline void Zero( void )
	{
		m_M[0][0] = 0.f;
		m_M[0][1] = 0.f;
		m_M[0][2] = 0.f;
		m_M[0][3] = 0.f;
		m_M[1][0] = 0.f;
		m_M[1][1] = 0.f;
		m_M[1][2] = 0.f;
		m_M[1][3] = 0.f;
		m_M[2][0] = 0.f;
		m_M[2][1] = 0.f;
		m_M[2][2] = 0.f;
		m_M[2][3] = 0.f;
		m_M[3][0] = 0.f;
		m_M[3][1] = 0.f;
		m_M[3][2] = 0.f;
		m_M[3][3] = 0.f;
	}

	void ToQuaternion( Quaternion &q ) const;

	void Invert( void );
	inline void Transpose( void );

	inline void SetRotX( real a );
	inline void SetRotY( real a );
	inline void SetRotZ( real a );

	inline void SetTranslation( real x, real y, real z );
	inline void SetTranslation( Vector3 const &v );

	inline void GetTranslation(Vector3 &out) const;

	inline void SetProjection( real fov, real aspect, real nearclip, real farclip );

	inline void ApplyScale(Vector3 Scl);

	inline Matrix4x4 &operator+=( Matrix4x4 const &other );

	friend inline Matrix4x4 operator*( Matrix4x4 const &a, Matrix4x4 const &b );
	friend inline Matrix4x4 operator+( Matrix4x4 const &a, Matrix4x4 const &b );

	friend inline Vector3 operator*( Matrix4x4 const &a, Vector3 const &b );
};

#pragma warning(pop)

inline Matrix4x4 &Matrix4x4::operator +=( Matrix4x4 const &other )
{
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
		{
			m_M[i][j] += other.m_M[i][j];
		}
	}
	return *this;
}

#if 0//defined(USE_INTRINSICS)
inline void Matrix4x4::Transpose( void )
{
	_MM_TRANSPOSE4_PS(_114, _214, _314, _414);
}
#else
inline void Matrix4x4::Transpose( void )
{
	Matrix4x4 temp = *this;
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
		{
			m_M[i][j] = temp(j,i);
		}
	}
}
#endif

inline void Matrix4x4::SetRotX( real a )
{
	real ca = (real)cos( a );
	real sa = (real)sin( a );
	m_M[0][0] = 1.f;
	m_M[0][1] = 0.f;
	m_M[0][2] = 0.f;
	m_M[0][3] = 0.f;

	m_M[1][0] = 0.f;
	m_M[1][1] = ca;
	m_M[1][2] = sa;
	m_M[1][3] = 0.f;

	m_M[2][0] = 0.f;
	m_M[2][1] = -sa;
	m_M[2][2] = ca;
	m_M[2][3] = 0.f;
	
	m_M[3][0] = 0.f;
	m_M[3][1] = 0.f;
	m_M[3][2] = 0.f;
	m_M[3][3] = 1.f;
}

inline void Matrix4x4::SetRotY( real a )
{
	real ca = (real)cos( a );
	real sa = (real)sin( a );
	m_M[0][0] = ca;
	m_M[0][1] = 0.f;
	m_M[0][2] = -sa;
	m_M[0][3] = 0.f;

	m_M[1][0] = 0.f;
	m_M[1][1] = 1.f;
	m_M[1][2] = 0.f;
	m_M[1][3] = 0.f;

	m_M[2][0] = sa;
	m_M[2][1] = 0.f;
	m_M[2][2] = ca;
	m_M[2][3] = 0.f;
	
	m_M[3][0] = 0.f;
	m_M[3][1] = 0.f;
	m_M[3][2] = 0.f;
	m_M[3][3] = 1.f;
}

inline void Matrix4x4::SetRotZ( real a )
{
	real ca = (real)cos( a );
	real sa = (real)sin( a );
	m_M[0][0] = ca;
	m_M[0][1] = sa;
	m_M[0][2] = 0.f;
	m_M[0][3] = 0.f;

	m_M[1][0] = -sa;
	m_M[1][1] = ca;
	m_M[1][2] = 0.f;
	m_M[1][3] = 0.f;

	m_M[2][0] = 0.f;
	m_M[2][1] = 0.f;
	m_M[2][2] = 1.f;
	m_M[2][3] = 0.f;
	
	m_M[3][0] = 0.f;
	m_M[3][1] = 0.f;
	m_M[3][2] = 0.f;
	m_M[3][3] = 1.f;
}

inline void Matrix4x4::SetTranslation( real x, real y, real z )
{
	m_M[3][0] = x;
	m_M[3][1] = y;
	m_M[3][2] = z;
}

inline void Matrix4x4::SetTranslation( Vector3 const &v )
{
	m_M[3][0] = v.x;
	m_M[3][1] = v.y;
	m_M[3][2] = v.z;
}

inline void Matrix4x4::GetTranslation(Vector3 &out) const
{
	out.x=m_M[3][0];
	out.y=m_M[3][1];
	out.z=m_M[3][2];
}

inline void Matrix4x4::SetProjection( real fovx, real fovy, real nearclip, real farclip )
{
	real w = 1.f/((real)tan( fovx*0.5f));
	real h = 1.f/((real)tan( fovy*0.5f));
	real q = farclip / (farclip - nearclip);

	m_M[0][0] = w;
	m_M[0][1] = 0.f;
	m_M[0][2] = 0.f;
	m_M[0][3] = 0.f;
	m_M[1][0] = 0.f;
	m_M[1][1] = h;
	m_M[1][2] = 0.f;
	m_M[1][3] = 0.f;
	m_M[2][0] = 0.f;
	m_M[2][1] = 0.f;
	m_M[2][2] = q;
	m_M[2][3] = 1.f;
	m_M[3][0] = 0.f;
	m_M[3][1] = 0.f;
	m_M[3][2] = -q*nearclip;
	m_M[3][3] = 0.f;
}

inline void Matrix4x4::ApplyScale(Vector3 Scl)
{
	m_M[0][0] *= Scl.x;
	m_M[1][0] *= Scl.x;
	m_M[2][0] *= Scl.x;
	m_M[0][1] *= Scl.y;
	m_M[1][1] *= Scl.y;
	m_M[2][1] *= Scl.y;
	m_M[0][2] *= Scl.z;
	m_M[1][2] *= Scl.z;
	m_M[2][2] *= Scl.z;
}


#if defined(USE_INTRINSICS)
inline void MatrixMult( Matrix4x4 &out, Matrix4x4 const &a, Matrix4x4 const &b )
{
	__m128 t114 = _mm_loadu_ps( b.m_M[0] );
	__m128 t214 = _mm_loadu_ps( b.m_M[1] );
	__m128 t314 = _mm_loadu_ps( b.m_M[2] );
	__m128 t414 = _mm_loadu_ps( b.m_M[3] );
	__m128 a114 = _mm_loadu_ps( a.m_M[0] );
	__m128 a214 = _mm_loadu_ps( a.m_M[1] );
	__m128 a314 = _mm_loadu_ps( a.m_M[2] );
	__m128 a414 = _mm_loadu_ps( a.m_M[3] );

	_MM_TRANSPOSE4_PS(t114, t214, t314, t414);

	__m128 o11 = _mm_mul_ps( a114, t114 );
	__m128 o12 = _mm_mul_ps( a114, t214 );
	__m128 o13 = _mm_mul_ps( a114, t314 );
	__m128 o14 = _mm_mul_ps( a114, t414 );

	__m128 o21 = _mm_mul_ps( a214, t114 );
	__m128 o22 = _mm_mul_ps( a214, t214 );
	__m128 o23 = _mm_mul_ps( a214, t314 );
	__m128 o24 = _mm_mul_ps( a214, t414 );

	__m128 o31 = _mm_mul_ps( a314, t114 );
	__m128 o32 = _mm_mul_ps( a314, t214 );
	__m128 o33 = _mm_mul_ps( a314, t314 );
	__m128 o34 = _mm_mul_ps( a314, t414 );

	__m128 o41 = _mm_mul_ps( a414, t114 );
	__m128 o42 = _mm_mul_ps( a414, t214 );
	__m128 o43 = _mm_mul_ps( a414, t314 );
	__m128 o44 = _mm_mul_ps( a414, t414 );


	__m128 _11 = _mm_add_ss(o11,_mm_add_ss(_mm_shuffle_ps(o11, o11, 1),_mm_add_ss(_mm_shuffle_ps(o11, o11, 2),_mm_shuffle_ps(o11, o11, 3))));
	__m128 _12 = _mm_add_ss(o12,_mm_add_ss(_mm_shuffle_ps(o12, o12, 1),_mm_add_ss(_mm_shuffle_ps(o12, o12, 2),_mm_shuffle_ps(o12, o12, 3))));
	__m128 _13 = _mm_add_ss(o13,_mm_add_ss(_mm_shuffle_ps(o13, o13, 1),_mm_add_ss(_mm_shuffle_ps(o13, o13, 2),_mm_shuffle_ps(o13, o13, 3))));
	__m128 _14 = _mm_add_ss(o14,_mm_add_ss(_mm_shuffle_ps(o14, o14, 1),_mm_add_ss(_mm_shuffle_ps(o14, o14, 2),_mm_shuffle_ps(o14, o14, 3))));

	__m128 _21 = _mm_add_ss(o21,_mm_add_ss(_mm_shuffle_ps(o21, o21, 1),_mm_add_ss(_mm_shuffle_ps(o21, o21, 2),_mm_shuffle_ps(o21, o21, 3))));
	__m128 _22 = _mm_add_ss(o22,_mm_add_ss(_mm_shuffle_ps(o22, o22, 1),_mm_add_ss(_mm_shuffle_ps(o22, o22, 2),_mm_shuffle_ps(o22, o22, 3))));
	__m128 _23 = _mm_add_ss(o23,_mm_add_ss(_mm_shuffle_ps(o23, o23, 1),_mm_add_ss(_mm_shuffle_ps(o23, o23, 2),_mm_shuffle_ps(o23, o23, 3))));
	__m128 _24 = _mm_add_ss(o24,_mm_add_ss(_mm_shuffle_ps(o24, o24, 1),_mm_add_ss(_mm_shuffle_ps(o24, o24, 2),_mm_shuffle_ps(o24, o24, 3))));

	__m128 _31 = _mm_add_ss(o31,_mm_add_ss(_mm_shuffle_ps(o31, o31, 1),_mm_add_ss(_mm_shuffle_ps(o31, o31, 2),_mm_shuffle_ps(o31, o31, 3))));
	__m128 _32 = _mm_add_ss(o32,_mm_add_ss(_mm_shuffle_ps(o32, o32, 1),_mm_add_ss(_mm_shuffle_ps(o32, o32, 2),_mm_shuffle_ps(o32, o32, 3))));
	__m128 _33 = _mm_add_ss(o33,_mm_add_ss(_mm_shuffle_ps(o33, o33, 1),_mm_add_ss(_mm_shuffle_ps(o33, o33, 2),_mm_shuffle_ps(o33, o33, 3))));
	__m128 _34 = _mm_add_ss(o34,_mm_add_ss(_mm_shuffle_ps(o34, o34, 1),_mm_add_ss(_mm_shuffle_ps(o34, o34, 2),_mm_shuffle_ps(o34, o34, 3))));

	__m128 _41 = _mm_add_ss(o41,_mm_add_ss(_mm_shuffle_ps(o41, o41, 1),_mm_add_ss(_mm_shuffle_ps(o41, o41, 2),_mm_shuffle_ps(o41, o41, 3))));
	__m128 _42 = _mm_add_ss(o42,_mm_add_ss(_mm_shuffle_ps(o42, o42, 1),_mm_add_ss(_mm_shuffle_ps(o42, o42, 2),_mm_shuffle_ps(o42, o42, 3))));
	__m128 _43 = _mm_add_ss(o43,_mm_add_ss(_mm_shuffle_ps(o43, o43, 1),_mm_add_ss(_mm_shuffle_ps(o43, o43, 2),_mm_shuffle_ps(o43, o43, 3))));
	__m128 _44 = _mm_add_ss(o44,_mm_add_ss(_mm_shuffle_ps(o44, o44, 1),_mm_add_ss(_mm_shuffle_ps(o44, o44, 2),_mm_shuffle_ps(o44, o44, 3))));

	_mm_store_ss(&out._11, _11);
	_mm_store_ss(&out._12, _12);
	_mm_store_ss(&out._13, _13);
	_mm_store_ss(&out._14, _14);

	_mm_store_ss(&out._21, _21);
	_mm_store_ss(&out._22, _22);
	_mm_store_ss(&out._23, _23);
	_mm_store_ss(&out._24, _24);

	_mm_store_ss(&out._31, _31);
	_mm_store_ss(&out._32, _32);
	_mm_store_ss(&out._33, _33);
	_mm_store_ss(&out._34, _34);

	_mm_store_ss(&out._41, _41);
	_mm_store_ss(&out._42, _42);
	_mm_store_ss(&out._43, _43);
	_mm_store_ss(&out._44, _44);

/*	Matrix4x4 testout;
	testout(0,0) = a(0,0) * b(0,0) + a(0,1) * b(1,0) + a(0,2) * b(2,0) + a(0,3) * b(3,0);
	testout(0,1) = a(0,0) * b(0,1) + a(0,1) * b(1,1) + a(0,2) * b(2,1) + a(0,3) * b(3,1);
	testout(0,2) = a(0,0) * b(0,2) + a(0,1) * b(1,2) + a(0,2) * b(2,2) + a(0,3) * b(3,2);
	testout(0,3) = a(0,0) * b(0,3) + a(0,1) * b(1,3) + a(0,2) * b(2,3) + a(0,3) * b(3,3);

	testout(1,0) = a(1,0) * b(0,0) + a(1,1) * b(1,0) + a(1,2) * b(2,0) + a(1,3) * b(3,0);
	testout(1,1) = a(1,0) * b(0,1) + a(1,1) * b(1,1) + a(1,2) * b(2,1) + a(1,3) * b(3,1);
	testout(1,2) = a(1,0) * b(0,2) + a(1,1) * b(1,2) + a(1,2) * b(2,2) + a(1,3) * b(3,2);
	testout(1,3) = a(1,0) * b(0,3) + a(1,1) * b(1,3) + a(1,2) * b(2,3) + a(1,3) * b(3,3);

	testout(2,0) = a(2,0) * b(0,0) + a(2,1) * b(1,0) + a(2,2) * b(2,0) + a(2,3) * b(3,0);
	testout(2,1) = a(2,0) * b(0,1) + a(2,1) * b(1,1) + a(2,2) * b(2,1) + a(2,3) * b(3,1);
	testout(2,2) = a(2,0) * b(0,2) + a(2,1) * b(1,2) + a(2,2) * b(2,2) + a(2,3) * b(3,2);
	testout(2,3) = a(2,0) * b(0,3) + a(2,1) * b(1,3) + a(2,2) * b(2,3) + a(2,3) * b(3,3);

	testout(3,0) = a(3,0) * b(0,0) + a(3,1) * b(1,0) + a(3,2) * b(2,0) + a(3,3) * b(3,0);
	testout(3,1) = a(3,0) * b(0,1) + a(3,1) * b(1,1) + a(3,2) * b(2,1) + a(3,3) * b(3,1);
	testout(3,2) = a(3,0) * b(0,2) + a(3,1) * b(1,2) + a(3,2) * b(2,2) + a(3,3) * b(3,2);
	testout(3,3) = a(3,0) * b(0,3) + a(3,1) * b(1,3) + a(3,2) * b(2,3) + a(3,3) * b(3,3);*/
}
#else
inline void MatrixMult( Matrix4x4 &out, Matrix4x4 const &a, Matrix4x4 const &b )
{
	out(0,0) = a(0,0) * b(0,0) + a(0,1) * b(1,0) + a(0,2) * b(2,0) + a(0,3) * b(3,0);
	out(0,1) = a(0,0) * b(0,1) + a(0,1) * b(1,1) + a(0,2) * b(2,1) + a(0,3) * b(3,1);
	out(0,2) = a(0,0) * b(0,2) + a(0,1) * b(1,2) + a(0,2) * b(2,2) + a(0,3) * b(3,2);
	out(0,3) = a(0,0) * b(0,3) + a(0,1) * b(1,3) + a(0,2) * b(2,3) + a(0,3) * b(3,3);

	out(1,0) = a(1,0) * b(0,0) + a(1,1) * b(1,0) + a(1,2) * b(2,0) + a(1,3) * b(3,0);
	out(1,1) = a(1,0) * b(0,1) + a(1,1) * b(1,1) + a(1,2) * b(2,1) + a(1,3) * b(3,1);
	out(1,2) = a(1,0) * b(0,2) + a(1,1) * b(1,2) + a(1,2) * b(2,2) + a(1,3) * b(3,2);
	out(1,3) = a(1,0) * b(0,3) + a(1,1) * b(1,3) + a(1,2) * b(2,3) + a(1,3) * b(3,3);

	out(2,0) = a(2,0) * b(0,0) + a(2,1) * b(1,0) + a(2,2) * b(2,0) + a(2,3) * b(3,0);
	out(2,1) = a(2,0) * b(0,1) + a(2,1) * b(1,1) + a(2,2) * b(2,1) + a(2,3) * b(3,1);
	out(2,2) = a(2,0) * b(0,2) + a(2,1) * b(1,2) + a(2,2) * b(2,2) + a(2,3) * b(3,2);
	out(2,3) = a(2,0) * b(0,3) + a(2,1) * b(1,3) + a(2,2) * b(2,3) + a(2,3) * b(3,3);

	out(3,0) = a(3,0) * b(0,0) + a(3,1) * b(1,0) + a(3,2) * b(2,0) + a(3,3) * b(3,0);
	out(3,1) = a(3,0) * b(0,1) + a(3,1) * b(1,1) + a(3,2) * b(2,1) + a(3,3) * b(3,1);
	out(3,2) = a(3,0) * b(0,2) + a(3,1) * b(1,2) + a(3,2) * b(2,2) + a(3,3) * b(3,2);
	out(3,3) = a(3,0) * b(0,3) + a(3,1) * b(1,3) + a(3,2) * b(2,3) + a(3,3) * b(3,3);
}
#endif

inline void MatrixAdd( Matrix4x4 &out, Matrix4x4 const &a, Matrix4x4 const &b )
{
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
		{
			out(i,j) = a(i,j) + b(i,j);
		}
	}
}

inline Matrix4x4 operator*( Matrix4x4 const &a, Matrix4x4 const &b )
{
	Matrix4x4 temp;
	MatrixMult( temp, a, b );
	return temp;
}

inline Matrix4x4 operator+( Matrix4x4 const &a, Matrix4x4 const &b )
{
	Matrix4x4 temp;
	MatrixAdd( temp, a, b );
	return temp;
}

#if 0//defined(USE_INTRINSICS)
inline void MatrixMult( Vector3 &out, Matrix4x4 const &a, Vector3 const &b )
{
	__m128 a114 = _mm_loadu_ps( a.m_M[0] );
	__m128 a214 = _mm_loadu_ps( a.m_M[1] );
	__m128 a314 = _mm_loadu_ps( a.m_M[2] );
	__m128 a414 = _mm_loadu_ps( a.m_M[3] );
	__m128 b114 = _mm_loadu_ps( b.m_Vec );

	_MM_TRANSPOSE4_PS(a114, a214, a314, a414);

	__m128 o11 = _mm_mul_ps( a114, b114 );
	__m128 o12 = _mm_mul_ps( a214, b114 );
	__m128 o13 = _mm_mul_ps( a314, b114 );

	__m128 _11 = _mm_add_ss(o11,_mm_add_ss(_mm_shuffle_ps(o11, o11, 1),_mm_add_ss(_mm_shuffle_ps(o11, o11, 2),_mm_shuffle_ps(o11, o11, 3))));
	__m128 _12 = _mm_add_ss(o12,_mm_add_ss(_mm_shuffle_ps(o12, o12, 1),_mm_add_ss(_mm_shuffle_ps(o12, o12, 2),_mm_shuffle_ps(o12, o12, 3))));
	__m128 _13 = _mm_add_ss(o13,_mm_add_ss(_mm_shuffle_ps(o13, o13, 1),_mm_add_ss(_mm_shuffle_ps(o13, o13, 2),_mm_shuffle_ps(o13, o13, 3))));

}
#else
inline void MatrixMult( Vector3 &out, Matrix4x4 const &a, Vector3 const &b )
{
//	out.x = a(0,0) * b.x + a(1,0) * b.y + a(2,0) * b.z + a(3,0);
//	out.y = a(0,1) * b.x + a(1,1) * b.y + a(2,1) * b.z + a(3,1);
//	out.z = a(0,2) * b.x + a(1,2) * b.y + a(2,2) * b.z + a(3,2);
	out.x = a._11 * b.x + a._21 * b.y + a._31 * b.z + a._41;
	out.y = a._12 * b.x + a._22 * b.y + a._32 * b.z + a._42;
	out.z = a._13 * b.x + a._23 * b.y + a._33 * b.z + a._43;
}
#endif


inline Vector3 operator*( Matrix4x4 const &a, Vector3 const &b )
{
	Vector3 temp;
	MatrixMult( temp, a, b );
	return temp;
}

inline void MatrixMult( Plane &out, Matrix4x4 const &a, Plane const &b )
{
	Vector3 const &n = b.m_Normal;
	Vector3 const &o = out.m_Normal;
	float const   d = b.m_Distance;

	out.m_Normal.x	= a._11 * n.x + a._21 * n.y + a._31 * n.z;
	out.m_Normal.y	= a._12 * n.x + a._22 * n.y + a._32 * n.z;
	out.m_Normal.z	= a._13 * n.x + a._23 * n.y + a._33 * n.z;
	out.m_Distance	=-a._41 * o.x - a._42 * o.y - a._43 * o.z - d;
//	out.m_Distance = -b.m_Normal.x*M[3][0]  - P.b*M[3][1] - P.c*M[3][2] - P.d
}

inline Plane operator*( Matrix4x4 const &a, Plane const &b )
{
	Plane temp;
	MatrixMult( temp, a, b );
	return temp;
}


void MatrixInvert( Matrix4x4 &out, Matrix4x4 const &m );

#endif