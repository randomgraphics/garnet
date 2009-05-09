#if GN_MSWIN
#ifndef __GN_BASE_CODEPAGEMSWIN_H__
#define __GN_BASE_CODEPAGEMSWIN_H__
// *****************************************************************************
/// \file
/// \brief   Code page implementation using MS Windows API
/// \author  chen@@CHENLI-HOMEPC (2009.5.9)
// *****************************************************************************

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
        void clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //@{

        size_t
        toUTF16_LE(
            void            * destBuffer,
            size_t            destBufferSizeInBytes,
            const void      * sourceBuffer,
            size_t            sourceBufferSizeInBytes );

        size_t
        fromUTF16_LE(
            void            * destBuffer,
            size_t            destBufferSizeInBytes,
            const void      * sourceBuffer,
            size_t            sourceBufferSizeInBytes );

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

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
