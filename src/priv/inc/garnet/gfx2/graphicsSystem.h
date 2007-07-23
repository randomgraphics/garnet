#ifndef __GN_GFX2_GRAPHICSSYSTEM_H__
#define __GN_GFX2_GRAPHICSSYSTEM_H__
// *****************************************************************************
/// \file
/// \brief   interface of experimental kernel based graphics system
/// \author  chenli@@FAREAST (2007.6.11)
// *****************************************************************************

#include <set>

namespace GN { namespace gfx
{
    // *************************************************************************
    // surface that stores device data
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
        /// default contructor
        ///
        SurfaceAttributeSemantic() {}

        ///
        /// contruct from string
        ///
        SurfaceAttributeSemantic( const char * str )
        {
            set( str );
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
    /// surface layout template that can be used to match one or mutiple surface layouts.
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
    /// describe a view to surfaces
    ///
    struct SurfaceView
    {
        Surface * surf;       ///< surface pointer
        UInt32    firstLevel; ///< first mipmap level. 0 means the most detailed level.
        UInt32    numLevels;  ///< set 0 for all levels staring from firstLevel.
        UInt32    firstFace;  ///< first face index, starting from 0
        UInt32    numFaces;   ///< set to 0 for all faces starting from firstFace.

        ///
        /// ctor
        ///
        SurfaceView() : surf(0) {}

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

    // *************************************************************************
    // stream source that transters host data to kernel
    // *************************************************************************

    ///
    /// stream source descriptor
    ///
    struct StreamSourceDesc
    {
        //@{
        StrA                 name;     ///< stream name
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
    // Kernel parameter that controls behavior of the kernel
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
    /// kernel parameter value descriptor
    ///
    struct KernelParameterDesc
    {
        StrA                name;  ///< parameter name
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
        //@}
    };

    struct Kernel;

    ///
    /// kernel parameter set
    ///
    struct KernelParameterSet : public NoCopy
    {
        //@{
        inline  Kernel          & getKernel() const { return mKernel; }

        virtual KernelParameter * get( size_t index ) = 0;
        inline  KernelParameter * get( const StrA & name );

        inline  void              sets( size_t index, const char * value );
        inline  void              seti( size_t index, int value );
        inline  void              setu( size_t index, unsigned int value );
        inline  void              setf( size_t index, float value );
        inline  void              setv( size_t index, const Vector4f & value );
        inline  void              setm( size_t index, const Matrix44f & value );
        inline  void              unset( size_t index );

        inline  void              sets( const StrA & name, const char * value );
        inline  void              seti( const StrA & name, int value );
        inline  void              setu( const StrA & name, unsigned int value );
        inline  void              setf( const StrA & name, float value );
        inline  void              setv( const StrA & name, const Vector4f & value );
        inline  void              setm( const StrA & name, const Matrix44f & value );
        inline  void              unset( const StrA & name );
        //@}

    protected:

        ///
        /// ctor
        ///
        KernelParameterSet( Kernel & e ) : mKernel( e ) {}

    private:

        Kernel & mKernel;
    };

    // *************************************************************************
    // Kernel Port that defines surface binding slot of the kernel
    // *************************************************************************

    ///
    /// kernel port descriptor.
    ///
    struct KernelPortDesc
    {
        //@{
        StrA                  name;       ///< port name
        SurfaceLayoutTemplate layout;     ///< surface layout that the port accepts.
        unsigned int          input  : 1; ///< non zero for input port
        unsigned int          output : 1; ///< non zero for output port
        //@}
    };

    ///
    /// define surface binding to kernel ports
    ///
    struct KernelPortBindingDesc
    {
        //@{
        std::map<StrA,SurfaceView> bindings; ///< bindings indexed by port name.
        //@}
    };

    ///
    /// kernel port binding handle
    ///
    typedef UIntPtr KernelPortBinding;

    // *************************************************************************
    // Kernel: main kernel interface
    // *************************************************************************

    ///
    /// kernel interface
    ///
    struct Kernel : public NoCopy
    {
        ///
        /// get kernel name
        ///
        virtual const char                * getName() const = 0;

        ///
        /// do rendering, with user defined parameter set and binding.
        ///
        /// Note that some kernels accepts '0' as valid binding
        ///
        virtual void                        render( const KernelParameterSet &, KernelPortBinding ) = 0;

        /// \name stream management
        //@{

        virtual size_t                      getNumStreams() const = 0;
        virtual size_t                      getStreamIndex( const StrA & name ) const = 0; ///< return -1, if name is invalid.
        inline  StrA                        getStreamName( size_t index ) const; ///< return empty string, if index is invalid.
        virtual StreamSource              * getStream( size_t index ) const = 0;
        inline  StreamSource              * getStream( const StrA & name ) const;

        //@}

        /// \name parameter management
        //@{

        virtual size_t                      getNumParameters() const = 0;
        virtual size_t                      getParameterIndex( const StrA & name ) const = 0; ///< return -1, if name is invalid.
        inline  StrA                        getParameterName( size_t index ) const; ///< return empty string, if index is invalid.
        virtual const KernelParameterDesc * getParameterDesc( size_t index ) const = 0;
        inline  const KernelParameterDesc * getParameterDesc( const StrA & name ) const;
        virtual KernelParameterSet        * createParameterSet() = 0;

        //@}

        //// \name port & binding management
        //@{

        virtual size_t                      getNumPorts() const = 0;
        virtual size_t                      getPortIndex( const StrA & name ) const = 0; ///< return -1, if name is invalid.
        inline  StrA                        getPortName( size_t index ) const; ///< return empty string, if index is invalid.
        virtual const KernelPortDesc      * getPortDesc( size_t index ) const = 0;
        inline  const KernelPortDesc      * getPortDesc( const StrA & name ) const;
        virtual bool                        compatible( const Surface * surf, const StrA & port ) const = 0;
        virtual KernelPortBinding           createPortBinding( const KernelPortBindingDesc & ) = 0;
        virtual void                        deletePortBinding( KernelPortBinding ) = 0;

        //@}
    };

    class GraphicsSystem;

    ///
    /// kernel factory (creator)
    ///
    typedef Kernel * (*KernelFactory)( GraphicsSystem & );

    // *************************************************************************
    // Graphics system
    // *************************************************************************

    ///
    /// surface creation hints
    ///
    typedef std::map<StrA,StrA> SurfaceCreationHints;

    ///
    /// surface creation parameters
    ///
    struct SurfaceCreationParameter
    {
        ///
        /// binding surface to specific port of specific kernel
        ///
        struct SurfaceBindingParameter
        {
            StrA kernel; ///< kernel name
            StrA port;   ///< port name
        };

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

        ///
        /// add new binding
        ///
        void bindTo( const StrA & kernel, const StrA & port )
        {
            SurfaceBindingParameter b;
            b.kernel = kernel;
            b.port   = port;
            bindings.append( b );
        }
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

        /// \name kernel management
        //@{

        virtual void     registerKernelFactory( const StrA & kernelName, KernelFactory factory, int quality = 100 ) = 0;
        virtual Kernel * getKernel( const StrA & name ) = 0;
        virtual void     deleteAllKernels() = 0;

        template<typename T>
        T * getKernel()
        {
            Kernel * k = getKernel( T::KERNEL_NAME() );
            if( 0 == k ) return 0;
            return safeCastPtr<T>(k);
        }

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

    // *************************************************************************
    // inline functions
    // *************************************************************************

    //inline void KernelParameterSet::sets( size_t index, const char * value );
    //inline void KernelParameterSet::seti( size_t index, int value );
    inline void KernelParameterSet::setu( size_t index, unsigned int value ) { get(index)->seti( 0, 1, (const int*)&value ); }
    //inline void KernelParameterSet::setf( size_t index, float value );
    inline void KernelParameterSet::setv( size_t index, const Vector4f & value ) { get(index)->setf( 0, 4, value ); }
    //inline void KernelParameterSet::setm( size_t index, const Matrix44f & value );
    //inline void KernelParameterSet::unset( size_t index );

    //inline void KernelParameterSet::sets( const StrA & name, const char * value );
    //inline void KernelParameterSet::seti( const StrA & name, int value );
    inline void KernelParameterSet::setu( const StrA & name, unsigned int value ) { setu( getKernel().getParameterIndex(name), value ); }
    //inline void KernelParameterSet::setf( const StrA & name, float value );
    inline void KernelParameterSet::setv( const StrA & name, const Vector4f & value ) { setv( getKernel().getParameterIndex(name), value ); }
    //inline void KernelParameterSet::setm( const StrA & name, const Matrix44f & value );
    //inline void KernelParameterSet::unset( const StrA & name );

    //
    //
    // -----------------------------------------------------------------------------
    inline StreamSource *  Kernel::getStream( const StrA & name ) const
    {
        size_t index =  getStreamIndex( name );
        if( (size_t)-1 == index ) return 0;
        return getStream( index );
    }

    //
    //
    // -----------------------------------------------------------------------------
    const KernelParameterDesc * Kernel::getParameterDesc( const StrA & name ) const
    {
        size_t index =  getParameterIndex( name );
        if( (size_t)-1 == index ) return 0;
        return getParameterDesc( index );
    }

    //
    //
    // -----------------------------------------------------------------------------
    const KernelPortDesc * Kernel::getPortDesc( const StrA & name ) const
    {
        size_t index = getPortIndex( name );
        if( (size_t)-1 == index ) return 0;
        return getPortDesc( index );
    }
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_GRAPHICSSYSTEM_H__
