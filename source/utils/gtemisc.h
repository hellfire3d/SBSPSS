/* gtemisc.h															ZZKJ
	Contains miscellaneous GTE defines.
	Note that this also contains all sorts of C compiler assembler macros for
	the CPU as well as for CPU access to the GTE.
*/


#ifndef	__GTEMISC_H__
#define	__GTEMISC_H__

/* Convert an integer to a GTE fixed point value, and vice versa.
	libgte.h contains the definition of ONE which is 4096=1<<12
*/
#define	GTE_IntToFixed(i)	((i)<<12)
#define	GTE_FixedToInt(f)	((f)>>12)

/* The value for a full circle (360 degrees) for the GTE libraries */
#define	FULLCIRCLE	ONE


/******************************************************************************/


/* All the gte_ld* set macros take a long word VALUE as their parameter */
/* All the gte_st* read macros take a POINTER TO a long word as their parameter */
/* All the gte_get* read macros return a long word VALUE */
/* Note that the compiler seems to know to insert a NOP delay after reading a GTE
	register into a CPU register before using it.
*/

/* These have been made out of the ones in the Sony library file INLINE_C.H
	with some new creation for the gte_get* versions					ZZKJ
*/

/* Control registers */

/* Set the Light Source Direction Vector ("L" matrix) first line X (and Y) L11,L12 values */
#define gte_ldL12L11( r0 ) __asm__ volatile (	\
	"ctc2	%0, $8"							\
	:										\
	: "r"( r0 ) )

/* Set the Light Source Direction Vector ("L" matrix) first line Z (and second line X) L13,L21 values */
#define gte_ldL21L13( r0 ) __asm__ volatile (	\
	"ctc2	%0, $9"							\
	:										\
	: "r"( r0 ) )

/* Set the DQA register in the GTE */
#define gte_lddqa( r0 ) __asm__ volatile (	\
	"ctc2	%0, $27"						\
	:										\
	: "r"( r0 ) )

/* Read the DQA register in the GTE */
#define gte_stdqa( r0 ) __asm__ volatile (	\
	"cfc2	$12, $27;"						\
	"nop;"									\
	"sw	$12, 0( %0 )"						\
	:										\
	: "r"( r0 )								\
	: "$12", "memory" )

/* Get the DQA register from the GTE */
#define gte_getdqa( )			\
	({ long r0;					\
	__asm__ volatile (			\
	"cfc2	%0, $27;"			\
	: "=r"( r0 )				\
	:							\
	);							\
	r0; })

/* Set the DQB register in the GTE */
#define gte_lddqb( r0 ) __asm__ volatile (	\
	"ctc2	%0, $28"						\
	:										\
	: "r"( r0 ) )

/* Read the DQB register in the GTE */
#define gte_stdqb( r0 ) __asm__ volatile (	\
	"cfc2	$12, $28;"						\
	"nop;"									\
	"sw	$12, 0( %0 )"						\
	:										\
	: "r"( r0 )								\
	: "$12", "memory" )

/* Get the DQB register from the GTE */
#define gte_getdqb( )			\
	({ long r0;					\
	__asm__ volatile (			\
	"cfc2	%0, $28;"			\
	: "=r"( r0 )				\
	:							\
	);							\
	r0; })

/* Set the ZSF3 (ZAverage3 scaling factor) register in the GTE */
#define gte_ldzsf3( r0 ) __asm__ volatile (	\
	"ctc2	%0, $29"						\
	:										\
	: "r"( r0 ) )

/* Read the ZSF3 (ZAverage3 scaling factor) register in the GTE */
#define gte_stzsf3( r0 ) __asm__ volatile (	\
	"cfc2	$12, $29;"						\
	"nop;"									\
	"sw	$12, 0( %0 )"						\
	:										\
	: "r"( r0 )								\
	: "$12", "memory" )

/* Set the ZSF4 (ZAverage4 scaling factor) register in the GTE */
#define gte_ldzsf4( r0 ) __asm__ volatile (	\
	"ctc2	%0, $30"						\
	:										\
	: "r"( r0 ) )

/* Read the ZSF4 (ZAverage4 scaling factor) register in the GTE */
#define gte_stzsf4( r0 ) __asm__ volatile (	\
	"cfc2	$12, $30;"						\
	"nop;"									\
	"sw	$12, 0( %0 )"						\
	:										\
	: "r"( r0 )								\
	: "$12", "memory" )

