#ifndef __GN_GFXD3D9_D3D9SAMPLER_H__
#define __GN_GFXD3D9_D3D9SAMPLER_H__
// *****************************************************************************
//! \file    d3d9/d3d9Sampler.h
//! \brief   D3D9 sampler object
//! \author  chenli@@FAREAST (2007.6.11)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// D3D9 sampler object
    ///
    class D3D9SamplerObject : public StdClass
    {
        GN_DECLARE_STDCLASS( D3D9SamplerObject, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9SamplerObject()          { clear(); }
        virtual ~D3D9SamplerObject() { quit(); }
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
//                           End of d3d9Sampler.h
// *****************************************************************************
#endif // __GN_GFXD3D9_D3D9SAMPLER_H__
