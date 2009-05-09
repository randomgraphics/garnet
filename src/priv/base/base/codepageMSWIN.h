#if GN_MSWIN
#ifndef __GN_BASE_CODEPAGEMSWIN_H__
#define __GN_BASE_CODEPAGEMSWIN_H__
// *****************************************************************************
/// \file
/// \brief   Code page implementation using MS Windows API
/// \author  chen@@CHENLI-HOMEPC (2009.5.9)
// *****************************************************************************

#include <locale.h>

namespace GN
{
    ///
    /// Multibyte encoding implementation class on MS Windows.
    ///
    class MBCEImplMSWIN : public StdClass
    {
        GN_DECLARE_STDCLASS( MBCEImplMSWIN, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        MBCEImplMSWIN()          { clear(); }
        virtual ~MBCEImplMSWIN() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( MultiByteCharacterEncoding::Enum e );
        void quit();
    private:
        void clear() { mLocale = 0; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //@{

        size_t
        toUTF16_LE(
            wchar_t         * destBuffer,
            size_t            destBufferSizeInBytes,
            const char      * sourceBuffer,
            size_t            sourceBufferSizeInBytes );

        size_t
        fromUTF16_LE(
            char            * destBuffer,
            size_t            destBufferSizeInBytes,
            const wchar_t   * sourceBuffer,
            size_t            sourceBufferSizeInBytes );

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        _locale_t mLocale;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}


// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_CODEPAGEMSWIN_H__
#endif // GN_MSWIN
