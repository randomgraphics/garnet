#ifndef __GN_GFX2_GNGFX2_H__
#define __GN_GFX2_GNGFX2_H__
// *****************************************************************************
/// \file
/// \brief   experimental kernel based GFX interface
/// \author  chenli@@FAREAST (2007.6.11)
// *****************************************************************************

#include "GNgfx.h"
#include <set>

namespace GN { namespace gfx
{
    // *************************************************************************
    // surface
    // *************************************************************************

    enum
    {
        MAX_SURFACE_ELEMENT_ATTRIBUTES = 64,  ///< max attributes in single surface element
        MAX_SUB_SURFACES               = 256, ///< max subsurfaces in single surface
    };

    ///
    /// surface access flags
    ///
    enum SurfaceAccessFlag
    {
        //@{
        SURFACE_ACCESS_HOST_READ    = 0x1, ///< can be readen by host application.
        SURFACE_ACCESS_HOST_WRITE   = 0x2, ///< can be modified by host application.
        SURFACE_ACCESS_DEVICE_READ  = 0x4, ///< can bind to input port of a kernel
        SURFACE_ACCESS_DEVICE_WRITE = 0x8, ///< can bind to output port of a kernel
        //@}
    };

    ///
    /// surface dimension
    ///
    enum SurfaceDimension
    {
        //@{
        SURFACE_DIMENSION_1D,
        SURFACE_DIMENSION_2D,
        SURFACE_DIMENSION_3D,
        SURFACE_DIMENSION_COUNT, ///< count of valid dimensions.
        //@}
    };

    ///
    /// convert surface dimension tag to string
    ///
    inline const char * surfaceDimension2String( int dim )
    {
        static const char * sTable[] = { "1D", "2D", "3D" };
        GN_CASSERT( GN_ARRAY_COUNT(sTable) == SURFACE_DIMENSION_COUNT );
        if( 0 <= dim && dim < SURFACE_DIMENSION_COUNT ) return sTable[dim];
        else return "INVALID_SURFACE_DIMENSION";
    }

    ///
    /// surface attribute semantic (8 characters at most)
    ///
    union SurfaceAttributeSemantic
    {
        UInt64 u64;   ///< as 64-bit integer
        char   c8[8]; ///< as 8 characters

        ///
        /// equality check
        ///
        bool operator==( const SurfaceAttributeSemantic & rhs ) const
        {
            return u64 == rhs.u64;
        }

        ///
        /// set semantic value
        ///
        void set(const char * str )
        {
            u64 = 0;
            if( str )
            {
                int i = 0;
                while( *str && i < 8 )
                {
                    c8[i] = *str;
                    ++str;
                    ++i;
                }
                if( *str )
                {
                    GN_WARN(getLogger("GN.gfx.SurfaceAttributeSemantic"))( "semantic string is too long!" );
                }
            }
        }

        ///
        /// convert to string
        ///
        const char * str() const
        {
            static char s[9];
            s[0] = c8[0];
            s[1] = c8[1];
            s[2] = c8[2];
            s[3] = c8[3];
            s[4] = c8[4];
            s[5] = c8[5];
            s[6] = c8[6];
            s[7] = c8[7];
            s[8] = 0;
            return s;
        }

        ///
        /// make surface semantic from string
        ///
        static inline SurfaceAttributeSemantic sMake( const char * s )
        {
            SurfaceAttributeSemantic semantic;
            semantic.set( s );
            return semantic;
        }
    };

    ///
    /// syrface element attribute. This is the minimal unit of a surface.
    ///
    struct SurfaceAttribute
    {
        SurfaceAttributeSemantic semantic; ///< 8-character sementic. (must be unique in single surfel)
        UInt32                   offset;   ///< offset in element.
        ClrFmt                   format;   ///< attribute format. (FMT_XXX).

        ///
        /// set values in attribute descriptor
        ///
        void set( SurfaceAttributeSemantic s, UInt32 o, ClrFmt f )
        {
            semantic = s;
            offset   = o;
            format   = f;
        }
    };
    GN_CASSERT( sizeof(SurfaceAttribute) == 16 );

