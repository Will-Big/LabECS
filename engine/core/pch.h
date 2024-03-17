#ifndef CORE_PCH_H
#define CORE_PCH_H

#define WIN32_LEAN_AND_MEAN
#define LIB_BUILD

//#ifndef CORE_EXPORT
//	#elif DLL_BUILD
//		#define CORE_EXPORT __declspec(dllexport)
//	#else
//		#pragma comment(lib, "core.lib")
//		#define CORE_EXPORT __declspec(dllimport)
//	#endif
//#endif

// STL
#include <any>
#include <map>
#include <list>
#include <array>
#include <mutex>
#include <ranges>
#include <memory>
#include <vector>
#include <variant>
#include <algorithm>
#include <functional>
#include <string_view>
#include <shared_mutex>
#include <unordered_map>

// WIN
#include <cassert>
#include <windows.h>

// USER
#include "Defines.h"

#endif //PCH_H
