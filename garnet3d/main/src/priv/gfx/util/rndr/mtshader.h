#ifndef __GN_GFX_UTIL_RNDR_MTSHADER_H__
#define __GN_GFX_UTIL_RNDR_MTSHADER_H__
// *****************************************************************************
/// \file
/// \brief   multi-thread shader wrappers
/// \author  chenli@@REDMOND (2009.1.3)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    ///
    ///
    class MultiThreadGpuProgram : public GpuProgram, public StdClass
    {
        GN_DECLARE_STDCLASS( MultiThreadGpuProgram, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        MultiThreadGpuProgram()          { clear(); }
        virtual ~MultiThreadGpuProgram() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // from GpuProgram
        // ********************************
    public:

        virtual size_t getNumParameters() const;
        virtual const GpuProgramParameterDesc * getParameters() const;
        virtual void setParameter( size_t index, const void * value, size_t length );

        // ********************************
        // private variables
        // ********************************
    private:

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_UTIL_RNDR_MTSHADER_H__
