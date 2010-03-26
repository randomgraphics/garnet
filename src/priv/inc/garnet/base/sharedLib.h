#ifndef __GN_BASE_SHAREDLIB_H__
#define __GN_BASE_SHAREDLIB_H__
// *****************************************************************************
/// \file
/// \brief   Shared library utilities
/// \author  chenlee (2005.10.4)
// *****************************************************************************

namespace GN
{
    ///
    /// Shared library class
    ///
    class SharedLib : public NoCopy
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        SharedLib() : mHandle(0) {}
        virtual ~SharedLib() { Free(); }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// Load shared library.
        ///
        /// \param libName
        ///     If the libName has no extension. A platform dependent extension
        ///     will be appended to libName: ".dll" on Windows, ".so" on linux.
        ///     However, libName can include a trailing point character (.) to
        ///     indicate that the library name has no extension.
        ///
        bool Load( const char * libName );

        ///
        /// Free the library.
        ///
        /// \note All symbols of the library will be invalidated as well.
        ///
        void Free();

        ///
        /// Get library handle
        ///
        void * GetHandle() const { return mHandle; }

        ///
        /// Get pointer of specified symbol of the shared library.
        ///
        void * GetSymbol( const char * symbol );

        // ********************************
        // private variables
        // ********************************
    private:

        void * mHandle;

        StrA mFileName;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_SHAREDLIB_H__
