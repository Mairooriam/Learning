#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>

// include the formatter specialization
#include "Mir/Log.h"
#include "spdlog/fmt/ostr.h"
#include "Mir/Utils/Utils.h"

#ifdef MIR_PLATFORM_WINDOWS
    #include <Windows.h>
#endif