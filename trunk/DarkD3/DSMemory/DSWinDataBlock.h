#ifndef _WIN_DATA_BLOCK_H_
#define _WIN_DATA_BLOCK_H_

#include "DSMemory.h"
#include <Windows.h>

namespace ds_utils
{
    namespace ds_memory
    {
        class CDSWinDataBlock : public CDSDataBlock
        {
        public:
            DSUTILS_API NORETURN                    CDSWinDataBlock(void);
            DSUTILS_API NORETURN                    ~CDSWinDataBlock(void);

            DSUTILS_API virtual unsigned char*      Allocate(size_t size);
            DSUTILS_API virtual unsigned char*      Reallocate(size_t newsize);
            DSUTILS_API virtual void                Free();

        private:
            static unsigned int                     m_pagesize;
        };
    }
}

#endif//_WIN_DATA_BLOCK_H_