/* Get the FLAGS register from the GTE */
#define gte_getflg( )			\
	({ long r0;					\
	__asm__ volatile (			\
	"cfc2	%0, $31;"			\
	: "=r"( r0 )				\
	:							\
	);							\
	r0; })

/******************************************************************************/

/* Data registers */

/* Get the OTZ result register from the GTE */
#define gte_getotz( )			\
	({ long r0;					\
	__asm__ volatile (			\
	"mfc2	%0, $7;"			\
	: "=r"( r0 )				\
	:							\
	);							\
	r0; })

/* Get the SZx(0) Z result register from the GTE */
#define gte_getszx( )			\
	({ long r0;					\
	__asm__ volatile (			\
	"mfc2	%0, $16;"			\
	: "=r"( r0 )				\
	:							\
	);							\
	r0; })

/* Get the SZ0 Z result register from the GTE */
#define gte_getsz0( )			\
	({ long r0;					\
	__asm__ volatile (			\
	"mfc2	%0, $17;"			\
	: "=r"( r0 )				\
	:							\
	);							\
	r0; })

/* Set the SZ0 Z register in the GTE */
#define gte_ldsz0( r0 ) __asm__ volatile (	\
	"mtc2	%0, $17"							\
	:										\
	: "r"( r0 ) )

/* Get the SZ1 Z result register from the GTE */
#define gte_getsz1( )			\
	({ long r0;					\
	__asm__ volatile (			\
	"mfc2	%0, $18;"			\
	: "=r"( r0 )				\
	:							\
	);							\
	r0; })

/* Set the SZ1 Z register in the GTE */
#define gte_ldsz1( r0 ) __asm__ volatile (	\
	"mtc2	%0, $18"							\
	:										\
	: "r"( r0 ) )

/* Get the SZ2 Z result register from the GTE */
#define gte_getsz2( )			\
	({ long r0;					\
	__asm__ volatile (			\
	"mfc2	%0, $19;"			\
	: "=r"( r0 )				\
	:							\
	);							\
	r0; })

/* Get the SZ Z result register from the GTE */
#define	gte_getsz	gte_getsz2

/* Set the SZ2 Z register in the GTE */
#define gte_ldsz2( r0 ) __asm__ volatile (	\
	"mtc2	%0, $19"							\
	:										\
	: "r"( r0 ) )

/******************************************************************************/

/* Set the IR0 register in the GTE */
#define	gte_ldir0( r0 )	gte_lddp( r0 )
/* Read the IR0 register in the GTE */
#define	gte_stir0( r0 )	gte_stdp( r0 )

/* Get the IR0 register from the GTE */
#define gte_getir0( )			\
	({ long r0;					\
	__asm__ volatile (			\
	"mfc2	%0, $8;"			\
	: "=r"( r0 )				\
	:							\
	);							\
	r0; })
#define	gte_getdp( )	gte_getir0( )

/* Set the IR1 register in the GTE */
#define gte_ldir1( r0 ) __asm__ volatile (	\
	"mtc2	%0, $9"							\
	:										\
	: "r"( r0 ) )

/* Read the IR1 register in the GTE */
#define gte_stir1( r0 ) __asm__ volatile (	\
	"swc2	$9, 0( %0 );"					\
	:										\
	: "r"( r0 )								\
	: "memory" )

/* Get the IR1 register from the GTE */
#define gte_getir1( )			\
	({ long r0;					\
	__asm__ volatile (			\
	"mfc2	%0, $9;"			\
	: "=r"( r0 )				\
	:							\
	);							\
	r0; })

/* Set the IR2 register in the GTE */
#define gte_ldir2( r0 ) __asm__ volatile (	\
	"mtc2	%0, $10"						\
	:										\
	: "r"( r0 ) )

/* Read the IR2 register in the GTE */
#define gte_stir2( r0 ) __asm__ volatile (	\
	"swc2	$10, 0( %0 );"					\
	:										\
	: "r"( r0 )								\
	: "memory" )

/* Get the IR2 register from the GTE */
#define gte_getir2( )			\
	({ long r0;					\
	__asm__ volatile (			\
	"mfc2	%0, $10;"			\
	: "=r"( r0 )				\
	:							\
	);							\
	r0; })

/* Set the IR3 register in the GTE */
#define gte_ldir3( r0 ) __asm__ volatile (	\
	"mtc2	%0, $11"						\
	:										\
	: "r"( r0 ) )

