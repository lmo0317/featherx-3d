

#ifndef SIMPLE_CODEC_H
#define SIMPLE_CODEC_H

#include <assert.h>

namespace SimpleXORCodec
{
	extern const unsigned char szEncodeKey[];

	extern const unsigned long dwCrc32Table[];

	inline unsigned char GetEncodeKeyChar( size_t index )
	{
		return szEncodeKey[ index % 256 ];
	}

	inline unsigned long GetCrc32TableIndex( size_t index )
	{
		assert( index < 256 );

		return dwCrc32Table[ index ];
	}

	inline unsigned long GetCRC( unsigned char* pBuf, int nBufSize )
	{
		unsigned long dwCrc32 = 0xFFFFFFFF;

		for( int i(0); nBufSize>i; ++i )
			dwCrc32 = (dwCrc32 >> 8) ^ GetCrc32TableIndex( (dwCrc32 & 0xFF) ^ pBuf[i] );

		return dwCrc32;
	}

	int Randomize(int n);
	void MidCopy(CHAR *input, CHAR *output, int start_pos, int stop_pos);
	int EncryptAndEncode(CHAR* szData, const UINT8 *pKey, CHAR *szResult);
	int DecryptAndDecode(CHAR* szData, const UINT8 *pKey, CHAR *szResult);
}


#endif