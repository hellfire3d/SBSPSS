/******************/
/*** Misc Utils ***/
/******************/

#ifndef	__UTILS_HEADER__
#define	__UTILS_HEADER__

#include	"utils\mathmip.h"

#include "Dstructs.h"

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif

/*****************************************************************************/
#define	CIRCLE_TAB_MAX	64
#define	CIRCLE_TAB_MASK	63
extern const s16 CircleTable[ CIRCLE_TAB_MAX];

/*****************************************************************************/
u8		*MakePtr(void *BasePtr,int Offset);

/*****************************************************************************/
/*** Loads of inlines ********************************************************/
/*****************************************************************************/
inline	void	SetIdentTrans(MATRIX *Mtx,s32 AX,s32 AY,s32 AZ,s32 TX,s32 TY,s32 TZ)
{
	Mtx->m[0][0]=AX; 	Mtx->m[0][1]=0;		Mtx->m[0][2]=0;
	Mtx->m[1][0]=0; 	Mtx->m[1][1]=AY;	Mtx->m[1][2]=0;
	Mtx->m[2][0]=0; 	Mtx->m[2][1]=0;		Mtx->m[2][2]=AZ;
	Mtx->t[0]=TX;		Mtx->t[1]=TY;		Mtx->t[2]=TZ;
}

inline	void	SetIdentTrans(MATRIX *Mtx)												{SetIdentTrans(Mtx,	ONE,ONE,ONE,		0,0,0);}
inline	void	SetIdentTrans(MATRIX *Mtx,VECTOR *T)									{SetIdentTrans(Mtx,	ONE,ONE,ONE,		T->vx,T->vy,T->vz);}
inline	void	SetIdentTrans(MATRIX *Mtx,s32 TX,s32 TY,s32 TZ)							{SetIdentTrans(Mtx,	ONE,ONE,ONE,		TX,TY,TZ);}
inline	void	SetIdentTrans(MATRIX *Mtx,VECTOR *S,VECTOR *T)							{SetIdentTrans(Mtx,	S->vx,S->vy,S->vz,	T->vx,T->vy,T->vz);}
inline	void	SetIdentTrans(MATRIX *Mtx,VECTOR *S,s32 TX,s32 TY,s32 TZ)				{SetIdentTrans(Mtx,	S->vx,S->vy,S->vz,	TX,TY,TZ);}
inline	void	SetIdentTrans(MATRIX *Mtx,s32 SX,s32 SY, s32 SZ,VECTOR *T)				{SetIdentTrans(Mtx,	SX,SY,SZ,			T->vx,T->vy,T->vz);}
//inline	void	SetIdent(MATRIX *Mtx,s32 SX,s32 SY, s32 SZ,s32 TX,s32 TY,s32 TZ)	{SetIdentTrans(Mtx,	SX,SY,SZ,			TX,TY,TZ);}

/*****************************************************************************/
inline	void	SetIdentNoTrans(MATRIX *Mtx,s32 AX,s32 AY,s32 AZ)
{
	Mtx->m[0][0]=AX; 	Mtx->m[0][1]=0;		Mtx->m[0][2]=0;
	Mtx->m[1][0]=0; 	Mtx->m[1][1]=AY;	Mtx->m[1][2]=0;
	Mtx->m[2][0]=0; 	Mtx->m[2][1]=0;		Mtx->m[2][2]=AZ;
//	Mtx->t[0]=TX;		Mtx->t[1]=TY;		Mtx->t[2]=TZ;
}

inline	void	SetIdentNoTrans(MATRIX *Mtx)										{SetIdentNoTrans(Mtx,	ONE,ONE,ONE);}
inline	void	SetIdentNoTrans(MATRIX *Mtx,VECTOR *S)								{SetIdentNoTrans(Mtx,	S->vx,S->vy,S->vz);}
//inline	void	SetIdentNoTrans(MATRIX *Mtx,s32 SX,s32 SY, s32 SZ)				{SetIdentNoTrans(Mtx,	SX,SY,SZ);}


