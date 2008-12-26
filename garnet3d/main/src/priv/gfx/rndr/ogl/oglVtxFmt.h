#ifndef __GN_GFXOGL_OGLVTXFMT_H__
#define __GN_GFXOGL_OGLVTXFMT_H__
// *****************************************************************************
/// \file
/// \brief   OGL vertex binding class
/// \author  chenlee (2005.11.21)
// *****************************************************************************

#include "oglResource.h"

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
        OGLVtxFmt( OGLRenderer & r ) : OGLResource(r) { clear(); }
        virtual ~OGLVtxFmt() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const VertexFormat & );
        void quit();
    private:
        void clear() { mStreamBindings.clear(); mStateBindings.clear(); }
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
        /// Get vertex format descriptor
        ///
        size_t getNumStreams() const { return mStreamBindings.size(); }

        ///
        /// Bind the format to device
        ///
        void bind() const;

        ///
        /// Bind the buffer to device
        ///
        void bindBuffer( size_t index, const UInt8 * buf, size_t stride ) const;

        // ********************************
        // private variables
        // ********************************
    private:

        struct AttribBindingInfo
        {
            const OGLVtxFmt * self;
            size_t            offset;
            size_t            index; ///< texture coordinate stage index or vertex attribute index
            GLuint            format;
            GLuint            components;
            GLboolean         normalization;
        };

        typedef void (*FP_setOglVertexBuffer)( const AttribBindingInfo &, const UInt8 * buf, size_t stride );

        struct AttribBinding
        {
            AttribBindingInfo     info;
            FP_setOglVertexBuffer func;
            void bind( const UInt8 * buf, size_t stride ) const
            {
                GN_ASSERT( func );
                func( info, buf, stride );
            }
        };

        typedef StackArray<AttribBinding,VertexFormat::MAX_VERTEX_ELEMENTS> StreamBinding;

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

        VertexFormat                                                   mFormat;
        StackArray<StreamBinding, RendererContext::MAX_VERTEX_BUFFERS> mStreamBindings;
        StackArray<StateBinding , VertexFormat::MAX_VERTEX_ELEMENTS>   mStateBindings;

        // ********************************
        // private functions
        // ********************************
    private:

        // state binding utils
        bool setupStateBindings();

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
