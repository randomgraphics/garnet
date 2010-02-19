#if GN_MSWIN
#ifndef __GN_BASE_CODEPAGEMSWIN_H__
#define __GN_BASE_CODEPAGEMSWIN_H__
// *****************************************************************************
/// \file
/// \brief   Code page implementation using MS Windows API
/// \author  chen@@CHENLI-HOMEPC (2009.5.9)
// *****************************************************************************

#include <locale>

namespace GN
{
    ///
    /// Multibyte encoding implementation class on MS Windows.
    ///
    class CECImplMSWIN : public StdClass
    {
        GN_DECLARE_STDCLASS( CECImplMSWIN, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        CECImplMSWIN()          { Clear(); }
        virtual ~CECImplMSWIN() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( CharacterEncodingConverter::Encoding from, CharacterEncodingConverter::Encoding to );
        void Quit();
    private:
        void Clear() { mLocaleFrom = mLocaleTo = 0; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //@{

        size_t
        convert(
            void         * destBuffer,
            size_t         destBufferSizeInBytes,
            const void   * sourceBuffer,
            size_t         sourceBufferSizeInBytes );

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        CharacterEncodingConverter::Encoding mEncodingFrom;
        CharacterEncodingConverter::Encoding mEncodingTo;

        std::locale * mLocaleFrom;
        std::locale * mLocaleTo;

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
