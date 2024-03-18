#ifndef CORE_PCH_H
#define CORE_PCH_H

#define WIN32_LEAN_AND_MEAN

#ifdef CORE_EXPORT
	#define CORE_API __declspec(dllexport)
#else
	#pragma comment(lib, "core.lib")
	#define CORE_API __declspec(dllimport)
#endif

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
#include <unordered_set>

// WIN
#include <cassert>
#include <windows.h>

// USER
#include "Defines.h"

#endif //PCH_H
