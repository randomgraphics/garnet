#ifndef __GN_BASE_SHAREDLIB_H__
#define __GN_BASE_SHAREDLIB_H__
// *****************************************************************************
/// \file
/// \brief   Shared library utilities
/// \author  chenlee (2005.10.4)
// *****************************************************************************

namespace GN {
///
/// Shared library class
///
class GN_API SharedLib : public NoCopy {
    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    SharedLib(): mHandle(0) {}
    virtual ~SharedLib() { unload(); }
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
    bool load(const char * libName);

    ///
    /// Free the library.
    ///
    /// \note All symbols of the library will be invalidated as well.
    ///
    void unload();

    ///
    /// Get library handle
    ///
    void * getHandle() const { return mHandle; }

    ///
    /// Get pointer of specified symbol of the shared library.
    ///
    void * getSymbol(const char * symbol);

    // ********************************
    // private variables
    // ********************************
private:
    void * mHandle;

    std::string mFileName;

    // ********************************
    // private functions
    // ********************************
private:
};
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_SHAREDLIB_H__