/*****************************************************************************/
inline	void InverseMatrix(MATRIX *m, MATRIX &im)	//assumes no scale, just transformation and rotation
{
	TransposeMatrix(m, &im);
	ApplyMatrixLV(&im, (VECTOR*)&m->t[0], (VECTOR*)&im.t[0]);
	im.t[0] = -im.t[0];
	im.t[1] = -im.t[1];
	im.t[2] = -im.t[2];
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
extern long s_randomSeed;
inline	long getRndSeed()
{
	const u32 INCREMENT = 1;
	const u32 MULTIPLIER = 0x015a4e35L;
	s_randomSeed = MULTIPLIER * s_randomSeed + INCREMENT;
	return abs(s_randomSeed);
}

/*****************************************************************************/
inline	void setRndSeed( long seed )
{
	s_randomSeed = seed;
}

/*****************************************************************************/
inline	long getRnd()
{
	return getRndSeed();
}

/*****************************************************************************/
inline	long getRndRange( long v )
{
	if (v <= 0) return 0;

	// high order bits are more "random" than low order bits
	if (v < 0x0ffff) return (getRndSeed() >> 16) % v;

	return getRndSeed() % v;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

// -- VECTOR Operator Arithmetic operation --

inline VECTOR  operator - (const VECTOR& a)
{
	VECTOR	ret;
	ret.vx = -a.vx;
	ret.vy = -a.vy;
	ret.vz = -a.vz;
	return ret;
}

inline VECTOR operator + (const VECTOR& a, const VECTOR& b)
{
	VECTOR	ret;
	ret.vx = a.vx + b.vx;
	ret.vy = a.vy + b.vy;
	ret.vz = a.vz + b.vz;
	return ret;
}

inline VECTOR operator - (const VECTOR& a, const VECTOR& b)
{
	VECTOR	ret;
	ret.vx = a.vx - b.vx;
	ret.vy = a.vy - b.vy;
	ret.vz = a.vz - b.vz;
	return ret;
}

inline VECTOR operator *= (const VECTOR& a, const int & b)
{
	VECTOR	ret;
	ret.vx = a.vx * b;
	ret.vy = a.vy * b;
	ret.vz = a.vz * b;
	return ret;
}

inline VECTOR operator >>= (const VECTOR& a, const int & b)
{
	VECTOR	ret;
	ret.vx = a.vx >> b;
	ret.vy = a.vy >> b;
	ret.vz = a.vz >> b;
	return ret;
}

inline VECTOR operator += (const VECTOR& a, const VECTOR& b)
{
	VECTOR	ret;
	ret.vx = a.vx + b.vx;
	ret.vy = a.vy + b.vy;
	ret.vz = a.vz + b.vz;
	return ret;
}

inline int operator == (const VECTOR& a, const VECTOR& b)
{
	return (a.vx == b.vx) && (a.vy == b.vy) && (a.vz == b.vz);
}

inline int operator != (const VECTOR& a, const VECTOR& b)
{
	return !(a == b); 
}


// -- VECTOR to sVtx Operator Arithmetic operation --

inline VECTOR operator - (const sShortXYZ& a, const sVtx& b)
{
	VECTOR	ret;
	ret.vx = a.vx - b.vx;
	ret.vy = a.vy - b.vy;
	ret.vz = a.vz - b.vz;
	return ret;
}

inline VECTOR operator - (const sVtx& a, const sVtx& b)
{
	VECTOR	ret;
	ret.vx = a.vx - b.vx;
	ret.vy = a.vy - b.vy;
	ret.vz = a.vz - b.vz;
	return ret;
}

inline VECTOR operator *= (const sVtx& a, const int & b)
{
	VECTOR	ret;
	ret.vx = a.vx * b;
	ret.vy = a.vy * b;
	ret.vz = a.vz * b;
	return ret;
}

inline VECTOR operator >>= (const sVtx& a, const int & b)
{
	VECTOR	ret;
	ret.vx = a.vx >> b;
	ret.vy = a.vy >> b;
	ret.vz = a.vz >> b;
	return ret;
}

// -- sShortXYZ Operator Arithmetic operation --

inline sShortXYZ  operator - (const sShortXYZ& a)
{
	sShortXYZ	ret;
	ret.vx = -a.vx;
	ret.vy = -a.vy;
	ret.vz = -a.vz;
	return ret;
}

inline sShortXYZ operator + (const sShortXYZ& a, const sShortXYZ& b)
{
	sShortXYZ	ret;
	ret.vx = a.vx + b.vx;
	ret.vy = a.vy + b.vy;
	ret.vz = a.vz + b.vz;
	return ret;
}

inline sShortXYZ operator >> (const VECTOR& a, const int& b)
{
	sShortXYZ	ret;
	ret.vx = a.vx >> b;
	ret.vy = a.vy >> b;
	ret.vz = a.vz >> b;
	return ret;
}

inline int operator * (const sShortXYZ& a, const sShortXYZ& b)	// DOT PRODUCT
{
	return (a.vx * b.vx) + (a.vy * b.vy) + (a.vz * b.vz);
}

inline int operator == (const sShortXYZ& a, const sShortXYZ& b)
{
	return (a.vx == b.vx) && (a.vy == b.vy) && (a.vz == b.vz);
}

inline int operator != (const sShortXYZ& a, const sShortXYZ& b)
{
	return !(a == b); 
}

// -- sShortXYZ to VECTOR Operator Arithmetic operation --

inline VECTOR operator + (const VECTOR& a, const sShortXYZ& b)
{
	VECTOR	ret;
	ret.vx = a.vx + b.vx;
	ret.vy = a.vy + b.vy;
	ret.vz = a.vz + b.vz;
	return ret;
}

inline VECTOR operator - (const VECTOR& a, const sShortXYZ& b)
{
	VECTOR	ret;
	ret.vx = a.vx - b.vx;
	ret.vy = a.vy - b.vy;
	ret.vz = a.vz - b.vz;
	return ret;
}

inline VECTOR operator - (const sShortXYZ& a, const sShortXYZ& b)
{
	VECTOR	ret;
	ret.vx = a.vx - b.vx;
	ret.vy = a.vy - b.vy;
	ret.vz = a.vz - b.vz;
	return ret;
}

inline int operator * (const VECTOR& a, const sShortXYZ& b)	// DOT PRODUCT
{
	return (a.vx * b.vx) + (a.vy * b.vy) + (a.vz * b.vz);
}

inline int operator * (const sShortXYZ& a, const VECTOR& b)	// DOT PRODUCT
{
	return (a.vx * b.vx) + (a.vy * b.vy) + (a.vz * b.vz);
}

inline VECTOR operator ^ (const VECTOR& a, const sShortXYZ& b)	// CROSS PRODUCT
{
	VECTOR	ret;
	ret.vx = (a.vy * b.vz) - (a.vz * b.vy);
	ret.vy = (a.vz * b.vx) - (a.vx * b.vz);
	ret.vz = (a.vx * b.vy) - (a.vy * b.vx);
	return ret;
}

inline VECTOR operator ^ (const sShortXYZ& a, const sShortXYZ& b)	// CROSS PRODUCT
{
	VECTOR	ret;
	ret.vx = (a.vy * b.vz) - (a.vz * b.vy);
	ret.vy = (a.vz * b.vx) - (a.vx * b.vz);
	ret.vz = (a.vx * b.vy) - (a.vy * b.vx);
	return ret;
}


/*****************************************************************************/
/*** Inlines *****************************************************************/
/*****************************************************************************/
inline u32 	isqrt2(u32 v)
{
register u32 root = 0;
register u32 remhi = 0;
register u32 remlo = v;
register u32 count = 15;
register u32 testdiv;

	do
		{
		remhi = (remhi << 2) | (remlo >> 30);
		remlo <<= 2;
		root <<= 1;
		testdiv = (root << 1) + 1;
		if (remhi >= testdiv)
			{
			remhi -= testdiv;
			root += 1;
			}
		} while (count -- != 0);
	return(root);
}


/*****************************************************************************/
inline u32		CalcLength(const SVECTOR *s)
{
int		Dt;
		gte_ldsv(s);
		gte_sqr0();
		CMX_StVecXYZMag(&Dt);
		return(Dt);
}

/*****************************************************************************/
inline u32		CalcLengthSquared(const SVECTOR *s)
{
u32		Dt;
		gte_ldsv(s);
		gte_sqr0();
		CMX_StVecXYZMag(&Dt);
		return(Dt);
}

/*****************************************************************************/
inline u32		CalcLengthV(const VECTOR *s)
{
u32		Dt;
		gte_ldlvl(s);
		gte_sqr0();
		CMX_StVecXYZMag(&Dt);
		return(isqrt2(Dt));
}

/*****************************************************************************/
inline u32		CalcLengthSquaredV(const VECTOR *s)
{
u32		Dt;
		gte_ldlvl(s);
		gte_sqr0();
		CMX_StVecXYZMag(&Dt);
		return(Dt);
}

/*****************************************************************************/
inline u32		CalcDist(const SVECTOR *s, const SVECTOR *e)
{
s32		Dx = s->vx - e->vx;
s32		Dy = s->vy - e->vy;
s32		Dz = s->vz - e->vz;
u32		Dt;

		CMX_ldXYZ(Dx,Dy,Dz);
		gte_sqr0();
		CMX_StVecXYZMag(&Dt);
		return(isqrt2(Dt));
}

/*****************************************************************************/
inline u32		CalcDistXZ(const SVECTOR *s, const SVECTOR *e)
{
s32		Dx = s->vx - e->vx;
s32		Dz = s->vz - e->vz;
u32		Dt;

		CMX_ldXZ(Dx,Dz);
		gte_sqr0();
		CMX_StVecXZMag(&Dt);
		return(isqrt2(Dt));
}

/*****************************************************************************/
inline u32		CalcDistV(const VECTOR *s, const VECTOR *e)
{
s32		Dx = s->vx - e->vx;
s32		Dy = s->vy - e->vy;
s32		Dz = s->vz - e->vz;
u32		Dt;

		CMX_ldXYZ(Dx,Dy,Dz);
		gte_sqr0();
		CMX_StVecXYZMag(&Dt);
		return(isqrt2(Dt));
}

/*****************************************************************************/
inline u32		CalcDistV(const VECTOR *s, const VECTOR *e, u16 shift)
{
s32		Dx = (s->vx - e->vx) >> shift;
s32		Dz = (s->vz - e->vz) >> shift;
u32		Dt;

		CMX_ldXZ(Dx,Dz);
		gte_sqr0();
		CMX_StVecXZMag(&Dt);
		return(isqrt2(Dt) << shift);
}

/*****************************************************************************/
inline u32		CalcDistXZV(const VECTOR *s, const VECTOR *e)
{
s32		Dx = s->vx - e->vx;
s32		Dz = s->vz - e->vz;
u32		Dt;
		CMX_ldXZ(Dx,Dz);
		gte_sqr0();
		CMX_StVecXZMag(&Dt);
		return(isqrt2(Dt));
}

/*****************************************************************************/
inline u32		CalcDistXZVSquared(const VECTOR *s, const VECTOR *e)
{
s32		Dx = s->vx - e->vx;
s32		Dz = s->vz - e->vz;
u32		Dt;

		CMX_ldXZ(Dx,Dz);
		gte_sqr0();
		CMX_StVecXZMag(&Dt);
		return abs(Dt);
}

/*****************************************************************************/
inline u32		CalcDistXZV(const VECTOR *s, const VECTOR *e, u16 shift)
{
s32		Dx = (s->vx - e->vx) >> shift;
s32		Dz = (s->vz - e->vz) >> shift;
u32		Dt;

		CMX_ldXZ(Dx,Dz);
		gte_sqr0();
		CMX_StVecXZMag(&Dt);
		return(isqrt2(Dt) << shift);
}

/*****************************************************************************/
inline	long	CalcDistXZVAccurate(const VECTOR *s, const VECTOR *e)
{
long	Dx = s->vx - e->vx;
long	Dz = s->vz - e->vz;
long	Dt = (Dx*Dx) + (Dz*Dz);
long	len = SquareRoot0(Dt);
		return(len);
}

/*****************************************************************************/
inline u32		CalcDist(const sShortXYZ *s, const sShortXYZ *e)
{
s32		Dx = s->vx - e->vx;
s32		Dy = s->vy - e->vy;
s32		Dz = s->vz - e->vz;
u32		Dt;

		CMX_ldXYZ(Dx,Dy,Dz);
		gte_sqr0();
		CMX_StVecXYZMag(&Dt);
		return(isqrt2(Dt));
}

/*****************************************************************************/
inline u32		CalcDistXZ(const sShortXYZ *s, const sShortXYZ *e)
{
s32		Dx = s->vx - e->vx;
s32		Dz = s->vz - e->vz;
u32		Dt;

		CMX_ldXZ(Dx,Dz);
		gte_sqr0();
		CMX_StVecXZMag(&Dt);
		return(isqrt2(Dt));
}

/*****************************************************************************/
inline u32		CalcDistXZSquared(const sShortXYZ *s, const sShortXYZ *e)
{
s32		Dx = s->vx - e->vx;
s32		Dz = s->vz - e->vz;
u32		Dt;

		CMX_ldXZ(Dx,Dz);
		gte_sqr0();
		CMX_StVecXZMag(&Dt);
		return(Dt);
}
/*****************************************************************************/
inline	s32		DotProduct(const SVECTOR *V0, const SVECTOR *V1)
{		
s32		Dpx,Dpy,Dpz;
		Dpx=V0->vx*V1->vx;
		Dpy=V0->vy*V1->vy;
		Dpz=V0->vz*V1->vz;
		return(Dpx+Dpy+Dpz);
}

/*****************************************************************************/
inline	s32 DotProductV( const VECTOR *V0, const VECTOR *V1 )
{		
s32		ax, ay, az;
s32		bx, by, bz;
s32		Dpx, Dpy, Dpz;

		ax = V0->vx;
		ay = V0->vy;
		az = V0->vz;

		bx = V1->vx;
		by = V1->vy;
		bz = V1->vz;

		Dpx = ax * bx;
		Dpy = ay * by;
		Dpz = az * bz;

		return (Dpx + Dpy + Dpz);
}
/*****************************************************************************/
inline	s32 DotProductSV( const SVECTOR *V0, const VECTOR *V1 )
{
s32		ax, ay, az;
s32		bx, by, bz;
s32		Dpx, Dpy, Dpz;

		ax = V0->vx;
		ay = V0->vy;
		az = V0->vz;

		bx = V1->vx;
		by = V1->vy;
		bz = V1->vz;

		Dpx = ax * bx;
		Dpy = ay * by;
		Dpz = az * bz;

		return (Dpx + Dpy + Dpz);
}
/*****************************************************************************/
/*
inline void	QuatSlerp(sQuat *s, sQuat *d, int t, sQuat *o)
{
s32		xx, yy, zz, ww;
s32		cs;
s32		c0, c1;
sQuat	to;

		xx = (s->vx * d->vx);
		yy = (s->vy * d->vy);
		zz = (s->vz * d->vz);
		ww = (s->vw * d->vw);

		cs  = xx + yy + zz + ww;
		if (cs<0) 
			{
			cs = -cs;
			to.vx = -d->vx;
			to.vy = -d->vy;
			to.vz = -d->vz;
			to.vw = -d->vw;
			} 
		else 
			{
			to.vx = d->vx;
			to.vy = d->vy;
			to.vz = d->vz;
			to.vw = d->vw;
			}

		cs >>= 12;
		s32 omega;
		if (cs < 3500)	
			{	// standard case (slerp)
			omega = macos(cs);
			s32 sinom = monesin(omega);
			c0 = (msin((((4096 - t) * omega) >> 12) & 4095) * sinom)>>12;
			c1 = (msin(((t          * omega) >> 12) & 4095) * sinom)>>12;
			} 
		else
			{
			c0 = 4096 - t;
			c1 = t;
			}

		gte_LoadAverageShort12(s, &to, c0, c1, o);
		o->vw = ((c0 * s->vw) + (c1 * to.vw)) >> 12;
}
*/
/*****************************************************************************/
/*
static const s32 DeltaErr = 1000;
inline void NormalizeQuaternion(sQuat *o)
{
s32 t = (u32)((s32)o->vx*o->vx+(s32)o->vy*o->vy+(s32)o->vz*o->vz+(s32)o->vw*o->vw);
	s32 diff = abs((1<<24) - t);
	if (diff>DeltaErr)
		{
		s32 isqrs = isqrt2(t);
		if (isqrs)
			{
			o->vx = (s16)(((s32)o->vx << 12) / (s32)isqrs);
			o->vy = (s16)(((s32)o->vy << 12) / (s32)isqrs);
			o->vz = (s16)(((s32)o->vz << 12) / (s32)isqrs);
			o->vw = (s16)(((s32)o->vw << 12) / (s32)isqrs);
			}
		}
}
*/
#endif