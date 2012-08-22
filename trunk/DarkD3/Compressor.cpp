#include "Compressor.h"

CCompressor::CCompressor(void)
{
}

CCompressor::~CCompressor(void)
{
}

DWORD CCompressor::Compress( PVOID pData, DWORD& size, DWORD blocksize, DWORD level )
{
	int BlockSizeCompress = blocksize;
	int cprLevel = level;
	long lBufferSizeCpr = 0;
	ds_utils::ds_memory::CDSWinDataBlock CprPtr;
	long lSizeCpr = 0;

	lBufferSizeCpr = size + (size/0x10) + 0x200;

	CprPtr.Allocate(lBufferSizeCpr + BlockSizeCompress);

	z_stream zcpr;
	int ret = Z_OK;
	long lOrigToDo = size;
	long lOrigDone = 0;
	int step = 0;

	memset(&zcpr, 0, sizeof(z_stream));
	deflateInit(&zcpr, cprLevel);

	zcpr.next_in = (BYTE*)pData;
	zcpr.next_out = CprPtr;

	do
	{
		long all_read_before = zcpr.total_in;
		zcpr.avail_in = min(lOrigToDo,BlockSizeCompress);
		zcpr.avail_out = BlockSizeCompress;
		ret = deflate(&zcpr,(zcpr.avail_in == (uInt)lOrigToDo) ? Z_FINISH : Z_SYNC_FLUSH);
		lOrigDone += (zcpr.total_in-all_read_before);
		lOrigToDo -= (zcpr.total_in-all_read_before);
		step++;
	} while (ret==Z_OK);

	lSizeCpr = zcpr.total_out;
	deflateEnd(&zcpr);

	RtlCopyMemory(pData, CprPtr, lSizeCpr);

	size = lSizeCpr;

	return ERROR_SUCCESS;        
}

DWORD CCompressor::Decompress( PVOID pData, DWORD& size, DWORD sizeReserve, DWORD blocksize /*= 0x8000*/ )
{
	int BlockSizeUncompress = blocksize;
	long lBufferSizeUncpr = 0;
	ds_utils::ds_memory::CDSWinDataBlock UncprPtr;
	long lSizeUncpr = 0;

	lBufferSizeUncpr = sizeReserve + (sizeReserve/0x10) + 0x200;

	UncprPtr.Allocate(lBufferSizeUncpr + BlockSizeUncompress);

	z_stream zcpr;
	int ret=Z_OK;
	long lOrigToDo = size;
	long lOrigDone = 0;
	int step = 0;

	memset(&zcpr, 0, sizeof(z_stream));
	inflateInit(&zcpr);

	zcpr.next_in = (BYTE*)pData;
	zcpr.next_out = UncprPtr;

	do
	{
		long all_read_before = zcpr.total_in;
		zcpr.avail_in = min(lOrigToDo,BlockSizeUncompress);
		zcpr.avail_out = BlockSizeUncompress;
		ret=inflate(&zcpr, Z_SYNC_FLUSH);
		lOrigDone += (zcpr.total_in-all_read_before);
		lOrigToDo -= (zcpr.total_in-all_read_before);
		step++;
	} while (ret == Z_OK);

	lSizeUncpr = zcpr.total_out;
	inflateEnd(&zcpr);

	RtlCopyMemory(pData, UncprPtr, lSizeUncpr);

	size = lSizeUncpr;

	return ERROR_SUCCESS;    
}