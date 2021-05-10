#include "stdafx.h"

#include "MagiZlib.h"

// zlib 헤더파일  
#include "zlib.h"

#include <windows.h>


namespace
{
	const short c_nZipHeaderSize = 12;
};

//압축
int Magi_compress(unsigned char *dest, unsigned long *destLen, const unsigned char *source, unsigned long sourceLen)
{
	return compress(dest, destLen, source, sourceLen);
}

//해제
int Magi_uncompress(unsigned char *dest, unsigned long *destLen, const unsigned char *source, unsigned long sourceLen)
{
	return uncompress(dest, destLen, source, sourceLen); 
}

//압축 크기 얻기
int Magi_compressBufSize( unsigned long ori_size )
{
	unsigned long dwTemp = (unsigned long)(ori_size*0.1f);		//Source + 0.1% 정도 여분 필요
	if( dwTemp < c_nZipHeaderSize ) dwTemp = c_nZipHeaderSize;	//헤더 보다 작으면, 헤더 만큼 추가

	return dwTemp;
}