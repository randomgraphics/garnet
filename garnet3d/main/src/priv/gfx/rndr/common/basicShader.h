#ifndef __GN_GFX_BASICSHADER_H__
#define __GN_GFX_BASICSHADER_H__
// *****************************************************************************
/// \file
/// \brief   Basic shader classes and utilities
/// \author  chenli@@REDMOND (2009.4.2)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// Self-contained GPU program description: all string pointers point to internal data buffer
    ///
    class SelfContainedGpuProgramDesc : public GN::gfx::CompiledGpuProgram
    {
        DynaArray<UInt8> mBuffer;

        static inline bool
        sCheckShaderCode(
            const char *       type, ///< shader type
            const ShaderCode & sc,
            const char *       begin,
            const char *       end )
        {
            if( 0 != sc.code && ( sc.code < begin || sc.code >= end ) )
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common");
                GN_ERROR(sLogger)( "invalid %s shader code pointer.", type );
                return false;
            }

            if( 0 != sc.entry && ( sc.entry < begin || sc.entry >= end ) )
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common");
                GN_ERROR(sLogger)( "invalid %s shader entry pointer.", type );
                return false;
            }

            return true;
        }

    public:

        /// initialize from shader description
        bool init( const GpuProgramDesc & desc )
        {
            // calculate buffer size
            size_t headerLen  = sizeof(desc);
            size_t vsCodeLen  = desc.vs.code ? ( strlen(desc.vs.code) + 1 ) : 0;
            size_t vsEntryLen = desc.vs.entry ? ( strlen(desc.vs.entry) + 1 ) : 0;
            size_t gsCodeLen  = desc.gs.code ? ( strlen(desc.gs.code) + 1 ) : 0;
            size_t gsEntryLen = desc.gs.entry ? ( strlen(desc.gs.entry) + 1 ) : 0;
            size_t psCodeLen  = desc.ps.code ? ( strlen(desc.ps.code) + 1 ) : 0;
            size_t psEntryLen = desc.ps.entry ? ( strlen(desc.ps.entry) + 1 ) : 0;
            size_t length     = headerLen +
                                vsCodeLen + vsEntryLen +
                                gsCodeLen + gsEntryLen +
                                psCodeLen + psEntryLen;

            // allocate buffer
            mBuffer.resize( length );
            GpuProgramDesc & copy = *(GpuProgramDesc*)mBuffer.cptr();
            UInt8 * start = mBuffer.cptr();
            UInt8 * ptr = start;

            // copy header
            memcpy( ptr, &desc, sizeof(desc) );
            ptr += sizeof(desc);

#define COPY_CODE( X ) \
        memcpy( ptr, desc.X.code, X##CodeLen ); \
        copy.X.code = ( X##CodeLen > 0 ) ? (const char*)( ptr - start ) : 0; \
        ptr += X##CodeLen;

#define COPY_ENTRY( X ) \
        memcpy( ptr, desc.X.entry, X##EntryLen ); \
        copy.X.entry = ( X##EntryLen > 0 ) ? (const char*)( ptr - start ) : 0; \
        ptr += X##EntryLen;

            // copy codes and entries
            COPY_CODE( vs ); COPY_ENTRY( vs );
            COPY_CODE( gs ); COPY_ENTRY( gs );
            COPY_CODE( ps ); COPY_ENTRY( ps );

            // done
            GN_ASSERT( ((size_t)( ptr - start )) == length );
            return true;
        }

        /// initialize from raw data buffer
        bool init( const void * data, size_t length )
        {
            // copy input buffer
            mBuffer.resize( length );
            memcpy( mBuffer.cptr(), data, length );

            const char     * start = (const char *)mBuffer.cptr();
            const char     * end   = start + length;
            GpuProgramDesc & desc  = *(GpuProgramDesc*)start;

            // patch all offsets to pointers
            if( 0 != desc.vs.code ) desc.vs.code = start + (size_t)desc.vs.code;
            if( 0 != desc.vs.entry ) desc.vs.entry = start + (size_t)desc.vs.entry;

            if( 0 != desc.gs.code ) desc.gs.code = start + (size_t)desc.gs.code;
            if( 0 != desc.gs.entry ) desc.gs.entry = start + (size_t)desc.gs.entry;

            if( 0 != desc.ps.code ) desc.ps.code = start + (size_t)desc.ps.code;
            if( 0 != desc.ps.entry ) desc.ps.entry = start + (size_t)desc.ps.entry;

            // check GPU program language
            if( desc.lang >= NUM_GPU_PROGRAM_LANGUAGES )
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common");
                GN_ERROR(sLogger)( "invalid GPU program language: %d", desc.lang );
                return false;
            }

            // check data integrity
            if( !sCheckShaderCode( "vertex", desc.vs, start, end ) ||
                !sCheckShaderCode( "geometry", desc.gs, start, end ) ||
                !sCheckShaderCode( "pixel", desc.ps, start, end ) )
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common");
                GN_ERROR(sLogger)( "Invalid shader binary." );
                return false;
            }

            return true;
        }

        const GpuProgramDesc & desc() const { return *(const GpuProgramDesc*)mBuffer.cptr(); }

        virtual const void   * data() const { return mBuffer.cptr(); }
        virtual size_t         size() const { return mBuffer.size(); }
    };

    ///
    /// API agonize uniform (just a memory buffer with time stamp)
    ///
    class SysMemUniform : public Uniform
    {
        const size_t mSize;
        void       * mData;
        SInt32       mTimeStamp;

    public:

        /// ctor
        SysMemUniform( size_t sz )
            : mSize(0==sz?1:sz)
            , mData( heapAlloc(mSize) )
            , mTimeStamp(0)
        {
        }

        /// dtor
        ~SysMemUniform() { heapFree(mData); }

        /// get parameter size
        virtual size_t size() const { return mSize; }

        /// get current parameter value
        virtual const void * getval() const { return mData; }

        /// update parameter value
        virtual void update( size_t offset, size_t length, const void * data )
        {
            if( offset >= mSize || (offset+length) > mSize )
            {
                GN_ERROR(getLogger("GN.gfx.Uniform"))( "Out of range!" );
                return;
            }
            if( NULL == data )
            {
                GN_ERROR(getLogger("GN.gfx.Uniform"))( "Null pointer!" );
                return;
            }
            memcpy( (UInt8*)mData + offset, data, length );
            ++mTimeStamp;
        }

        /// update parameter value
        template<typename T>
        void update( const T & t ) { set( 0, sizeof(t), &t ); }

        /// get current update time stamp
        SInt32 getTimeStamp() const { return mTimeStamp; }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_BASICSHADER_H__