/* Read the IR3 register in the GTE */
#define gte_stir3( r0 ) __asm__ volatile (	\
	"swc2	$11, 0( %0 );"					\
	:										\
	: "r"( r0 )								\
	: "memory" )


/* Get the IR3 register from the GTE */
#define gte_getir3( )			\
	({ long r0;					\
	__asm__ volatile (			\
	"mfc2	%0, $11;"			\
	: "=r"( r0 )				\
	:							\
	);							\
	r0; })

/******************************************************************************/

/* Read the MAC0 register in the GTE */
#define gte_stmac0( r0 )	gte_stopz( r0 )

/* Get the MAC0 register from the GTE */
#define gte_getmac0( )			\
	({ long r0;					\
	__asm__ volatile (			\
	"mfc2	%0, $24;"			\
	: "=r"( r0 )				\
	:							\
	);							\
	r0; })
#define	gte_getopz( )	gte_getmac0( )

/* Read the MAC1 register in the GTE */
#define gte_stmac1( r0 ) gte_stlvnl0( r0 )

/* Get the MAC1 register from the GTE */
#define gte_getmac1( )			\
	({ long r0;					\
	__asm__ volatile (			\
	"mfc2	%0, $25;"			\
	: "=r"( r0 )				\
	:							\
	);							\
	r0; })
#define gte_getlvnl0( ) gte_getmac1( )

/* Read the MAC2 register in the GTE */
#define gte_stmac2( r0 ) gte_stlvnl1( r0 )

/* Get the MAC2 register from the GTE */
#define gte_getmac2( )			\
	({ long r0;					\
	__asm__ volatile (			\
	"mfc2	%0, $26;"			\
	: "=r"( r0 )				\
	:							\
	);							\
	r0; })
#define gte_getlvnl1( ) gte_getmac2( )

/* Read the MAC3 register in the GTE */
#define gte_stmac3( r0 ) gte_stlvnl2( r0 )

/* Get the MAC3 register from the GTE */
#define gte_getmac3( )			\
	({ long r0;					\
	__asm__ volatile (			\
	"mfc2	%0, $27;"			\
	: "=r"( r0 )				\
	:							\
	);							\
	r0; })
#define gte_getlvnl2( ) gte_getmac3( )

/******************************************************************************/
/******************************************************************************/

/* Load the Screen XY registers from memory */
/* This is passed the base pointer register and the actual pointer as pairs for
	each vertex
*/
/* Setup vertex 0 only */
#define gte_ldsxy0_memory(b0,a0)	INTERNAL_gte_ldsxy0_memory( b0,((ULONG)a0)-((ULONG)b0) )

#define INTERNAL_gte_ldsxy0_memory(r0,r1) __asm__ volatile (	\
	"lwc2	$12, %1( %0 )"					\
	:										\
	: "r"( r0 ), "i"( r1 ) )

/* Setup all 3 vertices */
#define gte_ldsxy3_memory(b0,a0,b1,a1,b2,a2)	INTERNAL_gte_ldsxy3_memory(	\
												b0,((ULONG)a0)-((ULONG)b0),	\
												b1,((ULONG)a1)-((ULONG)b1),	\
												b2,((ULONG)a2)-((ULONG)b2) )

#define INTERNAL_gte_ldsxy3_memory(r0,r1,r2,r3,r4,r5) __asm__ volatile (	\
	"lwc2	$12, %1( %0 );"					\
	"lwc2	$13, %3( %2 );"					\
	"lwc2	$14, %5( %4 )"					\
	:										\
	: "r"( r0 ), "i"( r1 ), "r"( r2 ), "i"( r3 ), "r"( r4 ), "i"( r5 ) )


/* Store all 3 vertices in RAM */
#define gte_stsxy3_memory(b0,a0,b1,a1,b2,a2)	INTERNAL_gte_stsxy3_memory(	\
												b0,((ULONG)a0)-((ULONG)b0),	\
												b1,((ULONG)a1)-((ULONG)b1),	\
												b2,((ULONG)a2)-((ULONG)b2) )

#define INTERNAL_gte_stsxy3_memory(r0,r1,r2,r3,r4,r5) __asm__ volatile (	\
	"swc2	$12, %1( %0 );"					\
	"swc2	$13, %3( %2 );"					\
	"swc2	$14, %5( %4 )"					\
	:										\
	: "r"( r0 ), "i"( r1 ), "r"( r2 ), "i"( r3 ), "r"( r4 ), "i"( r5 ) )


