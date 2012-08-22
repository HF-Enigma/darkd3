#ifndef _DS_MEMORY_H_
#define _DS_MEMORY_H_

#include "stdafx.h"

namespace ds_utils
{
    namespace ds_memory
    {
            DSUTILS_API void                    CleanUp();

        class CDSDataBlock
        {
        public: 
            DSUTILS_API NORETURN                CDSDataBlock(void);
            DSUTILS_API NORETURN                CDSDataBlock(const CDSDataBlock& blk);
            DSUTILS_API virtual  NORETURN       ~CDSDataBlock(void);

            DSUTILS_API CDSDataBlock&           Assign(const CDSDataBlock& blk);
            DSUTILS_API unsigned char*          AllocateAndCopyFrom(CDSDataBlock& blk);
            DSUTILS_API unsigned char*          AllocateAndCopyFrom(unsigned char* pPtr, size_t size);

            DSUTILS_API CDSDataBlock&           operator =(const CDSDataBlock& blk);
            DSUTILS_API unsigned char&          operator[](int pos);
            DSUTILS_API virtual bool            operator ==(CDSDataBlock& blk);
            DSUTILS_API virtual bool            operator !=(CDSDataBlock& blk);
            DSUTILS_API operator                unsigned char*();
            DSUTILS_API operator                void*();

            
            DSUTILS_API virtual unsigned char*  Allocate(size_t size);
            DSUTILS_API virtual unsigned char*  Reallocate(size_t newsize);
            DSUTILS_API virtual void            Free();
            DSUTILS_API static void             CleanUp();
            DSUTILS_API unsigned char*          Data();
            DSUTILS_API size_t                  Size();
              
        protected:
            DSUTILS_API void                    StoreRecord(CDSDataBlock* ptr);
            DSUTILS_API void                    EraseRecord(CDSDataBlock* ptr, bool bDereference=true);
            DSUTILS_API void                    Reference();
            DSUTILS_API void                    Dereference();
            DSUTILS_API int                     GetRefCount();

        protected:
            unsigned char*                      m_pData;
            size_t                              m_size;

        private:
            static std::vector<CDSDataBlock*>   ms_Allocations;
            static std::map<void*, int>         ms_References;
        };    
    }
}

#endif//_DS_MEMORY_H_