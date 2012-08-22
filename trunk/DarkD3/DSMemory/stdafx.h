#ifndef _STDAFX3_H_
#define _STDAFX3_H_

#include "targetver.h"

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#ifdef DSMEMORY_EXPORTS
    #define DSUTILS_API __declspec(dllexport)
#else
    #define DSUTILS_API __declspec(dllimport)
#endif

#define NORETURN

#pragma warning(disable : 4995)

// Windows Header Files:
#include <windows.h>


// C RunTime Header Files
#include <stdlib.h>
#include <stdarg.h>
#include <locale.h>
#include <tchar.h>
#include <strsafe.h>
#include <assert.h>

//C++ RunTime Header Files
#include <memory>
#include <vector>
#include <string>
#include <map>

#endif//_STDAFX3_H_
