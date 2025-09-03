#ifdef HAS_FBX
    #if GN_GNUC
        #pragma GCC diagnostic ignored "-Wunused"
        #pragma GCC diagnostic ignored "-Wunused-value"
        #pragma GCC diagnostic ignored "-Wunused-parameter"
        #pragma GCC diagnostic ignored "-Wignored-qualifiers"
        #pragma GCC diagnostic ignored "-Wclass-memaccess"
        #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
        #pragma GCC diagnostic ignored "-Wdeprecated-copy"
    #endif
    #if GN_MSVC
        #pragma warning(disable : 4996) // 'x' was declared depreciated
    #endif
    #define FBXSDK_SHARED
    #include <fbxsdk.h>
#endif
