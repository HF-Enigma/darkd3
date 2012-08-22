#ifndef _DS_FILE_H_
#define _DS_FILE_H_

#include "stdafx.h"
#include "DSString.h"
#include "DSMemory.h"

using namespace ds_utils::ds_memory;

namespace ds_utils
{
    namespace ds_fs
    {
        template<class HEADER>
        class CDSFile
        {
        public:
            CDSFile(void)
            {
            }

            ~CDSFile(void)
            {
            }

            virtual bool Open(CDSString name, bool bCreateNew = false) = 0;
            virtual bool Create(CDSString name) = 0;
            virtual void Close() = 0;

            virtual bool Write(size_t pos, CDSDataBlock data, size_t size = -1) = 0;
            virtual bool Read(size_t pos, CDSDataBlock& data, size_t size) = 0;

            virtual bool ReadHeader(HEADER& header) = 0;
            virtual bool WriteHeader(const HEADER& header) = 0;

            virtual unsigned long long Size() = 0;
            virtual unsigned int LastError() = 0;
        };
    }
}

#endif//_DS_FILE_H_