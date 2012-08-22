// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef _STDAFX_H_
#define _STDAFX_H_

#include "targetver.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#pragma warning(disable : 4995 4005)

// Windows Header Files:
#include <windows.h>
#include <TlHelp32.h>
#include <winternl.h>
#include <ntstatus.h>


// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <strsafe.h>

//C++ RunTime Header Files
#include <vector>
#include <string>
#include <map>
#include <functional>

#ifdef PROC_PROTECT_EXPORTS
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

#endif//_STDAFX_H_