    ///
    /// Surface element format
    ///
    struct SurfaceElementFormat
    {
        SurfaceAttribute attribs[MAX_SURFACE_ELEMENT_ATTRIBUTES]; ///< surfel attribute list
        UInt32           count;                                   ///< surfel attribute count
        UInt32           stride;                                  ///< surfel stride in bytes
    };

    ///
    /// describe sub surface data orgnization
    ///
    struct SubSurfaceLayout
    {
        // all values are in unit of element
        //@{
        UInt32 width;        ///< sub surface width in element
        UInt32 height;       ///< sub surface height in element (must be 1 for 1D surface)
        UInt32 depth;        ///< sub surface depth in element  (must be 1 for 2D surface)
        UInt32 rowBytes;     ///< bytes of one row of element (for compressed texture, it is bytes of one texel row, not one block row)
        UInt32 sliceBytes;   ///< slice pitch in bytes
        //@}
    };

    ///
    /// describe surface data orgnization
    ///
    struct SurfaceLayout
    {
        SurfaceDimension     dim;     ///< 1D, 2D, 3D
        UInt32               levels;  ///< LOD levels
        UInt32               faces;   ///< number of faces
        SubSurfaceLayout     basemap; ///< properties of base map
        SurfaceElementFormat format;  ///< element format descriptor
    };

    ///
    /// surface descriptor
    ///
    struct SurfaceDesc
    {
        SurfaceLayout layout; ///< surface data layout
        UInt32        access; ///< surface access flags, combination of SurfaceAccessFlag.
    };

    ///
    /// unstructured/opaque surface data in native device format, which is used to fast surface save and load.
    ///
    struct NativeSurfaceData
    {
        ///
        /// surface data chunk
        ///
        struct Chunk
        {
            size_t bytes; ///< chunk bytes
            void * data;  ///< chunk data, may points to external data, or somewhere in DeviceSurfaceData::data
        };

        UInt32           numChunks;                ///< chunk count
        Chunk            chunks[MAX_SUB_SURFACES]; ///< chunks
        DynaArray<UInt8> data;                     ///< raw data
    };

    ///
    /// calculate sub surface index
    ///
    inline size_t calcSubSurfaceIndex( size_t face, size_t level, size_t levels )
    {
        return face * levels + level;
    }

    ///
    /// Surface interface: store device dependant data.
    ///
    struct Surface : public NoCopy
    {
        ///
        /// get surface descriptor
        ///
        virtual const SurfaceDesc & getDesc() const = 0;

        ///
        /// get sub surface information.
        ///
        /// Note that sub surface are index as 2D array: [face,level].
        /// That is:
        ///   subsurface index = face * levels + level.
        ///
        virtual const SubSurfaceLayout * getSubSurfaceLayout( size_t subsurface ) const = 0;

        ///
        /// copy data to surface. Surface must have HOST_WRITE flag.
        ///
        virtual void download(
            size_t                 subsurface,
            const Box<size_t>    * area,
            const void           * source,
            size_t                 srcRowBytes,
            size_t                 srcSliceBytes ) = 0;

        ///
        /// transfer data from device to host. Surface must have HOST_READ flag.
        ///
        virtual void upload(
            size_t              subsurface,
            const Box<size_t> * area,
            void              * destination,
            size_t              destRowBytes,
            size_t              destSliceBytes ) = 0;

        /// \name save and load surface content in device native format.
        ///
        /// Similar as download() and upload(), but operates on whole surface, and normally faster.
        ///
        //@{
        virtual void save( NativeSurfaceData & ) const = 0;
        virtual void load( const NativeSurfaceData & ) = 0;
        //@}
    };

    // *************************************************************************
    // stream source
    // *************************************************************************

    ///
    /// stream source descriptor
    ///
    struct StreamSourceDesc
    {
        //@{
        SurfaceElementFormat format;   ///< element format
        size_t               maxBytes; ///< max stream data size.
        //@}
    };

    ///
    /// stream data source of kernel function
    ///
    struct StreamSource : public NoCopy
    {
        //@{
        virtual const StreamSourceDesc & getDesc() const = 0;

