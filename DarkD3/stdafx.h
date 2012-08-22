#ifndef _STDAFX2_H_
#define _STDAFX2_H_

#include "targetver.h"

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

//Remote call method selection
#define USE_NOTHING		0					// Only memory writes are used to interact with process
#define USE_REMOTE_THD	1					// Remote thread creation with codecave
#define USE_APC			2					// User-mode APC delivered to main thread
#define USE_DLL			3					// Dll is injected and handles calls

//Current remote call method
#ifdef RCALL_TYPE
	#error Only one Remote call type must be selected
#else
	#define RCALL_TYPE	USE_DLL
#endif

//Some unnecessary warnings
#pragma warning(disable : 4995 4996 4793 4100)

//Windows Header Files:
#include <windows.h>
#include <Shlwapi.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <WinIoCtl.h>

//C RunTime Header Files
#include <stdlib.h>
#include <tchar.h>
#include <strsafe.h>

//C++ RunTime Header Files
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <algorithm>

#endif//_STDAFX2_H_
