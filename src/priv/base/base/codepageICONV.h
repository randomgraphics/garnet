#if HAS_ICONV
#ifndef __GN_BASE_CODEPAGEICONV_H__
#define __GN_BASE_CODEPAGEICONV_H__
// *****************************************************************************
/// \file
/// \brief   Code page implementation using GPU ICONV
/// \author  chen@@CHENLI-HOMEPC (2009.5.9)
// *****************************************************************************

#include <iconv.h>

namespace GN
{
    ///
    /// Multibyte encoding implementation class on MS Windows.
    ///
    class CECImplICONV : public StdClass
    {
        GN_DECLARE_STDCLASS( CECImplICONV, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        CECImplICONV()          { clear(); }
        virtual ~CECImplICONV() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( CharacterEncodingConverter::Encoding from, CharacterEncodingConverter::Encoding to );
        void quit();
    private:
        void clear() { mIconv = 0; }
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

        iconv_t mIconv;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}


// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_CODEPAGEICONV_H__
#endif // GN_ICONV
