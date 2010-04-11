#ifndef __GN_GFXOGL_OGLVTXFMT_H__
#define __GN_GFXOGL_OGLVTXFMT_H__
// *****************************************************************************
/// \file
/// \brief   OGL vertex binding class
/// \author  chenlee (2005.11.21)
// *****************************************************************************

#include "oglResource.h"
#include "oglShader.h"

namespace GN { namespace gfx
{
    ///
    /// OGL vertex binding class
    ///
    class OGLVtxFmt : public OGLResource, public StdClass
    {
         GN_DECLARE_STDCLASS( OGLVtxFmt, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLVtxFmt( OGLGpu & r ) : OGLResource(r) { clear(); }
        virtual ~OGLVtxFmt() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const VertexFormat &, const OGLBasicGpuProgram * program );
        void quit();
    private:
        void clear() { mAttribBindings.clear(); mStateBindings.clear(); mValid = false; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// Get vertex format descriptor
        ///
        const VertexFormat & getFormat() const { return mFormat; }

        ///
        /// Bind the format to device
        ///
        bool bindStates() const;

        ///
        /// Bind the buffer to device
        ///
        bool bindBuffers( const VertexBufferBinding * bindings,
                          size_t                      numbufs,
                          size_t                      startvtx ) const;

        ///
        /// Bind raw memory pointer to device, as vertex buffer 0.
        ///
        bool bindRawMemoryBuffer( const void * data, size_t stride ) const;

        // ********************************
        // private variables
        // ********************************
    private:

        struct AttribBindingInfo
        {
            const OGLVtxFmt * self;
            UInt16            offset;
            UInt8             stream; ///< vertex stream index
            UInt8             index;  ///< texture coordinate stage index or vertex attribute index
            GLuint            format;
            GLuint            components;
            GLboolean         normalization;
        };

        typedef void (*FP_setOglVertexBuffer)( const AttribBindingInfo &, const UInt8 * buf, size_t stride );

        struct AttribBinding
        {
            AttribBindingInfo     info;
            FP_setOglVertexBuffer func;
            void bind( const void * buf, size_t stride ) const
            {
                GN_ASSERT( func );
                func( info, (const UInt8*)buf, stride );
            }
        };

        struct StateBindingInfo
        {
            const OGLVtxFmt * self;
            size_t            texStage;
            GLenum            semantic;
            GLuint            attribute;
        };

        typedef void (*FP_setOglVertexState)( const StateBindingInfo & );

        struct StateBinding
        {
            StateBindingInfo     info;
            FP_setOglVertexState func;
        };

        VertexFormat               mFormat;
        const OGLBasicGpuProgram * mProgram;
        bool                       mValid;
        DynaArray<AttribBinding>   mAttribBindings;
        DynaArray<StateBinding>    mStateBindings;

        // ********************************
        // private functions
        // ********************************
    private:

        // setup state binding
        bool setupStateBindings();
        bool getVertexBindingDesc( OGLVertexBindingDesc & result, const char * bindingName, UInt8 bindingIndex );

        static void sSetVertexPointer( const AttribBindingInfo &, const UInt8 * buf, size_t stride );
        static void sSetNormalPointer( const AttribBindingInfo &, const UInt8 * buf, size_t stride );
        static void sSetColorPointer( const AttribBindingInfo &, const UInt8 * buf, size_t stride );
        static void sSetSecondaryColorPointer( const AttribBindingInfo &, const UInt8 * buf, size_t stride );
        static void sSetFogPointer( const AttribBindingInfo &, const UInt8 * buf, size_t stride );
        static void sSetTexCoordPointer( const AttribBindingInfo &, const UInt8 * buf, size_t stride );
        static void sSetVertexAttributePointer( const AttribBindingInfo &, const UInt8 * buf, size_t stride );

        static void sEnableClientState( const StateBindingInfo & info );
        static void sDisableClientState( const StateBindingInfo & info );
        static void sEnableVAA( const StateBindingInfo & info );
        static void sDisableVAA( const StateBindingInfo & info );
        static void sEnableTexArray( const StateBindingInfo & info );
        static void sDisableTexArray( const StateBindingInfo & info );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXOGL_OGLVTXFMT_H__
