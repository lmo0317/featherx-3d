


#ifndef MAGI_ZLIB_H
#define MAGI_ZLIB_H


int Magi_compress(unsigned char *dest, unsigned long *destLen, const unsigned char *source, unsigned long sourceLen);
int Magi_uncompress(unsigned char *dest, unsigned long *destLen, const unsigned char *source, unsigned long sourceLen);

int Magi_compressBufSize( unsigned long ori_size );

#endif