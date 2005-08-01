#ifndef __GN_BASE_NATIVEFILEDEVICE_H__
#define __GN_BASE_NATIVEFILEDEVICE_H__
// *****************************************************************************
//! \file    NativeFileDevice.h
//! \brief   native file device header
//! \author  chenlee (2005.7.11)
// *****************************************************************************

namespace GN { namespace detail {

    //!
    //! Native file device class
    //!
    class NativeFileDevice : public GN::FileDevice
    {
        // ********************************
        //! \name  ctor/dtor
        // ********************************

        //@{
    public:
        NativeFileDevice()          {}
        virtual ~NativeFileDevice() {}
        //@}

        // ********************************
        //   public functions
        // ********************************
    public:

        GN::AutoRef<GN::File>
        openFile( const GN::StrA & path, int mode ) const;

        bool isExist( const StrA & path ) const;

        bool isDir( const StrA & path ) const;

        void
        findFiles( std::vector<GN::StrA> & result,
                   const GN::StrA & dirName,
                   const GN::StrA & pattern,
                   bool recursive,
                   bool useRegex ) const;

        StrA rel2abs( const GN::StrA & relPath, const GN::StrA & base ) const;

        // ********************************
        //   private variables
        // ********************************
    private:

        // ********************************
        //   private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of NativeFileDevice.h
// *****************************************************************************
#endif // __GN_BASE_NATIVEFILEDEVICE_H__