        ///
        /// push data into stream
        ///
        virtual void push( const void * data, size_t bytes ) = 0;

        ///
        /// get free/available bytes in stream
        ///
        virtual size_t freeBytes() const = 0;
        //@}
    };

    // *************************************************************************
    // kernel
    //
    // There's 2 way to transter data between device and host: by surface or by
    // raw host data pointer. A kernel may support either one of them, or both.
    // send/get data to/from kernel by the "unsupported" way has no kernel but
    // generate some error messages.
    //
    // *************************************************************************

    ///
    /// kernel parameter type
    ///
    enum KernelParameterType
    {
        //@{
        KERNEL_PARAMETER_TYPE_BOOL,
        KERNEL_PARAMETER_TYPE_INT,
        KERNEL_PARAMETER_TYPE_FLOAT,
        KERNEL_PARAMETER_TYPE_STRING,
        //@}
    };

    ///
    /// surface attribute template
    ///
    struct SurfaceAttributeTemplate
    {
        SurfaceAttributeSemantic semantic;       ///< attribute semantic
        UInt32                   offset;         ///< -1, means any offset is ok.
        std::set<ClrFmt>         allowedFormats; ///< empty, means any format is ok.
    };

    ///
    /// define template of the surface data layout that can be used to match one or mutiple data layouts.
    ///
    struct SurfaceLayoutTemplate
    {
        //@{

        union
        {
            UInt32 u32; ///< all flags as 32-bits integer.
            struct
            {
                //@{

                // byte 0:
                unsigned int dim        : 1;
                unsigned int levels     : 1;
                unsigned int faces      : 1;
                unsigned int width      : 1;
                unsigned int height     : 1;
                unsigned int depth      : 1;
                unsigned int rowBytes   : 1;
                unsigned int sliceBytes : 1;

                // byte 1
                unsigned int attributes : 1; ///< number of attributes.
                unsigned int stride     : 1; ///< element stride
                unsigned int            : 7;

                // byte 2-3
                unsigned int            : 16;

                //@}
            };
        } flags; ///< template data field flags

        typedef StackArray<SurfaceAttributeTemplate,MAX_SURFACE_ELEMENT_ATTRIBUTES> AttributeArray;

        SurfaceDimension dim;        ///< surface dimension.
        UInt32           levels;     ///< level count
        UInt32           faces;      ///< face count
        SubSurfaceLayout basemap;    ///< basemap properties
        UInt32           attributes; ///< attribute count
        UInt32           stride;     ///< surface element stride
        AttributeArray   requiredAttributes;
        AttributeArray   optionalAttributes;
        //@}

        ///
        /// clear template (allow any layout)
        ///
        void clear()
        {
            flags.u32 = 0;
            requiredAttributes.clear();
            optionalAttributes.clear();
        }

        ///
        /// self check. Make sure itself a valid template.
        ///
        bool check() const;

        ///
        /// check whether a layout is compatible with the template
        ///
        bool compatible( const SurfaceLayout & ) const;

        ///
        /// apply template to a layout.
        ///
        void apply( SurfaceLayout & ) const;

        ///
        /// merge with another template. Return false, if their are conficts.
        ///
        bool mergeWith( const SurfaceLayoutTemplate & );
    };

    ///
    /// kernel port descriptor: describe a input or output port of the kernel
    ///
    struct KernelPortDesc
    {
        //@{
        SurfaceLayoutTemplate layout;
        unsigned int          input  : 1; ///< non zero for input port
        unsigned int          output : 1; ///< non zero for output port
        //@}
    };

    ///
    /// kernel parameter value descriptor
    ///
    struct KernelParameterDesc
    {
        KernelParameterType type;  ///< value type
        size_t              count; ///< array count
    };

    ///
    /// Kernel parameter
    ///
    struct KernelParameter : public NoCopy
    {
        //@{
        virtual const KernelParameterDesc & getDesc() const = 0;
        virtual void                        setb( size_t offset, size_t count, const bool         * values ) = 0;
        virtual void                        seti( size_t offset, size_t count, const int          * values ) = 0;
        virtual void                        setf( size_t offset, size_t count, const float        * values ) = 0;
        virtual void                        sets( size_t offset, size_t count, const char * const * values ) = 0;
        virtual void                        unset() = 0;
        inline  void                        sets( const char * );
        inline  void                        setu( size_t offset, size_t count, const unsigned int * values );
        //@}
    };

