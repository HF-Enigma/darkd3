#include "DSString.h"

namespace ds_utils
{
    CDSString::CDSString(void):
        m_pwData(0),
        m_pcData(0),
        m_size(0),
        m_csize(0),
        m_length(0),
        m_currentCP(0),
        m_bConverted(false)
    {
    }

    CDSString::CDSString( const CDSString& str ):
        m_pwData(0),
        m_pcData(0),
        m_size(0),
        m_csize(0),
        m_length(0),
        m_currentCP(0),
        m_bConverted(false)
    {
        assign(str);
    }

    CDSString::CDSString( const wchar_t *str ):
        m_pwData(0),
        m_pcData(0),
        m_size(0),
        m_csize(0),
        m_length(0),
        m_currentCP(0),
        m_bConverted(false)
    {   
        assign(str);
    }

    CDSString::CDSString( const char *str, bool utf8):
        m_pwData(0),
        m_pcData(0),
        m_size(0),
        m_csize(0),
        m_length(0),
        m_currentCP(0),
        m_bConverted(false)
    {        
        assign(str, utf8);
    }

    CDSString::CDSString( size_t number ):
        m_pwData(0),
        m_pcData(0),
        m_size(0),
        m_csize(0),
        m_length(0),
        m_currentCP(0),
        m_bConverted(false)
    {
        assign(number);
    }

    CDSString::~CDSString(void)
    {
        free();
    }

    CDSString& CDSString::assign( const CDSString& str )
    {
        this->m_currentCP = str.m_currentCP;

        this->alloc(str.m_size);

        memcpy(this->m_pwData, str.m_pwData, str.m_size);
        this->m_length = str.m_length;

        m_bConverted = false;

        return (*this);
    }

    CDSString& CDSString::assign( const wchar_t *str )
    {
        size_t len = wcslen(str);

        alloc(align(len));

        memcpy(m_pwData, str, (len+1)*sizeof(wchar_t));

        m_length = len;

        m_bConverted = false;

        return (*this);
    }

    CDSString& CDSString::assign( const char *str, bool utf8 )
    {
        size_t len = strlen(str);
       
        alloc(align(len));

#ifndef _WIN32
        if(!utf8)
        {
            size_t size = 0;
            _locale_t locale = _create_locale(LC_CTYPE, "");

            _mbstowcs_s_l(&size, m_pwData, m_size, str, len + sizeof(char), locale);

            _free_locale(locale);
        }
#else
        if(utf8)
            MultiByteToWideChar(CP_UTF8, 0, str, (int)len + 1, m_pwData, (int)m_size);
        else
            MultiByteToWideChar(CP_ACP, 0, str, (int)len + 1, m_pwData, (int)m_size);
#endif
        m_length = len;

        m_bConverted = false;

        return (*this);
    }

    CDSString& CDSString::assign( size_t number )
    {
        alloc(align(24));

        _ultow_s((unsigned long)number, m_pwData, (unsigned long)m_size, 10);

        m_length = wcslen(m_pwData);

        m_bConverted = false;

        return (*this);
    }

    CDSString& CDSString::assign( int number )
    {
        return assign((size_t)number);
    }

    CDSString& CDSString::operator=( const CDSString& str )
    {
        return assign(str);
    }

    CDSString& CDSString::operator=( const wchar_t *str )
    {
        return assign(str);
    }

    CDSString& CDSString::operator=( const char *str )
    {
        return assign(str);
    }

    CDSString& CDSString::operator=( size_t number )
    {
        return assign(number);
    }

    CDSString& CDSString::operator+( CDSString& str )
    {
        size_t len = wcslen(str.data());

        extend(len);

        wcscat_s(m_pwData, m_size, str.data());

        m_length += len;

        m_bConverted = false;

        return (*this);
    }

    CDSString& CDSString::operator+( const wchar_t *str )
    {
        size_t len = wcslen(str);

        extend(len);

        wcscat_s(m_pwData, m_size, str);

        m_length +=len;

        m_bConverted = false;

        return (*this);
    }