/* Load the Screen Z registers from memory */
/* This is passed the base pointer register and the actual pointer as pairs for
	each vertex
*/
/* Setup all 3 values for Z Average 3 */
#define gte_ldsz3_memory(b0,a0,b1,a1,b2,a2)	INTERNAL_gte_ldsz3_memory(	\
												b0,((ULONG)a0)-((ULONG)b0),	\
												b1,((ULONG)a1)-((ULONG)b1),	\
												b2,((ULONG)a2)-((ULONG)b2) )

#define INTERNAL_gte_ldsz3_memory(r0,r1,r2,r3,r4,r5) __asm__ volatile (	\
	"lwc2	$17, %1( %0 );"					\
	"lwc2	$18, %3( %2 );"					\
	"lwc2	$19, %5( %4 )"					\
	:										\
	: "r"( r0 ), "i"( r1 ), "r"( r2 ), "i"( r3 ), "r"( r4 ), "i"( r5 ) )

/* Setup all 4 values for Z Average 4 */
#define gte_ldsz4_memory(b0,a0,b1,a1,b2,a2,b3,a3)	INTERNAL_gte_ldsz4_memory(	\
												b0,((ULONG)a0)-((ULONG)b0),	\
												b1,((ULONG)a1)-((ULONG)b1),	\
												b2,((ULONG)a2)-((ULONG)b2),	\
												b3,((ULONG)a3)-((ULONG)b3) )

#define INTERNAL_gte_ldsz4_memory(r0,r1,r2,r3,r4,r5,r6,r7) __asm__ volatile (	\
	"lwc2	$16, %1( %0 );"					\
	"lwc2	$17, %3( %2 );"					\
	"lwc2	$18, %5( %4 );"					\
	"lwc2	$19, %7( %6 )"					\
	:										\
	: "r"( r0 ), "i"( r1 ), "r"( r2 ), "i"( r3 ), "r"( r4 ), "i"( r5 ), "r"( r6 ), "i"( r7 ) )

/******************************************************************************/
/******************************************************************************/

/* Setup the input vector registers */

/* Set the XY0 register in the GTE from a register. X in lower 16 bits, Y in upper 16 */
#define gte_ldv0XY_reg( r0 ) __asm__ volatile (		\
	"mtc2	%0, $0"									\
	:												\
	: "r"( r0 ) )

/* Set the Z0 register in the GTE from a register. */
#define gte_ldv0Z_reg( r0 ) __asm__ volatile (		\
	"mtc2	%0, $1"									\
	:												\
	: "r"( r0 ) )

/* Set the Z0 register in the GTE from memory. */
#define gte_ldv0Z( r0 ) __asm__ volatile (			\
	"lwc2	$1, 0( %0 )"							\
	:												\
	: "r"( r0 ) )

/* Set the XY1 register in the GTE from a register. X in lower 16 bits, Y in upper 16 */
#define gte_ldv1XY_reg( r0 ) __asm__ volatile (		\
	"mtc2	%0, $2"									\
	:												\
	: "r"( r0 ) )

/* Set the Z1 register in the GTE from a register. */
#define gte_ldv1Z_reg( r0 ) __asm__ volatile (		\
	"mtc2	%0, $3"									\
	:												\
	: "r"( r0 ) )

/* Set the Z1 register in the GTE from memory. */
#define gte_ldv1Z( r0 ) __asm__ volatile (			\
	"lwc2	$3, 0( %0 )"							\
	:												\
	: "r"( r0 ) )

/* Set the XY2 register in the GTE from a register. X in lower 16 bits, Y in upper 16 */
#define gte_ldv2XY_reg( r0 ) __asm__ volatile (		\
	"mtc2	%0, $4"									\
	:												\
	: "r"( r0 ) )

/* Set the Z2 register in the GTE from a register. */
#define gte_ldv2Z_reg( r0 ) __asm__ volatile (		\
	"mtc2	%0, $5"									\
	:												\
	: "r"( r0 ) )

/* Set the Z2 register in the GTE from memory. */
#define gte_ldv2Z( r0 ) __asm__ volatile (			\
	"lwc2	$5, 0( %0 )"							\
	:												\
	: "r"( r0 ) )


/******************************************************************************/
/******************************************************************************/


