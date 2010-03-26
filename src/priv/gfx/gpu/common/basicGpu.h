#ifndef __GN_GPUCOMMON_BASICGPU_H__
#define __GN_GPUCOMMON_BASICGPU_H__
// *****************************************************************************
/// \file
/// \brief   Basic GPU class
/// \author  chenlee (2005.10.1)
// *****************************************************************************

///
/// Rest-in-peace macro
///
#define GN_GPU_RIP GN::gfx::rip

namespace GN { namespace gfx
{
    ///
    /// reset in peace...
    ///
    void rip( const char * msg, ... );

    ///
    /// basic GPU class
    ///
    class BasicGpu : public Gpu, public StdClass
    {
        GN_DECLARE_STDCLASS( BasicGpu, StdClass );

        // ********************************
        // constructor/destructor
        // ********************************

        //@{
    public:
        BasicGpu() { Clear(); }
        virtual ~BasicGpu() {}
        //@}

        // ********************************
        // standard init / quit
        // ********************************

        //@{
    public:
        bool Init( const GpuOptions & );
        void Quit();
    private :
        void Clear()
        {
            contextClear();
            miscClear();
        }
        //@}

        // *****************************************************************************
        //
        /// \name                   Display Manager
        //
        // *****************************************************************************

        //@{

     protected:

        ///
        /// Called by sub class to respond to render window resizing/moving
        ///
        virtual void handleRenderWindowSizeMove() = 0;

        //@}

        // *****************************************************************************
        //
        /// \name                   Caps Manager
        //
        // *****************************************************************************

        //@{

        //@}

        // *****************************************************************************
        //
        /// \name                   Resource Manager
        //
        // *****************************************************************************

        //@{

        //@}

        // *****************************************************************************
        //
        /// \name                   Context Manager
        //
        // *****************************************************************************

        //@{

    public:

        virtual void bindContext( const GpuContext & c );
        virtual void rebindContext();
        virtual inline const GpuContext & getContext() const { return mContext; }

    protected:

        virtual bool bindContextImpl( const GpuContext & context, bool skipDirtyCheck ) = 0;

    private:

        void contextClear() { mContextOk = false; mContext.Clear(); }

    protected:

        GpuContext mContext;
        bool            mContextOk;

        //@}

        // *****************************************************************************
        //
        /// \name                   Draw Manager
        //
        // *****************************************************************************

        //@{

        //@}

        // *****************************************************************************
        //
        /// \name                   Misc Manager
        //
        // *****************************************************************************

        //@{

    public:

        virtual GpuSignals & getSignals() { return mSignals; }
        virtual void              getBackBufferContent( BackBufferContent & );
        virtual void              enableParameterCheck( bool enable ) { mParamCheckEnabled = enable; }
        virtual void              setUserData( const Guid & id, const void * data, size_t length );
        virtual const void      * getUserData( const Guid & id, size_t * length ) const;
        virtual bool              hasUserData( const Guid & id ) const;

    public:

        bool                      paramCheckEnabled() const { return mParamCheckEnabled; }

    private:

        void miscClear()
        {
            mParamCheckEnabled = GN_BUILD_DEBUG;
            mUserData.Clear();
        }

    private:

        typedef DynaArray<UInt8> UserData;

        typedef HashMap<Guid,UserData,Guid::Hash> UserDataMap;

        GpuSignals  mSignals;
        bool        mParamCheckEnabled;
        UserDataMap mUserData;

        //@}
    };

    ///
    /// render targets description
    ///
    struct RenderTargetDesc
    {
        /// color render targets
        StackArray<RenderTargetTexture, GpuContext::MAX_COLOR_RENDER_TARGETS> colortargets;

        /// depth stencil render target
        RenderTargetTexture                                                   depthstencil;

        /// check for invalid description.
        bool Valid() const
        {
            for( size_t i = 0; i < colortargets.Size(); ++i )
            {
                if( !colortargets[i].texture )
                {
                    GN_ERROR(GN::GetLogger("GN.gfx"))(
                        "NULL color render targets in render target array is not allowed." );
                    return false;
                }
            }

            return true;
        }

        /// equality check
        bool operator==( const RenderTargetDesc & rhs ) const
        {
            if( colortargets.Size() != rhs.colortargets.Size() ) return false;
            for( size_t i = 0; i < colortargets.Size(); ++i )
            {
                if( colortargets[i] != rhs.colortargets[i] ) return false;
            }
            return depthstencil == rhs.depthstencil;
        }

        /// equality check
        bool operator!=( const RenderTargetDesc & rhs ) const
        {
            return !operator==( rhs );
        }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GPUCOMMON_BASICGPU_H__