    CDSString& CDSString::operator+( const char *str )
    {
        size_t size = 0;
        size_t len = strlen(str);
        _locale_t locale = _create_locale(LC_CTYPE, "");
        wchar_t* tmp = new wchar_t[len+1];

        extend(len);

        _mbstowcs_s_l(&size, tmp, len+1, str, len + sizeof(char), locale);

        wcscat_s(m_pwData, m_size, tmp);

        m_length += len;

        _free_locale(locale);

        delete[] tmp;

        m_bConverted = false;

        return (*this);
    }

    CDSString& CDSString::operator+( size_t number )
    {
        wchar_t tmp[24] = {0};

        _ultow_s((unsigned long)number, tmp, 10);

        size_t len = wcslen(tmp);

        extend(len);

        wcscat_s(m_pwData, m_size, tmp);

        m_length += len;

        m_bConverted = false;

        return (*this);
    }

    CDSString& CDSString::operator+( int number )
    {
        return (*this+(size_t)number);
    }

    CDSString& CDSString::operator+=( CDSString& str )
    {
        return (*this+str);
    }

    CDSString& CDSString::operator+=( const wchar_t *str )
    {
        return (*this+str);
    }

    CDSString& CDSString::operator+=( const char *str )
    {
        return (*this+str);
    }

    CDSString& CDSString::operator+=( size_t number )
    {
        return (*this+number);
    }

    wchar_t& CDSString::operator[]( size_t pos )
    {
        return m_pwData[pos];
    }

    wchar_t& CDSString::operator[]( int pos )
    {
        return m_pwData[pos];
    }

    bool CDSString::operator==( CDSString& str )
    {
        return (compare(str) == 0);
    }

    bool CDSString::operator!=( CDSString& str )
    {
        return !(*this == str);
    }

    CDSString::operator const wchar_t*()
    {
        return m_pwData;
    }    

    bool CDSString::format( const wchar_t* fmt, ... )
    {
        size_t size = 0;

        alloc(1);
        
        va_list args;
        va_start(args, fmt);
        size = _vsnwprintf_s(m_pwData, m_size, m_size-1, fmt, args);
        va_end(args);

        while(size == (size_t)-1)
        {
            m_length = m_size;
            extend(1);

            va_start(args, fmt);
            size = _vsnwprintf_s(m_pwData, m_size, m_size-1, fmt, args);
            va_end(args);
        }

        m_length = size;

        m_bConverted = false;

        return true;
    }

    bool CDSString::alloc( size_t size, bool aligned/*=true*/ )
    {
        free();

        if(aligned)
            size = align(size);

        m_pwData = new wchar_t[size]();

        if(!m_pwData)
            return false;

        m_size = size;

        return true;
    }

    bool CDSString::realloc( size_t newsize, bool aligned/*=true*/ )
    {
        if(aligned && newsize < m_size)
            return true;

        wchar_t *tmp = new wchar_t[m_size];

        memcpy(tmp, m_pwData, m_size*sizeof(wchar_t));

        if(!alloc(newsize, aligned))
        {
            delete[] tmp;
            return false;
        }

        memcpy(m_pwData, tmp, m_size*sizeof(wchar_t));

        delete[] tmp;

        return true;
    }

    void CDSString::free()
    {
        m_bConverted  = false;

        if(m_pwData)
        {
            delete[] m_pwData;
            m_pwData = 0;
            m_size = 0;
            m_length = 0;
        }
        if(m_pcData)
        {
            delete[] m_pcData;
            m_currentCP = 0;
        }
    }

    const wchar_t* CDSString::data()
    {
        return m_pwData;
    }


    const TCHAR* CDSString::data_t()
    {
#ifdef _UNICODE
        return data();
#else
        return data_mb();
#endif
    }

    const char* CDSString::data_mb()
    {
        if(!m_bConverted)
        {
            if(m_pcData != NULL && m_csize < m_size)
            {
                delete[] m_pcData;
                m_pcData = NULL;

                m_pcData = new char[m_size];
                m_csize = m_size;
            }
            else if(m_pcData == NULL)
            {
                m_pcData = new char[m_size];
                m_csize = m_size;
            }  

            size_t size = 0;
            _locale_t locale = _create_locale(LC_CTYPE, "");

            _wcstombs_s_l(&size, m_pcData, m_csize, m_pwData, m_length, locale);

            _free_locale(locale);

            m_bConverted = true;
        }

        return m_pcData;
    }

