/* $PSLibId: Run-time Library Release 4.7$
 *
 *	Macro definitions of DMPSX version 3 for Assembler(aspsx) programs
 *	inline_s.h
 *	Copyright(C) 1998  Sony Computer Entertainment Inc.
 *	All rights reserved.
 */

/*
 * GTE commands with 2 nops
 */
#define nRTPS	\
		nop; \
		nop; \
		.word	0x0000007f

#define nRTPT	\
		nop; \
		nop; \
		.word	0x000000bf
		
#define nDCPL	\
		nop; \
		nop; \
		.word	0x00000dff

#define nDPCS	\
		nop; \
		nop; \
		.word	0x00000e3f

#define nDPCT	\
		nop; \
		nop; \
		.word	0x00000e7f

#define nINTPL	\
		nop; \
		nop; \
		.word	0x00000ebf

#define nNCS	\
		nop; \
		nop; \
		.word	0x00000f7f

#define nNCT	\
		nop; \
		nop; \
		.word	0x00000fbf

#define nNCDS	\
		nop; \
		nop; \
		.word	0x00000fff

#define nNCDT	\
		nop; \
		nop; \
		.word	0x0000103f

#define nNCCS	\
		nop; \
		nop; \
		.word	0x0000107f

#define nNCCT	\
		nop; \
		nop; \
		.word	0x000010bf

#define nCDP	\
		nop; \
		nop; \
		.word	0x000010ff

#define nCC	\
		nop; \
		nop; \
		.word	0x0000113f

#define nNCLIP	\
		nop; \
		nop; \
		.word	0x0000117f

#define nAVSZ3	\
		nop; \
		nop; \
		.word	0x000011bf

#define nAVSZ4	\
		nop; \
		nop; \
		.word	0x000011ff

#define nMVMVA(sf,mx,v,cv,lm)	\
		nop; \
		nop; \
		.word	0x000013bf|sf<<25|mx<<23|v<<21|cv<<19|lm<<18
		
#define nSQR(sf)	\
		nop; \
		nop; \
		.word	0x000013ff|sf<<25
		
#define nOP(sf)	\
		nop; \
		nop; \
		.word	0x0000143f|sf<<25
		
#define nGPF(sf)	\
		nop; \
		nop; \
		.word	0x0000147f|sf<<25
		
#define nGPL(sf)	\
		nop; \
		nop; \
		.word	0x000014bf|sf<<25
		
/*
 * GTE commands without nops
 */
#define RTPS	\
		.word	0x0000007f
		
#define RTPT	\
		.word	0x000000bf
		
#define DCPL	\
		.word	0x00000dff
		
#define DPCS	\
		.word	0x00000e3f
		
#define DPCT	\
		.word	0x00000e7f
		
#define INTPL	\
		.word	0x00000ebf
		
#define NCS	\
		.word	0x00000f7f
		
#define NCT	\
		.word	0x00000fbf
		
#define NCDS	\
		.word	0x00000fff
		
#define NCDT	\
		.word	0x0000103f
		
#define NCCS	\
		.word	0x0000107f
		
#define NCCT	\
		.word	0x000010bf
		
#define CDP	\
		.word	0x000010ff
		
#define CC	\
		.word	0x0000113f
		
#define NCLIP	\
		.word	0x0000117f
		
#define AVSZ3	\
		.word	0x000011bf
		
#define AVSZ4	\
		.word	0x000011ff
		
#define MVMVA(sf,mx,v,cv,lm)	\
		.word	0x000013bf|sf<<25|mx<<23|v<<21|cv<<19|lm<<18
		

#define SQR(sf)	\
		.word	0x000013ff|sf<<25
		

#define OP(sf)	\
		.word	0x0000143f|sf<<25
		

#define GPF(sf)	\
		.word	0x0000147f|sf<<25
		

#define GPL(sf)	\
		.word	0x000014bf|sf<<25
		
