#ifndef __PLANE_H__
#define __PLANE_H__

#include "mathtypes.h"
#include "vector3.h"


struct Plane
{
	Vector3 m_Normal;
	real m_Distance;

	inline real GetDistance( Vector3 const &pnt ) const;
	inline void Normalise( void );
	
	inline void SetPlane( Vector3 const &p0, Vector3 const &p1 );
	inline void SetPlane( Vector3 const &p0, Vector3 const &p1, Vector3 const &p2 );
};

inline real Plane::GetDistance( Vector3 const &pnt ) const
{
	real v = (m_Normal * pnt) + m_Distance;
	return v;
}

inline void Plane::Normalise( void )
{
	real mag = m_Normal.GetLength();
	real oneomag = 1.f / mag;
	
	m_Normal.x *= oneomag;
	m_Normal.y *= oneomag;
	m_Normal.z *= oneomag;
	m_Distance *= oneomag;
}

inline void Plane::SetPlane( Vector3 const &v0, Vector3 const &v1 )
{
	m_Normal = v0 ^ v1;
	m_Normal.Normalise();
	m_Distance = 0.f;
}

inline void Plane::SetPlane( Vector3 const &p0, Vector3 const &p1, Vector3 const &p2 )
{
	Vector3 v0 = (p1 - p0);
	Vector3 v1 = (p2 - p0);
	m_Normal = v0 ^ v1;
	m_Normal.Normalise();
	m_Distance = -(p0 * m_Normal);
}

#endif