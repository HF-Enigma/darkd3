#ifndef _DS_WIN_FILE_H_
#define _DS_WIN_FILE_H_

#include "stdafx.h"
#include "DSFile.hpp"

namespace ds_utils
{
    namespace ds_fs
    {
        template<class HEADER>
        class CDSWinFile : CDSFile<HEADER>
        {
        public:
            CDSWinFile(void):
                m_hFile(INVALID_HANDLE_VALUE)
            {
            }

            ~CDSWinFile(void)
            {
                Close();
            }

            virtual bool Open(CDSString name, bool bCreateNew = false)
            {   
                bool bResult = true;
                DWORD dwFlags = 0;

                if(m_hFile != INVALID_HANDLE_VALUE)
                {
                    SetLastError(ERROR_ALREADY_EXISTS);
                    return false;
                }

                if(bCreateNew)
                    dwFlags |= OPEN_ALWAYS;
                else
                    dwFlags |= OPEN_EXISTING;

                m_hFile = CreateFile
                    (
                        name.data_t(),
                        FILE_GENERIC_READ|FILE_GENERIC_WRITE,
                        FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
                        NULL, dwFlags, NULL, NULL
                    );

                if(m_hFile == INVALID_HANDLE_VALUE)
                    bResult = false;

                return bResult;
            }   

            virtual bool Create(CDSString name)
            {
                bool bResult = true;

                if(m_hFile != INVALID_HANDLE_VALUE)
                {
                    SetLastError(ERROR_ALREADY_EXISTS);
                    return false;
                }

                m_hFile = CreateFile
                    (
                        name.data_t(),
                        FILE_GENERIC_READ|FILE_GENERIC_WRITE,
                        FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
                        NULL, CREATE_ALWAYS, NULL, NULL
                    );

                if(m_hFile == INVALID_HANDLE_VALUE)
                    bResult = false;

                return bResult;
            }

            virtual bool Write(size_t pos, CDSDataBlock data, size_t size = -1)
            {
                bool bResult = true;
                LARGE_INTEGER filepos = {0};
                DWORD dwBytes = 0;

                if(size == -1)
                    size = data.Size();

                if(m_hFile == INVALID_HANDLE_VALUE)
                {
                    SetLastError(ERROR_INVALID_HANDLE);
                    return false;
                }

                filepos.QuadPart = pos;

                if(!SetFilePointerEx(m_hFile, filepos, NULL, FILE_BEGIN))
                    return false;

                if(!WriteFile(m_hFile, data.Data(), size, &dwBytes, NULL) || dwBytes != size)
                    bResult = false;

                return bResult;
            }

            virtual bool Read(size_t pos, CDSDataBlock& data, size_t size)
            {
                bool bResult = true;
                LARGE_INTEGER filepos = {0};
                DWORD dwBytes = 0;

                if(m_hFile == INVALID_HANDLE_VALUE)
                    return false;

                filepos.QuadPart = pos;

                if(!SetFilePointerEx(m_hFile, filepos, NULL, FILE_BEGIN))
                    return false;

                if(m_hFile == INVALID_HANDLE_VALUE)
                {
                    SetLastError(ERROR_INVALID_HANDLE);
                    return false;
                }

                if (size > data.Size())
                {
                    SetLastError(ERROR_INSUFFICIENT_BUFFER);
                    return false;
                }

                if(!ReadFile(m_hFile, data.Data(), size, &dwBytes, NULL) || dwBytes != size)
                    bResult = false;

                return bResult;
            }

            virtual bool ReadHeader(HEADER& header)
            {
                bool bResult = true;
                LARGE_INTEGER filepos = {0};
                DWORD dwBytes = 0;

                if(m_hFile == INVALID_HANDLE_VALUE)
                {
                    SetLastError(ERROR_INVALID_HANDLE);
                    return false;
                }

                if(!SetFilePointerEx(m_hFile, filepos, NULL, FILE_BEGIN))
                    return false;
                
                if(!ReadFile(m_hFile, &header, sizeof(header), &dwBytes, NULL) || dwBytes != sizeof(header))
                    bResult = false;

                return bResult;
            }

            virtual bool WriteHeader(const HEADER& header)
            {
                bool bResult = true;
                LARGE_INTEGER filepos = {0};
                DWORD dwBytes = 0;

                if(m_hFile == INVALID_HANDLE_VALUE)
                {
                    SetLastError(ERROR_INVALID_HANDLE);
                    return false;
                }

                if(!SetFilePointerEx(m_hFile, filepos, NULL, FILE_BEGIN))
                    return false;

                if(!WriteFile(m_hFile, &header, sizeof(header), &dwBytes, NULL) || dwBytes != sizeof(header))
                    bResult = false;

                return bResult;
            }

            virtual unsigned long long Size()
            {
                LARGE_INTEGER liSize = {0};

                if(m_hFile == INVALID_HANDLE_VALUE)
                {
                    SetLastError(ERROR_INVALID_HANDLE);
                    return 0;
                }

                if(!GetFileSizeEx(m_hFile, &liSize))
                    return 0;

                return liSize.QuadPart;
            }

            virtual void Close()
            {
                if(m_hFile != INVALID_HANDLE_VALUE)
                {
                    CloseHandle(m_hFile);
                    m_hFile = INVALID_HANDLE_VALUE;
                }
            }

            virtual unsigned int LastError()
            {
                return GetLastError();
            }

        private:
            HANDLE m_hFile;
        };
    }
}

#endif//_DS_WIN_FILE_H_