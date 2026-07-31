/// \def GN_ENABLE_GUARD       Enable function guard.
///                                  Note that disable this flag will also disable slow guard.
#ifndef GN_ENABLE_GUARD
    #define GN_ENABLE_GUARD 1
#endif

/// \def GN_ENABLE_SLOW_GUARD  Enable function guards in time-critical code.
///                                  Note that this switch is ignored, if GN_ENABLE_GUARD is off.
#ifndef GN_ENABLE_SLOW_GUARD
    #define GN_ENABLE_SLOW_GUARD (GN_BUILD_DEBUG_ENABLED && GN_ENABLE_GUARD)
#endif
