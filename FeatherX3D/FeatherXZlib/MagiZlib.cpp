#include "stdafx.h"

#include "MagiZlib.h"

// zlib �������  
#include "zlib.h"

#include <windows.h>


namespace
{
	const short c_nZipHeaderSize = 12;
};

//����
int Magi_compress(unsigned char *dest, unsigned long *destLen, const unsigned char *source, unsigned long sourceLen)
{
	return compress(dest, destLen, source, sourceLen);
}

//����
int Magi_uncompress(unsigned char *dest, unsigned long *destLen, const unsigned char *source, unsigned long sourceLen)
{
	return uncompress(dest, destLen, source, sourceLen); 
}

//���� ũ�� ���
int Magi_compressBufSize( unsigned long ori_size )
{
	unsigned long dwTemp = (unsigned long)(ori_size*0.1f);		//Source + 0.1% ���� ���� �ʿ�
	if( dwTemp < c_nZipHeaderSize ) dwTemp = c_nZipHeaderSize;	//��� ���� ������, ��� ��ŭ �߰�

	return dwTemp;
}