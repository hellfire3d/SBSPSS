#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include "mathtypes.h"
#include "math.h"

//AF - added this to suppress warning for nonstandard [unnamed union]
#pragma warning(push)
#pragma warning(disable:4201)

// there should be _NO_ constructor 
// there is now :o)
struct Vector3
{
	Vector3(){};
	Vector3(float _x,float _y,float _z)
	{
		x=_x;
		y=_y;
		z=_z;
	}
	union
	{
		float m_Vec[3];

		struct
		{
			float x, y, z;
		};
	};

	inline bool	     operator==( Vector3 const &other );
	inline Vector3 & operator+=( Vector3 const &other );
	inline Vector3 & operator-=( Vector3 const &other );

	inline Vector3 & operator/=( float val );
	inline Vector3 & operator*=( float val );

	inline float &operator[](int idx);
	inline float const &operator[](int idx) const;


	inline float GetLength( void ) const;
	inline float GetLengthSquared( void ) const;
	inline void Normalise( void );
	inline void Zero(void); //AF - hope mike don't mind me fiddling with his bits...

	
	friend inline float     operator*( Vector3 const &a, Vector3 const &b ); // dot product
	friend inline Vector3 operator^( Vector3 const &a, Vector3 const &b ); // cross product
	friend inline Vector3 operator+( Vector3 const &a, Vector3 const &b );
	friend inline Vector3 operator-( Vector3 const &a, Vector3 const &b );
	friend inline Vector3 operator*( Vector3 const &a, float val );
	friend inline Vector3 operator/( Vector3 const &a, float val );
	friend inline Vector3 operator*( float val, Vector3 const &a );
	friend inline Vector3 operator/( float val, Vector3 const &a );

};

#pragma warning(pop)
//AF - warning no longer suppressed

inline bool	Vector3::operator==( Vector3 const &other )
{
	return (x==other.x && y==other.y && z==other.z);
}

inline Vector3 & Vector3::operator+=( Vector3 const &other )
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

inline Vector3 & Vector3::operator-=( Vector3 const &other )
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

inline Vector3 & Vector3::operator/=( float val )
{
	float temp = 1.f / val;
	x *= temp;
	y *= temp;
	z *= temp;
	return *this;
}

inline Vector3 & Vector3::operator*=( float val )
{
	x *= val;
	y *= val;
	z *= val;
	return *this;
}

inline float Vector3::GetLength( void ) const
{
	float sqmag = x * x + y * y + z * z;
	float mag = (float)sqrt( sqmag );
	return mag;
}

inline float Vector3::GetLengthSquared( void ) const
{
	float sqmag = x * x + y * y + z * z;
	return sqmag;
}

inline void Vector3::Normalise( void )
{
	float mag = GetLength();
	float oneomag = 1.f / mag;
	x *= oneomag;
	y *= oneomag;
	z *= oneomag;
}

inline void Vector3::Zero(void)
{
	x=0;
	y=0;
	z=0;
}

inline float &Vector3::operator[](int idx)
{
	return m_Vec[idx];
}

inline float const &Vector3::operator[](int idx) const
{
	return m_Vec[idx];
}



inline float operator*( Vector3 const &a, Vector3 const &b ) // dot product, assumes 
{
	float tot = a.x * b.x + a.y * b.y + a.z * b.z;
	return tot;
}


inline Vector3 operator^( Vector3 const &a, Vector3 const &b ) // cross product
{
	Vector3 temp;
	temp.x = a.y * b.z - a.z * b.y;
	temp.y = a.z * b.x - a.x * b.z;
	temp.z = a.x * b.y - a.y * b.x;
	return temp;
}

inline Vector3 operator+( Vector3 const &a, Vector3 const &b )
{
	Vector3 temp;
	temp.x = a.x + b.x;
	temp.y = a.y + b.y;
	temp.z = a.z + b.z;
	return temp;
}

inline Vector3 operator-( Vector3 const &a, Vector3 const &b )
{
	Vector3 temp;
	temp.x = a.x - b.x;
	temp.y = a.y - b.y;
	temp.z = a.z - b.z;
	return temp;
}

inline Vector3 operator*( Vector3 const &a, float val )
{
	Vector3 temp;
	temp.x = a.x * val;
	temp.y = a.y * val;
	temp.z = a.z * val;
	return temp;
}

inline Vector3 operator/( Vector3 const &a, float val )
{
	Vector3 temp;
	float oneoval = 1.f / val;
	temp.x = a.x * oneoval;
	temp.y = a.y * oneoval;
	temp.z = a.z * oneoval;
	return temp;
}

inline Vector3 operator*( float val, Vector3 const &a )
{
	Vector3 temp;
	temp.x = a.x * val;
	temp.y = a.y * val;
	temp.z = a.z * val;
	return temp;
}

inline Vector3 operator/( float val, Vector3 const &a )
{
	Vector3 temp;
	float oneoval = 1.f / val;
	temp.x = a.x * oneoval;
	temp.y = a.y * oneoval;
	temp.z = a.z * oneoval;
	return temp;
}

#endif