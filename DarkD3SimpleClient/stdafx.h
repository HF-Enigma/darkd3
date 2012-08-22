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

//#define GDIPVER 0x0110

#pragma warning(disable : 4995)

// Windows Header Files:
#include <windows.h>
#include <CommCtrl.h>
#include <CommDlg.h>
#include <objidl.h>
#include <gdiplus.h>

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

#pragma comment(lib, "gdiplus.lib")

#endif//_STDAFX_H_
