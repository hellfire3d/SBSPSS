#ifndef __AABOX_H__
#define __AABOX_H__

#include "vector3.h"

struct AABox
{
	Vector3 m_Min;
	Vector3 m_Max;

	bool QueryInside( Vector3 const &pnt ) const
	{
		if (pnt.x >= m_Min.x-0.001f && pnt.y >= m_Min.y-0.001f && pnt.z >= m_Min.z-0.001f &&
			pnt.x < m_Max.x+0.001f && pnt.y < m_Max.y+0.001f && pnt.z < m_Max.z+0.001f)
		{
			return true;
		} else
		{
			return false;
		}
	}

	bool QueryInside( AABox const &box ) const
	{
		return QueryInside( box.m_Min ) && QueryInside( box.m_Max );
	}
};

#endif