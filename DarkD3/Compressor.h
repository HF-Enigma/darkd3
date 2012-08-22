#ifndef _COMPRESSOR_H_
#define _COMPRESSOR_H_

#include "stdafx.h"
#include "DSMemory\\DSUtils.h"
#include "MPQReader\\zlib.h"

class CCompressor
{
public:
	CCompressor(void);
	~CCompressor(void);

	static DWORD Compress(PVOID pData, DWORD& size, DWORD blocksize = 0x8000, DWORD level = Z_BEST_COMPRESSION);
	static DWORD Decompress(PVOID pData, DWORD& size, DWORD sizeReserve, DWORD blocksize = 0x8000);
};

#endif//_COMPRESSOR_H_