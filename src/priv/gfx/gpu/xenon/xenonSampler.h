#ifndef __GN_GFX_XENONSAMPLER_H__
#define __GN_GFX_XENONSAMPLER_H__
// *****************************************************************************
/// \file
/// \brief   Xenon sampler object
/// \author  chenli@@FAREAST (2007.6.11)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// Xenon sampler object
    ///
    class XenonSamplerObject : public StdClass
    {
        GN_DECLARE_STDCLASS( XenonSamplerObject, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        XenonSamplerObject()          { Clear(); }
        virtual ~XenonSamplerObject() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( const SamplerDesc & desc );
        void Quit();
    private:
        void Clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// get descriptor
        ///
        const SamplerDesc & GetDesc() const { return mDesc; }

        ///
        /// bind
        ///
        void bind( UInt32 stage ) const;

        // ********************************
        // private variables
        // ********************************
    private:

        SamplerDesc mDesc;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_XENONSAMPLER_H__
