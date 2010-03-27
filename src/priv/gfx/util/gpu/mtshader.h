#ifndef __GN_GFX_UTIL_GPU_MTSHADER_H__
#define __GN_GFX_UTIL_GPU_MTSHADER_H__
// *****************************************************************************
/// \file
/// \brief   multi-thread shader wrappers
/// \author  chenli@@REDMOND (2009.1.3)
// *****************************************************************************

#include "mtgpu.h"

namespace GN { namespace gfx
{
    ///
    /// multithread GPU uniform
    ///
    class MultiThreadUniform : public Uniform, public StdClass
    {
        GN_DECLARE_STDCLASS( MultiThreadUniform, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        MultiThreadUniform( MultiThreadGpu & r ) : mGpu(r) { Clear(); }
        virtual ~MultiThreadUniform() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( Uniform * );
        void Quit();
    private:
        void Clear() { mUniform = NULL; mFrontEndData = NULL; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        Uniform * getRealUniform() const { return mUniform; }

        // ********************************
        // from GpuProgram
        // ********************************
    public:

        virtual size_t Size() const { return mSize; }
        virtual const void * GetValue() const { return mFrontEndData; }
        virtual void Update( size_t offset, size_t length, const void * data );

        // ********************************
        // private variables
        // ********************************
    private:

        MultiThreadGpu & mGpu;
        Uniform             * mUniform;
        size_t                mSize;
        UInt8               * mFrontEndData;

        // ********************************
        // private functions
        // ********************************
    private:
    };

    ///
    /// multithread GPU program wrapper
    ///
    class MultiThreadGpuProgram : public GpuProgram, public StdClass
    {
        GN_DECLARE_STDCLASS( MultiThreadGpuProgram, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        MultiThreadGpuProgram( MultiThreadGpu & r ) : mGpu(r) { Clear(); }
        virtual ~MultiThreadGpuProgram() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( GpuProgram * );
        void Quit();
    private:
        void Clear() { mGpuProgram = NULL; }
        //@}

        // ********************************
        // public method
        // ********************************
    public:

        GpuProgram * getRealGpuProgram() const { return mGpuProgram; }

        // ********************************
        // from GpuProgram
        // ********************************
    public:

        virtual const GpuProgramParameterDesc & getParameterDesc() const { return *mParamDesc; }

        // ********************************
        // private variables
        // ********************************
    private:

        MultiThreadGpu           & mGpu;
        GpuProgram                    * mGpuProgram;
        size_t                          mNumParams;
        const GpuProgramParameterDesc * mParamDesc;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_UTIL_GPU_MTSHADER_H__
