#define	PROFMASK	unsigned int

typedef struct _profentry
{
	void* address;
	unsigned int flags;
} profentry;

typedef struct _profhdr
{
	void*			start;
	void*			end;
	profentry*	buffptr;
	profentry*	buffend;
	unsigned int			flags;
	unsigned int			mask;
	profentry*	ptr;
} profhdr;


/*************************************************/
/*             Profiler functions                */
/*************************************************/

extern void INITprofile(void* ptr, unsigned int length);
extern void SETprofrange(PROFMASK mask, void* startpc, void* endpc);
extern PROFMASK SETprofileflags(PROFMASK flags);
extern PROFMASK CLRprofileflags(PROFMASK flags);
extern void SNprofile();