    const char* CDSString::data_utf8()
    {
        if(m_pcData != NULL && m_csize < m_size)
        {
            delete[] m_pcData;
            m_pcData = NULL;

            m_pcData = new char[m_size];
            m_csize = m_size;
        }
        else if(m_pcData == NULL)
        {
            m_pcData = new char[m_size];
            m_csize = m_size;
        }  

#ifndef _WIN32
        size_t size = 0;
        size_t len = wcslen(m_pwData);
        _locale_t locale = _create_locale(LC_CTYPE, ".65001");

        locale->locinfo->lc_category[2].locale;

        _wcstombs_s_l(&size, m_pcData, m_csize, m_pwData, m_length, locale);

        _free_locale(locale);
#else
        WideCharToMultiByte(CP_UTF8, 0, m_pwData, (int)m_size, m_pcData, (int)m_csize, NULL, NULL);
#endif
        m_bConverted = false;

        return m_pcData;
    }

    wchar_t* CDSString::buffer()
    {
        return m_pwData;
    }

    size_t CDSString::size()
    {
        return m_size;
    }

    size_t CDSString::length()
    {
        return m_length;
    }

    void CDSString::upd_length()
    {
        m_length = wcslen(m_pwData);
    }

    bool CDSString::empty()
    {
        if(!m_pwData)
            return true;

        return (m_pwData[0] == 0);
    }

    inline size_t CDSString::align( size_t val )
    {
        if(val % m_blocksize == 0)
            return val;
        else
            return ((val / m_blocksize + 1)*m_blocksize);
    }

    void CDSString::extend( size_t amount )
    {
        if((m_length + 1 + amount) > m_size)
            realloc(m_length + 1 + amount);
    } 

    size_t CDSString::find( const wchar_t *str, size_t pos )
    {
        if(pos > m_length)
            return (size_t)-1;

        size_t len = wcslen(str);
        size_t size = m_length - (len-1);
        bool bFound = false;

        for(size_t i = pos; i<size; i++)
        {
            bFound = true;

            for(size_t j=0; j<len; j++)
            {
                if(m_pwData[i+j] != str[j])
                {
                    bFound = false;
                    break;
                }
            }

            if(bFound)
                return i;
        }

        return (size_t)-1;
    }

    size_t CDSString::find( const char *str, size_t pos /*= 0*/ )
    {
        CDSString strTmp(str);

        return find(strTmp, pos);
    }

    size_t CDSString::find_any( std::vector<CDSString>& strings, size_t& index, size_t pos/*=0*/ )
    {
        size_t len = m_length;
        bool bFound = false;
        size_t size = 0;

        if(pos > m_length)
            return (size_t)-1;

        //Minimum length of all strings
        for(size_t i=0; i<strings.size(); i++)
        {
            if(strings[i].length() < len)
                len = strings[i].length();
        }

        size = m_length - len;

        for(size_t i=pos; i<size; i++)
            for(size_t k=0; k<strings.size(); k++)
            {
                bFound = true;

                for(size_t j=0; j<strings[k].length(); j++)
                    if((i+j) > m_length || m_pwData[i+j] != strings[k][j])
                    {
                        bFound = false;
                        break;
                    }

                if(bFound)
                {
                    index = k;
                    return i;
                }
            }

        return (size_t)-1;
    }

    size_t CDSString::rfind( const wchar_t *str, size_t pos )
    {
        bool bFound = false;

        if(str == 0 || pos > m_length || pos < 0)
            return (size_t)-1;

        size_t len = wcslen(str);

        if (pos == 0)
            pos = m_length - len;
        
        if (pos + len > m_length)
            return (size_t)-1;
        
        for(int i=(int)pos; i>-1; i--)
        {
            bFound = true;

            for(size_t j=0; j<len; j++)
            {
                if(m_pwData[i+j] != str[j])
                {
                    bFound = false;
                    break;
                }
            }

            if(bFound)
                return i;
        }

        return (size_t)-1;
    }

