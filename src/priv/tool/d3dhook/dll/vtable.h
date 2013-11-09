#pragma once

#include <garnet/GNbase.h>

#define HOOK_ERROR_LOG(...) GN_ERROR(GN::getLogger("GN.d3dhook"))(__VA_ARGS__)
#define HOOK_INFO_LOG(...)  GN_INFO(GN::getLogger("GN.d3dhook"))(__VA_ARGS__)
#define HOOK_WARN_LOG(...)  GN_WARN(GN::getLogger("GN.d3dhook"))(__VA_ARGS__)
#define HOOK_ASSERT(x)      GN_ASSERT(x)
#define HOOK_RIP()          GN_UNEXPECTED()
