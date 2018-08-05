#if GN_WINPC
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
        CECImplMSWIN()          { clear(); }
        virtual ~CECImplMSWIN() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( CharacterEncodingConverter::Encoding from, CharacterEncodingConverter::Encoding to );
        void quit();
    private:
        void clear() { mCodePageFrom = mCodePageTo = -1; }
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

        int mCodePageFrom;
        int mCodePageTo;

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
#endif // GN_WINPC