    size_t CDSString::rfind( const char *str, size_t pos /*= 0*/ )
    {
        CDSString strTmp(str);

        return rfind(strTmp, pos);
    }

    size_t CDSString::rfind_any( std::vector<CDSString>& strings, size_t& index, size_t pos/*=0*/ )
    {
        bool bFound = false;

        if(pos > m_length)
            return (size_t)-1;

        if (pos == 0)
            pos = m_length;

        for(int i=(int)pos; i>-1; i--)
            for(size_t k=0; k<strings.size(); k++)
            {
                bFound = true;

                for(size_t j=0; j<strings[k].length(); j++)
                    if((i+j) > pos || m_pwData[i+j] != strings[k][j])
                    {
                        bFound = false;
                        break;
                    }

                if(bFound)
                {
                    index = k;
                    return i;
                }
            }

        return (size_t)-1;
    }


    ds_utils::CDSString CDSString::substr( size_t pos, size_t count /*= npos*/ )
    {
        CDSString out;

        if(pos >= m_length)
            return CDSString(L"");

        if(count == npos)
            count = m_length - pos;

        out.alloc(count);

        for(size_t i=pos; i<pos+count; i++)
            out[i-pos] = m_pwData[i];

        out[count] = L'\0';

        out.m_length = count;

        return out;
    }

    int CDSString::compare(CDSString& str )
    {
        return compare(str.data());
    }

    int CDSString::compare( const wchar_t* str )
    {
        size_t len = wcslen(str);

        if(len > m_length)
            return 1;

        else if(len < m_length)
            return -1;

        for(size_t i=0; i<m_length; i++)
        {
            if(m_pwData[i] > str[i])
                return 1;

            else if(m_pwData[i] < str[i])
                return -1;
        }

        return 0;
    }

    int CDSString::compare( const char* str )
    {
        CDSString tmp(str);

        return compare(tmp);
    }

    void CDSString::split( std::vector<CDSString>& delims, std::vector<CDSString>& out )
    {
        size_t index = 0;
        size_t pos = 0;
        CDSString tmp = *this;
        
        while(pos != npos)
        {
            tmp = tmp.substr(pos);
            pos = tmp.find_any(delims, index);

            if(pos == 0)
            {
                pos++;
                continue;
            }

            out.push_back(tmp.substr(0, pos));

            if(pos != npos)
                pos += delims[index].length();
        }
    }

    void CDSString::erase( size_t pos, size_t num /*= 1*/ )
    {
        if(pos > m_length )
            return;

        if(pos + num > m_length)
        {
            memset(m_pwData + pos, 0x00, m_length - pos);
            m_length = pos;
            return;
        }

        for(size_t i=pos; i<length()-num; i++)
            m_pwData[i] = m_pwData[i+num];

        memset(m_pwData + (m_length-num), 0x00, num*sizeof(wchar_t));

        m_length -= num; 
    }

    void CDSString::trim_all()
    {
        for(size_t i=0; i<m_length;)
            if(m_pwData[i] == L' ')
                erase(i);
            else
                i++;
    }

    void CDSString::replace_spaces( wchar_t newsym )
    {
        for (size_t i = 0; i < m_length; i++)
            if (m_pwData[i] == L' ')
                m_pwData[i] = newsym;
    }

    void CDSString::insert( size_t pos, CDSString& str )
    {
        return insert(pos, str.data());
    }

    void CDSString::insert( size_t pos, const wchar_t* str )
    {
        size_t len = wcslen(str);
        wchar_t *tmp = new wchar_t[m_length - pos];

        memcpy(tmp, m_pwData + pos, (m_length - pos)*sizeof(wchar_t));

        realloc(m_length + len);

        memcpy(m_pwData + pos, str, len*sizeof(wchar_t));
        memcpy(m_pwData + pos + len, tmp, (m_length - pos)*sizeof(wchar_t));

        m_length += len;

        delete[] tmp;
    }

    void CDSString::insert( size_t pos, const char* str )
    {
        CDSString tmp(str);

        insert(pos, tmp);
    }
}
