#ifndef CORE_PCH_H
#define CORE_PCH_H

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifndef CORE_EXPORT
	#ifdef DLL_BUILD
		#define CORE_EXPORT __declspec(dllexport)
	#else
		#pragma comment(lib, "core.lib")
		#define CORE_EXPORT __declspec(dllimport)
	#endif
#endif

// STL
#include <string_view>

// WIN
#include <windows.h>
#include <cassert>



#endif //PCH_H