    ///
    /// kernel property
    ///
    typedef Variant KernelProperty;

    ///
    /// kernel parameter handle
    ///
    typedef UIntPtr KernelParameterHandle;

    struct Kernel;

    ///
    /// kernel parameter set
    ///
    struct KernelParameterSet : public NoCopy
    {
        //@{
        inline  Kernel          & getKernel() const { return mKernel; }
        virtual KernelParameter * getParameter( KernelParameterHandle ) const = 0;
        inline  KernelParameter * getParameter( const StrA & name ) const;

        inline  void              setParameter( KernelParameterHandle handle, const char * value );
        inline  void              setParameter( KernelParameterHandle handle, int value );
        inline  void              setParameter( KernelParameterHandle handle, float value );
        inline  void              setParameter( KernelParameterHandle handle, const Vector4f & value );
        inline  void              setParameter( KernelParameterHandle handle, const Matrix44f & value );

        template<typename T>
        inline  void              setParameter( const StrA & name, const T & value );

        inline  void              unsetParameter( KernelParameterHandle handle );
        inline  void              unsetParameter( const StrA & name );
        //@}

    protected:

        ///
        /// ctor
        ///
        KernelParameterSet( Kernel & e ) : mKernel( e ) {}

    private:

        Kernel & mKernel;
    };

    ///
    /// describe the target surface that will be bind to specific kernel port.
    ///
    struct KernelBindingTarget
    {
        Surface * surf;       ///< surface pointer
        UInt32    firstLevel; ///< first mipmap level. 0 means the most detailed level.
        UInt32    numLevels;  ///< set 0 for all levels staring from firstLevel.
        UInt32    firstFace;  ///< first face index, starting from 0
        UInt32    numFaces;   ///< set to 0 for all faces starting from firstFace.

        ///
        /// setup port binding
        ///
        void set(
            Surface * surf_,
            UInt32    firstLevel_,
            UInt32    numLevels_,
            UInt32    firstFace_,
            UInt32    numFaces_ )
        {
            surf       = surf_;
            firstLevel = firstLevel_;
            numLevels  = numLevels_;
            firstFace  = firstFace_;
            numFaces   = numFaces_;
        }
    };

    ///
    /// define surface to kernel binding
    ///
    struct KernelBindingDesc
    {
        //@{
        std::map<StrA,KernelBindingTarget> bindings; ///< bindings indexed by port name.
        //@}
    };

    ///
    /// kernel binding handle
    ///
    typedef UIntPtr KernelBinding;

    ///
    /// kernel interface: represents a process kernel function
    ///
    struct Kernel : public NoCopy
    {
        /// \name kernel descriptions
        //@{

        ///
        /// get kernel descriptor
        ///
        virtual const KernelParameterDesc * getParameterDesc( const StrA & name ) const = 0;
        virtual const KernelPortDesc      * getPortDesc( const StrA & name ) const = 0;

        //@}

        /// \name parameter management
        //@{

        virtual KernelParameterHandle getParameterHandle( const StrA & name ) const = 0;
        virtual KernelParameterSet *  createParameterSet() = 0;

        //@}

        //// \name port & binding management
        //@{

        virtual bool          compatible( const Surface * surf, const StrA & port ) = 0;
        virtual KernelBinding createBinding( const KernelBindingDesc & ) = 0;
        virtual void          deleteBinding( KernelBinding ) = 0;

        //@}

        ///
        /// do rendering, with user defined parameter set and binding.
        ///
        /// Note that some kernels may accept '0' as valid binding
        ///
        virtual void render( const KernelParameterSet &, KernelBinding ) = 0;
    };

    // *************************************************************************
    // Graphics system
    // *************************************************************************

    ///
    /// surface creation hints
    ///
    typedef std::map<StrA,StrA> SurfaceCreationHints;

