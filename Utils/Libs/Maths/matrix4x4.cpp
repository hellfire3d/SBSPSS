#include "mathtypes.h"
#include "matrix4x4.h"
#include "math.h"

#include "quat.h"


void Matrix4x4::ToQuaternion( Quaternion &q ) const
{
	real s,v;

	v=_11+_22+_33;
	if (v>0.0f)
	{
		s=(float)sqrt(v+1.0f);
		q.w=0.5f*s;
		s=0.5f/s;
		q.x=(_32-_23)*s;
		q.y=(_13-_31)*s;
		q.z=(_21-_12)*s;
	} else
	{
		if (_22>_11)
		{
			if (_33>_22)
			{
				s=(float)sqrt((_33-(_11+_22))+1.0f);
				q.z=s*0.5f;
				if (s!=0.0f) s=0.5f/s;
				q.w=(_21-_12)*s;
				q.x=(_13+_31)*s;
				q.y=(_23+_32)*s;
			} else
			{
				s=(float)sqrt((_22-(_33+_11))+1.0f);
				q.y=s*0.5f;
				if (s!=0.0f) s=0.5f/s;
				q.w=(_13-_31)*s;
				q.z=(_32+_23)*s;
				q.x=(_12+_21)*s;
			}
		} else
		if (_33>_11)
		{
			s=(float)sqrt((_33-(_11+_22))+1.0f);
			q.z=s*0.5f;
			if (s!=0.0f) s=0.5f/s;
			q.w=(_21-_12)*s;
			q.x=(_13+_31)*s;
			q.y=(_23+_32)*s;
		}  else
		{
			s=(float)sqrt((_11-(_22+_33))+1.0f);
			q.x=s*0.5f;
			if (s!=0.0f) s=0.5f/s;
			q.w=(_32-_23)*s;
			q.y=(_21+_12)*s;
			q.z=(_31+_13)*s;
		}
	}
}



void MatrixInvert( Matrix4x4 &out, Matrix4x4 const &m )
{
#define SWAP_ROWS(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }

	out.Identity();

	float wtmp[4][8];
	float m0, m1, m2, m3, s;
	float *r0, *r1, *r2, *r3;

	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

	r0[0] = m(0,0), r0[1] = m(0,1),
	r0[2] = m(0,2), r0[3] = m(0,3),
	r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,

	r1[0] = m(1,0), r1[1] = m(1,1),
	r1[2] = m(1,2), r1[3] = m(1,3),
	r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,

	r2[0] = m(2,0), r2[1] = m(2,1),
	r2[2] = m(2,2), r2[3] = m(2,3),
	r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,

	r3[0] = m(3,0), r3[1] = m(3,1),
	r3[2] = m(3,2), r3[3] = m(3,3),
	r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

	/* choose pivot - or die */
	if (fabs(r3[0])>fabs(r2[0]))
		SWAP_ROWS(r3, r2);

	if (fabs(r2[0])>fabs(r1[0]))
		SWAP_ROWS(r2, r1);

	if (fabs(r1[0])>fabs(r0[0]))
		SWAP_ROWS(r1, r0);
	
	if (0.0 == r0[0])
		return;

 /* eliminate first variable     */
	m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
	s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
	s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
	s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;

	s = r0[4];
	if (s != 0.0)
		{ r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }

	s = r0[5];
	if (s != 0.0)
		{ r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }

	s = r0[6];
	if (s != 0.0)
		{ r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
	
	s = r0[7];
	if (s != 0.0)
		{ r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

	/* choose pivot - or die */
	if (fabs(r3[1])>fabs(r2[1]))
		SWAP_ROWS(r3, r2);

	if (fabs(r2[1])>fabs(r1[1]))
		SWAP_ROWS(r2, r1);

	if (0.0 == r1[1])
		return;

	/* eliminate second variable */
	m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
	r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];

	s = r1[4];
	if (0.0 != s)
		{ r2[4] -= m2 * s; r3[4] -= m3 * s; }
	
	s = r1[5];
	if (0.0 != s)
		{ r2[5] -= m2 * s; r3[5] -= m3 * s; }
	
	s = r1[6];
	if (0.0 != s)
		{ r2[6] -= m2 * s; r3[6] -= m3 * s; }

	s = r1[7];
	if (0.0 != s)
		{ r2[7] -= m2 * s; r3[7] -= m3 * s; }

	/* choose pivot - or die */
	if (fabs(r3[2])>fabs(r2[2]))
		SWAP_ROWS(r3, r2);

	if (0.0 == r2[2])
		return;

	/* eliminate third variable */
	m3 = r3[2]/r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
	r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
	r3[7] -= m3 * r2[7];

	/* last check */
	if (0.0 == r3[3])
		return;

	s = float(1.0)/r3[3];              /* now back substitute row 3 */
	r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

	m2 = r2[3];                 /* now back substitute row 2 */
	s  = float(1.0)/r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
	r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
	r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
	r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

	m1 = r1[2];                 /* now back substitute row 1 */
	s  = float(1.0)/r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
	r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
	r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

	m0 = r0[1];                 /* now back substitute row 0 */
	s  = float(1.0)/r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
	r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

	out(0,0) = r0[4]; out(0,1) = r0[5],
	out(0,2) = r0[6]; out(0,3) = r0[7],
	out(1,0) = r1[4]; out(1,1) = r1[5],
	out(1,2) = r1[6]; out(1,3) = r1[7],
	out(2,0) = r2[4]; out(2,1) = r2[5],
	out(2,2) = r2[6]; out(2,3) = r2[7],
	out(3,0) = r3[4]; out(3,1) = r3[5],
	out(3,2) = r3[6]; out(3,3) = r3[7]; 

	return;

#undef SWAP_ROWS
}

void Matrix4x4::Invert( void )
{
	Matrix4x4 temp = *this;
	MatrixInvert( *this, temp );
}