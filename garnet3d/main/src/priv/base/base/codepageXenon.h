#if GN_XENON
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
        CECImplXenon()          { Clear(); }
        virtual ~CECImplXenon() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( CharacterEncodingConverter::Encoding from, CharacterEncodingConverter::Encoding to );
        void Quit();
    private:
        void Clear() { }
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
#endif // GN_XENON