    ///
    /// binding surface to specific port of specific kernel
    ///
    struct SurfaceBindingParameter
    {
        StrA kernel; ///< kernel name
        StrA port;   ///< port name

        ///
        /// default ctor
        ///
        SurfaceBindingParameter() {}

        ///
        /// ctor
        ///
        SurfaceBindingParameter( const StrA & e, const StrA & p ) : kernel( e ), port( p ) {}
    };

    ///
    /// surface creation parameters
    ///
    struct SurfaceCreationParameter
    {
        ///
        /// define required bindings of the resources
        ///
        DynaArray<SurfaceBindingParameter> bindings;

        ///
        /// surface data layout
        ///
        SurfaceLayout layout;

        ///
        /// Force the created surface supports some access flags.
        ///
        /// Normally set to zero to let graphics system to determine the optimal access flags.
        ///
        /// Set to non-zero value may fail the creation process.
        ///
        int forcedAccessFlags;

        ///
        /// creation hints (name and value pairs)
        ///
        SurfaceCreationHints hints;
    };

    class GraphicsSystem;

    ///
    /// kernel factory
    ///
    struct KernelFactory
    {
        int       quality;                       ///< kernel quality
        Kernel * (*creator)( GraphicsSystem & ); ///< kernel creator
    };

    ///
    /// Describe common graphics system properties (platform independent)
    ///
    struct GraphicsSystemDesc
    {
        HandleType display; ///< platform specific display handle.
        HandleType window;  ///< platform specific render window handle.
        UInt32     width;   ///< graphics screen width
        UInt32     height;  ///< graphics screen height
        UInt32     depth;   ///< graphics screen color depth in bits
        UInt32     refrate; ///< graphics screen refresh rate
    };

    ///
    /// Major interface of new graphics system
    ///
    class GraphicsSystem : public NoCopy
    {
    public:

        ///
        /// get graphics descriptor
        ///
        virtual const GraphicsSystemDesc & getDesc() const = 0;

        ///
        /// called per-frame by host application to present rendering result to screen, as well as some per-frame statistics task.
        ///
        virtual void present() = 0;

        /// \name global kernel parameter management
        //@{

        virtual KernelParameterHandle createGlobalKernelParameter( const StrA & name, const KernelParameterDesc & desc ) = 0;
        virtual KernelParameterHandle getGlobalKernelParameterHandle( const StrA & name ) const = 0;
        virtual KernelParameter     * getGlobalKernelParameter( KernelParameterHandle ) const = 0;

        //@}

        /// \name kernel management
        //@{

        virtual void     registerKernel( const StrA & name, const KernelFactory & ) = 0;
        virtual Kernel * getKernel( const StrA & name ) = 0;
        virtual void     deleteKernel( const StrA & name ) = 0;
        virtual void     deleteAllKernels() = 0;

        //@}

        /// \name surface management
        //@{

        virtual Surface * createSurface( const SurfaceCreationParameter & scp ) = 0;

        //@}
    };

    //@{

    ///
    /// graphics system creation parameters
    ///
    struct GraphicsSystemCreationParameter
    {
        //@{
        FOURCC api;     ///< could be: 'OGL', 'DX9', 'DX10', 'XB2'
        UInt32 monitor; ///< monitor index. 0 is the first monitor.
        UInt32 fullscrWidth;
        UInt32 fullscrHeight;
        UInt32 fullscrDepth;
        UInt32 fullscrRefrate;
        UInt32 windowedWidth;
        UInt32 windowedHeight;
        bool   fullscr;
        bool   vsync;
        //@}
    };

    ///
    /// create and destroy graphics system instance
    ///
    class GraphicsSystemCreator
    {
        SharedLib        mDll;
        GraphicsSystem * mInstance;

    public:

        //@{

        GraphicsSystemCreator() : mInstance(0) {}
        ~GraphicsSystemCreator() { destroy(); }

        GraphicsSystem * create( const GraphicsSystemCreationParameter & );
        void             destroy();
        GraphicsSystem * get() const { return mInstance; }

        //@}
    };

    //@}
}}

#include "gfx2/GNgfx2.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_GNGFX2_H__
