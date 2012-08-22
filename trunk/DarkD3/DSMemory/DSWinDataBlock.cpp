#include "DSWinDataBlock.h"

namespace ds_utils
{
    namespace ds_memory
    {
        unsigned int CDSWinDataBlock::m_pagesize = 0;

        CDSWinDataBlock::CDSWinDataBlock( void ) :
            CDSDataBlock()
        {
            if(m_pagesize == 0)
            {
                SYSTEM_INFO info = {0};

                GetNativeSystemInfo(&info);
                m_pagesize = info.dwPageSize;
            }
        }

        CDSWinDataBlock::~CDSWinDataBlock( void )
        {
            Free();
        }

        unsigned char* CDSWinDataBlock::Allocate( size_t size )
        {
            Free();

            m_size = size;

            if(size == 0)
                return NULL;

            if(m_size >= m_pagesize)
                m_pData = (unsigned char*)VirtualAlloc(NULL, m_size, MEM_COMMIT, PAGE_READWRITE);
            else
                m_pData = (unsigned char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);

            StoreRecord(dynamic_cast<CDSDataBlock*>(this));
            Reference();

            return m_pData;
        }

        unsigned char* CDSWinDataBlock::Reallocate( size_t newsize )
        {
            if(newsize == 0)
                return NULL;

            if(m_size >= m_pagesize)
            {
                size_t size = m_size;
                void* pTempData = NULL;

                pTempData = (unsigned char*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);

                RtlCopyMemory(pTempData, m_pData, size);

                Allocate(newsize);

                if(newsize > size)
                    RtlCopyMemory(m_pData, pTempData, size);
                else
                    RtlCopyMemory(m_pData, pTempData, newsize);

                VirtualFree(pTempData, 0, MEM_RELEASE);
            }
            else
                m_pData = (unsigned char*)HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, m_pData, newsize);

            return m_pData;
        }

        void CDSWinDataBlock::Free()
        {
            if(m_pData != NULL )
            {
                int refs = GetRefCount();

                assert(refs > 0);

                Dereference();

                if(refs == 1)
                {
                    if(m_size >= m_pagesize)
                        VirtualFree(m_pData, 0, MEM_RELEASE);
                    else
                        HeapFree(GetProcessHeap(), 0, m_pData);
                }

                EraseRecord(this, (refs == 1)); 

                m_pData = NULL;
                m_size = NULL;

            }  
        }       
    }
}