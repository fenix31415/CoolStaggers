#pragma once

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include "AutoTOML.hpp"

//#define DEBUG

#ifdef DEBUG
#include <spdlog/sinks/basic_file_sink.h>
#else
#include <spdlog/sinks/msvc_sink.h>
#endif

using namespace std::literals;

namespace logger = SKSE::log;

#define DLLEXPORT __declspec(dllexport)

#include "UselessFenixUtils.h"
