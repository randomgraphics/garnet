#include "pch.h"

// Inline backward-cpp signal handling (same as backward.cpp in the submodule).
// Ensures crash handler runs on SIGSEGV/SIGABRT etc. and dumps full stack trace.
#ifdef _MSC_VER
#pragma warning(disable: 4267) // conversion from 'size_t' to 'int', possible loss of data
#pragma warning(disable: 4996) // 'function': was declared deprecated
#endif
#include <backward.hpp>

namespace backward {

backward::SignalHandling sh;

} // namespace backward
