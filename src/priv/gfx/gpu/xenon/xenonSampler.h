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
        XenonSamplerObject()          { clear(); }
        virtual ~XenonSamplerObject() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const SamplerDesc & desc );
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// get descriptor
        ///
        const SamplerDesc & getDesc() const { return mDesc; }

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