/* Clear the GTE translation matrix to 0,0,0 */
#define gte_ClearTransMatrix( ) __asm__ volatile (	\
	"ctc2	$0, $5;"								\
	"ctc2	$0, $6;"								\
	"ctc2	$0, $7"									\
	: )


/******************************************************************************/
/******************************************************************************/

/* Setup the 3 matrix components from a VECTOR structure. Used for Outer Product
	calculations and interpolations
*/
#define gte_SetMatrix012(r0) __asm__ volatile (	\
	"lw	$12, 0( %0 );"							\
	"lw	$13, 4( %0 );"							\
	"ctc2	$12, $0;"							\
	"lw	$12, 8( %0 );"							\
	"ctc2	$13, $2;"							\
	"ctc2	$12, $4;"							\
	:											\
	: "r"( r0 )									\
	: "$12", "$13" )

/* Setup the IR1,IR2,IR3 components from a VECTOR structure. Used for Outer
	Product calculations and interpolations
*/
#define gte_ldir123(r0) __asm__ volatile (	\
	"lwc2	$9, 0( %0 );"					\
	"lwc2	$10, 4( %0 );"					\
	"lwc2	$11, 8( %0 )"					\
	:										\
	: "r"( r0 ) )

/* Read the IR1,IR2,IR3 registers into a VECTOR structure. */
#define gte_stir123( r0 ) __asm__ volatile (	\
	"swc2	$9, 0( %0 );"						\
	"swc2	$10, 4( %0 );"						\
	"swc2	$11, 8( %0 );"						\
	:											\
	: "r"( r0 )									\
	: "memory" )

/* Read the MAC1,MAC2,MAC3 registers into a VECTOR structure. */
#define gte_stmac123( r0 ) __asm__ volatile (	\
	"swc2	$25, 0( %0 );"						\
	"swc2	$26, 4( %0 );"						\
	"swc2	$27, 8( %0 );"						\
	:											\
	: "r"( r0 )									\
	: "memory" )


/******************************************************************************/
/******************************************************************************/


/* This is the same as MulMatrix, except that is uses the matrix already
	setup in the GTE
*/
#define gte_MulMatrix0AlreadySetup(r2,r3)	\
		{									\
			gte_ldclmv(r2);					\
			gte_rtir();						\
			gte_stclmv(r3);					\
			gte_ldclmv((char*)r2+2);		\
			gte_rtir();						\
			gte_stclmv((char*)r3+2);		\
			gte_ldclmv((char*)r2+4);		\
			gte_rtir();						\
			gte_stclmv((char*)r3+4);		\
		}


/* This is the same as CompMatrix, except that is uses the matrix already
	setup in the GTE
*/
#define gte_CompMatrixAlreadySetup(r2,r3)		\
		{										\
			gte_MulMatrix0AlreadySetup(r2,r3);	\
			gte_ldlv0((char*)r2+20);			\
			gte_rt();							\
			gte_stlvl((char*)r3+20);			\
		}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


/* OK, OK!! I know this should be somewhere else, but I currently only need it here!! */

/* Multiply 2 signed 32 bit numbers for a 64 bit result and return the upper 32 bits */
/* long mul64u32(long a, long b) */
#define	mul64u32(a,b)	  			\
	({ long r0,r1=a,r2=b; 			\
	__asm__ volatile (	  			\
	"mult	%1, %2;"	  			\
	"mfhi	%0;"		  			\
	: "=r"( r0 )		  			\
	: "r"( r1 ), "r"( r2 )			\
	);					  			\
	r0; })

/* Multiply 2 unsigned 32 bit numbers for a 64 bit result and return the upper 32 bits */
/* long mulu64u32(unsigned long a, unsigned long b) */
#define	mulu64u32(a,b)	  			\
	({ long r0,r1=a,r2=b; 			\
	__asm__ volatile (	  			\
	"multu	%1, %2;"	  			\
	"mfhi	%0;"		  			\
	: "=r"( r0 )		  			\
	: "r"( r1 ), "r"( r2 )			\
	);					  			\
	r0; })

/****************/

/* Individual component instructions to start multiplies and read the results */

/* void	start_SignedMultiply(long a, long b) */
#define	start_SignedMultiply(r0,r1)	\
	__asm__ volatile (				\
	"mult	%0, %1;"				\
	:								\
	: "r"( r0 ), "r"( r1 )			\
	);								\

