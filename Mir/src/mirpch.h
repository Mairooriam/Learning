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

// Include spdlog
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

// Include glfw
#include <GLFW/glfw3.h>


#ifdef MIR_PLATFORM_WINDOWS
    #include <windows.h>
#endif