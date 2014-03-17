#ifndef _DSSTING_H_
#define _DSSTING_H_

#include "stdafx.h"

namespace ds_utils
{
    class CDSString
    {
    public:
        DSUTILS_API NORETURN        CDSString(void);
        DSUTILS_API NORETURN        CDSString(const CDSString& str);
        DSUTILS_API NORETURN        CDSString(const wchar_t *str);
        DSUTILS_API NORETURN        CDSString(const char *str, bool utf8 = false);
        DSUTILS_API NORETURN        CDSString(size_t number);
        DSUTILS_API NORETURN        ~CDSString(void);

        DSUTILS_API CDSString&      assign(const CDSString& str);
        DSUTILS_API CDSString&      assign(const wchar_t *str);
        DSUTILS_API CDSString&      assign(const char *str, bool utf8 = false);
        DSUTILS_API CDSString&      assign(size_t number);
        DSUTILS_API CDSString&      assign(int number);

        DSUTILS_API CDSString&      operator =(const CDSString& str);
        DSUTILS_API CDSString&      operator =(const wchar_t *str);
        DSUTILS_API CDSString&      operator =(const char *str);
        DSUTILS_API CDSString&      operator =(size_t number);

        DSUTILS_API CDSString&      operator +(CDSString& str);
        DSUTILS_API CDSString&      operator +(const wchar_t *str);
        DSUTILS_API CDSString&      operator +(const char *str);
        DSUTILS_API CDSString&      operator +(size_t number);
        DSUTILS_API CDSString&      operator +(int number);

        DSUTILS_API CDSString&      operator +=(CDSString& str);
        DSUTILS_API CDSString&      operator +=(const wchar_t *str);
        DSUTILS_API CDSString&      operator +=(const char *str);
        DSUTILS_API CDSString&      operator +=(size_t number);

        DSUTILS_API wchar_t&        operator [](size_t pos);
        DSUTILS_API wchar_t&        operator [](int pos);

        DSUTILS_API bool            operator ==(CDSString& str);
        DSUTILS_API bool            operator !=(CDSString& str);

        DSUTILS_API operator        const wchar_t*();

        DSUTILS_API size_t          find(const wchar_t *str, size_t pos = 0);
        DSUTILS_API size_t          find(const char *str, size_t pos = 0);
        DSUTILS_API size_t          find_any(std::vector<CDSString>& strings, size_t& index, size_t pos = 0);

        DSUTILS_API size_t          rfind(const wchar_t *str, size_t pos = 0);
        DSUTILS_API size_t          rfind(const char *str, size_t pos = 0);
        DSUTILS_API size_t          rfind_any(std::vector<CDSString>& strings, size_t& index, size_t pos = 0);

        DSUTILS_API int             compare(CDSString& str);
        DSUTILS_API int             compare(const wchar_t* str);
        DSUTILS_API int             compare(const char* str);

        DSUTILS_API void            insert(size_t pos, CDSString& str);
        DSUTILS_API void            insert(size_t pos, const wchar_t* str);
        DSUTILS_API void            insert(size_t pos, const char* str);

        DSUTILS_API CDSString       substr(size_t pos, size_t count = npos);
        DSUTILS_API bool            format(const wchar_t* fmt, ...);
        DSUTILS_API void            split(std::vector<CDSString>& delims, std::vector<CDSString>& out);
        DSUTILS_API void            erase(size_t pos, size_t num = 1);
        DSUTILS_API void            trim_all();
        DSUTILS_API void            replace_spaces(wchar_t newsym);

        DSUTILS_API const wchar_t*  data();
        DSUTILS_API const char*     data_mb();
        DSUTILS_API const TCHAR*    data_t();
        DSUTILS_API const char*     data_utf8();
        DSUTILS_API wchar_t*        buffer();
        DSUTILS_API size_t          size();
        DSUTILS_API size_t          length();
        DSUTILS_API void            upd_length();
        DSUTILS_API bool            empty();

        DSUTILS_API bool            alloc(size_t size, bool aligned=true);
        DSUTILS_API bool            realloc(size_t newsize, bool aligned=true);
        DSUTILS_API void            free();

    public:
        static DSUTILS_API
        const  size_t   npos = (size_t)-1;      //Invalid character position

    private:
        void            extend(size_t amount);  //extend current buffer by amount of characters

        inline 
        static size_t   align(size_t val);      //align value on block size

    private:
        wchar_t        *m_pwData;               //internal character buffer
        char           *m_pcData;               //buffer converted to multi-byte
        size_t          m_length;               //actual buffer length
        size_t          m_size;                 //allocated space for buffer
        size_t          m_csize;                //length of multi-byte string
        unsigned int    m_currentCP;            //current codepage in multi-byte buffer
        bool            m_bConverted;           //Indicates that string is already converted to multi-byte 
                                                //and there is no need for conversion

        static const size_t  m_blocksize = 260; //Minimal allocation block size
    };
};

#endif//_DSSTING_H_