/* void	start_UnsignedMultiply(long a, long b) */
#define	start_UnsignedMultiply(r0,r1)	\
	__asm__ volatile (				\
	"multu	%0, %1;"				\
	:								\
	: "r"( r0 ), "r"( r1 )			\
	);								\

/* long get_MultiplyHigh(void) */
#define	get_MultiplyHigh()			\
	({ long r0;			 			\
	__asm__ volatile (	  			\
	"mfhi	%0;"		  			\
	: "=r"( r0 )		  			\
	:								\
	);					  			\
	r0; })

/* long get_MultiplyLow(void) */
#define	get_MultiplyLow()			\
	({ long r0;			 			\
	__asm__ volatile (	  			\
	"mflo	%0;"		  			\
	: "=r"( r0 )		  			\
	:								\
	);					  			\
	r0; })

/****************/

#if	0
/* Multiply 2 32 bit numbers for a 64 bit result and store the upper 32 bits */
/* void mul64u32(long *dest,long a, long b) */
#define	mul64u32pointer( r0, r1, r2 ) __asm__ volatile (	\
	"mult	%1, %2;"								\
	"mfhi	$12;"									\
	"sw		$12, 0( %0 );"							\
	:												\
	: "r"( r0 ), "r"( r1 ), "r"( r2 )				\
	: "$12", "memory" )

/* Multiply 2 32 bit numbers for a 64 bit result and store the upper 32 bits */
/* void mul64u32(long dest,long a, long b) */
#define	mul64u32value( r0, r1, r2 ) __asm__ volatile (	\
	"mult	%1, %2;"								\
	"mfhi	%0;"									\
	: "=r"( r0 )									\
	: "r"( r1 ), "r"( r2 )							\
	)
#endif

/******************************************************************************/
/******************************************************************************/

/* This is passed a value, a base pointer register and the actual pointer value
	to be read.
	These are used to forcefully override the compiler's desire to move memory
	reads later in the program until they are needed instead of leaving them
	where they are which can actually be better in terms of not having a delay
	slot while waiting for the read, or by being able to read early while the
	memory bus is clear.												ZZKJ
*/
#define read_LONG(v,b0,a0)	INTERNAL_read_LONG(v, b0,((ULONG)a0)-((ULONG)b0) )
#define read_SLONG(v,b0,a0)	INTERNAL_read_LONG(v, b0,((ULONG)a0)-((ULONG)b0) )
#define read_ULONG(v,b0,a0)	INTERNAL_read_LONG(v, b0,((ULONG)a0)-((ULONG)b0) )
#define read_SWORD(v,b0,a0)	INTERNAL_read_SWORD(v, b0,((ULONG)a0)-((ULONG)b0) )
#define read_UWORD(v,b0,a0)	INTERNAL_read_UWORD(v, b0,((ULONG)a0)-((ULONG)b0) )
#define read_SBYTE(v,b0,a0)	INTERNAL_read_SBYTE(v, b0,((ULONG)a0)-((ULONG)b0) )
#define read_UBYTE(v,b0,a0)	INTERNAL_read_UBYTE(v, b0,((ULONG)a0)-((ULONG)b0) )

#define INTERNAL_read_LONG(r0,r1,r2)	\
	__asm__ volatile (	  				\
	"lw		%0, %2( %1 )"				\
	: "=r"( r0 )						\
	: "r"( r1 ), "i"( r2 )				\
	)

#define INTERNAL_read_SWORD(r0,r1,r2) 	\
	__asm__ volatile (	  				\
	"lh		%0, %2( %1 )"				\
	: "=r"( r0 )						\
	: "r"( r1 ), "i"( r2 )				\
	)

#define INTERNAL_read_UWORD(r0,r1,r2) 	\
	__asm__ volatile (	  				\
	"lhu	%0, %2( %1 )"				\
	: "=r"( r0 )						\
	: "r"( r1 ), "i"( r2 )				\
	)

#define INTERNAL_read_SBYTE(r0,r1,r2) 	\
	__asm__ volatile (	  				\
	"lb		%0, %2( %1 )"				\
	: "=r"( r0 )						\
	: "r"( r1 ), "i"( r2 )				\
	)

#define INTERNAL_read_UBYTE(r0,r1,r2) 	\
	__asm__ volatile (	  				\
	"lbu	%0, %2( %1 )"				\
	: "=r"( r0 )						\
	: "r"( r1 ), "i"( r2 )				\
	)


/******************************************************************************/


#endif	/* __GTEMISC_H__ */
