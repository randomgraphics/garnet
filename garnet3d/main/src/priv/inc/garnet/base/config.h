/// \def GN_ENABLE_DEBUG              Enable debug code
#ifndef GN_ENABLE_DEBUG
#define GN_ENABLE_DEBUG 0
#endif

/// \def GN_ENABLE_PROFILING            Enable profiling code
#ifndef GN_ENABLE_PROFILING
#define GN_ENABLE_PROFILING 0
#endif

/// \def GN_ENABLE_GUARD       Enable function guard.
///                                  Note that disable this flag will also disable slow guard.
#ifndef GN_ENABLE_GUARD
#define GN_ENABLE_GUARD 1
#endif

/// \def GN_ENABLE_SLOW_GUARD  Enable function guards in time-critical code.
///                                  Note that this switch is ignored, if GN_ENABLE_GUARD is off.
#ifndef GN_ENABLE_SLOW_GUARD
#define GN_ENABLE_SLOW_GUARD (GN_ENABLE_DEBUG && GN_ENABLE_GUARD)
#endif

/// \def GN_ENABLE_LOG         Enable runtime log.
#ifndef GN_ENABLE_LOG
#define GN_ENABLE_LOG 1
#endif

/// \def GN_ENABLE_ASSERT      Enable assert macro
#ifndef GN_ENABLE_ASSERT
#define GN_ENABLE_ASSERT GN_ENABLE_DEBUG
#endif

/// \def GN_ENABLE_GPU_DEBUG_MARK      Enable GPU Debug markers
#ifndef GN_ENABLE_GPU_DEBUG_MARK
#define GN_ENABLE_GPU_DEBUG_MARK (GN_ENABLE_DEBUG || GN_ENABLE_PROFILING)
#endif
