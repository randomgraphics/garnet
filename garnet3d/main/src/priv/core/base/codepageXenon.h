#if GN_XBOX2
#ifndef __GN_BASE_CODEPAGEXENON_H__
#define __GN_BASE_CODEPAGEXENON_H__
// *****************************************************************************
/// \file
/// \brief   Code page implementation on Xbox 360
/// \author  chen@@CHENLI-HOMEPC (2009.5.9)
// *****************************************************************************

#include <locale>

namespace GN
{
    ///
    /// Multibyte encoding implementation class on Xbox 360.
    ///
    class CECImplXenon : public StdClass
    {
        GN_DECLARE_STDCLASS( CECImplXenon, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        CECImplXenon()          { clear(); }
        virtual ~CECImplXenon() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( CharacterEncodingConverter::Encoding from, CharacterEncodingConverter::Encoding to );
        void quit();
    private:
        void clear() { }
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

        // ********************************
        // private functions
        // ********************************
    private:
    };
}


// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_CODEPAGEXENON_H__
#endif // GN_XBOX2
