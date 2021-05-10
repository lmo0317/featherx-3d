#ifndef _FBDEF_H_
#define _FBDEF_H_

#define FBDEL(x)		{ if(x) delete x; x = 0; }
#define FBDELARRAY(x)	{ if(x) delete[] x; x = 0; }

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)		{ if(x) x->Release(); x=0; }
#endif

#ifndef SAFE_DELETE_VECTOR
#define SAFE_DELETE_VECTOR(x) { for(size_t __i__(0); __i__ < x.size(); ++__i__) delete(x.at(__i__)); x.clear(); }
#endif

inline unsigned int GetKey( const char* pString )
{ 
	const char* pStr = pString;
	unsigned int uKey;
	for ( uKey = 0; *pStr; pStr++ )
	{
		uKey = uKey * 32 - uKey + *pStr;
	}
	return uKey;
}

inline unsigned int GetKeyW( const wchar_t* pString )
{ 
	const wchar_t* pStr = pString;
	unsigned int uKey;
	for ( uKey = 0; *pStr; pStr++ )
	{
		uKey = uKey * 32 - uKey + *pStr;
	}
	return uKey;
}

//#define ROUND_UP(n, div) (((n)+(div)-1) / (div))
#define ROUND_OFF(n, div) (((n)+(div)/2) / (div))
#define ROUND_DOWN(n, div) ((n) / (div))

#define SAMPLE_ALLOC(x)	getSampleAllocator()->allocate(x, 0, __FILE__, __LINE__)
#define	SAMPLE_FREE(x)	if(x)	{ getSampleAllocator()->deallocate(x); x = NULL;	}
#define SAMPLE_NEW(x)	new(#x, __FILE__, __LINE__) x

extern double g_dCurTime;
inline void SetCurTime( double dCurTime ) { g_dCurTime = dCurTime; }
inline double GetCurTime() { return g_dCurTime; }

#endif