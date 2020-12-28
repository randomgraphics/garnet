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
        bool init( const VertexBinding &, const OGLGpuProgram * program );
        void quit();
    private:
        void clear() { mAttribBindings.clear(); mVAO = 0; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// Bind the format to device
        ///
        bool bindStates() const {
            GN_GUARD_SLOW;
            if (mVAO) {
                glBindVertexArray(mVAO);
                return true;
            } else {
                return false;
            }
            GN_UNGUARD_SLOW;
        }


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
            uint16            offset;
            uint8             stream; ///< vertex stream index
            uint8             index;  ///< texture coordinate stage index or vertex attribute index
            GLuint            format;
            GLuint            components;
            GLboolean         normalization;
        };

        VertexBinding                       mFormat;
        DynaArray<AttribBindingInfo>        mAttribBindings;
        GLuint                              mVAO;

        // ********************************
        // private functions
        // ********************************
    private:

        // setup state binding
        bool setupStateBindings( const OGLGpuProgram * gpuProgram );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXOGL_OGLVTXFMT_H__
