#ifndef __GN_GFX2_GNGFX2_H__
#define __GN_GFX2_GNGFX2_H__
// *****************************************************************************
//! \file    experimental/GNgfx2.h
//! \brief   experimental effect based GFX interface
//! \author  chenli@@FAREAST (2007.6.11)
// *****************************************************************************

namespace GN
{
    ///
    /// namespace for new experimental graphics system
    namespace gfx2
    {
    }
}

namespace GN { namespace gfx2
{
    // *************************************************************************
    // surface
    // *************************************************************************

    enum
    {
        MAX_SURFACE_ELEMENT_ATTRIBUTES = 16,  ///< max attributes in single surface element
        MAX_SUB_SURFACES               = 256, ///< max subsurfaces in single surface
    };

    ///
    /// surface access flags
    ///
    enum SurfaceAccessFlag
    {
        //@{
        HOST_READ    = 0x1, ///< can be readen by host application.
        HOST_WRITE   = 0x2, ///< can be modified by host application.
        DEVICE_READ  = 0x4, ///< can bind to input port of a effect
        DEVICE_WRITE = 0x8, ///< can bind to output port of a effect
        //@}
    };

    ///
    /// surface dimension
    ///
    enum SurfaceDimension
    {
        //@{
        DIM_1D,
        DIM_2D,
        DIM_3D,
        //@}
    };

    ///
    /// syrface element attribute. This is the minimal unit of a surface.
    ///
    union SurfaceAttribute
    {
        UInt16     u64;      ///< attribute as 64bit integer
        struct
        {
            FOURCC semantic; ///< FORCC encoded sementic. (must be unique in single surfel)
            UInt16 offset;   ///< offset in element.
            SInt16 format;   ///< attribute format. (FMT_XXX).
        };

        ///
        /// set values in attribute descriptor
        ///
        void set( FOURCC s, UInt16 o, SInt16 f )
        {
            semantic = s;
            offset   = o;
            format   = f;
        }
    };
    GN_CASSERT( sizeof(SurfaceAttribute) == 8 );

    ///
    /// Surface element (surfel)
    ///
    struct SurfaceElement
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
        UInt32 height;       ///< sub surface height in element
        UInt32 depth;        ///< sub surface depth in element
        UInt32 rowBytes;     ///< row pitch in bytes
        UInt32 sliceBytes;   ///< slice pitch in bytes
        //@}
    };

    ///
    /// describe surface data orgnization
    ///
    struct SurfaceLayout
    {
        SurfaceDimension dim;     ///< 1D, 2D, 3D
        UInt32           levels;  ///< LOD levels
        UInt32           faces;   ///< number of faces
        SubSurfaceLayout basemap; ///< properties of base map
        SurfaceElement   element; ///< element descriptor
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
            const Box<size_t>    & area,
            const void           * source,
            size_t                 srcRowBytes,
            size_t                 srcSliceBytes ) = 0;

        ///
        /// transfer data from device to host. Surface must have HOST_READ flag.
        ///
        virtual void upload(
            size_t              subsurface,
            const Box<size_t> & area,
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
    // effect
    //
    // There's 2 way to transter data between device and host: by surface or by
    // raw host data pointer. A effect may support either one of them, or both.
    // send/get data to/from effect by the "unsupported" way has no effect but
    // generate some error messages.
    //
    // *************************************************************************

    ///
    /// surface attribute template
    ///
    struct SurfaceAttributeTemplate
    {
        FOURCC           semantic;
        UInt16           offset;         ///< -1, means any offset is ok.
        std::set<SInt16> allowedFormats; ///< empty, means any format is ok.
    };

    ///
    /// define template of the surface data layout that can be used to match one or mutiple data layouts.
    ///
    struct SurfaceLayoutTemplate
    {
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
                unsigned int stride     : 1; ///< element stride
                unsigned int            : 7;

                // byte 2-3
                unsigned int            : 16;

                //@}
            };
        } flags; ///< template data field flags

        typedef StackArray<SurfaceAttributeTemplate,MAX_SURFACE_ELEMENT_ATTRIBUTES> AttributeArray;

        SurfaceDimension dim;
        UInt16           levels;
        UInt16           faces;
        SubSurfaceLayout basemap;
        AttributeArray   requiredAttributes;
        AttributeArray   optionalAttributes;
        //@}

        ///
        /// self check. Make sure itself a valid template.
        ///
        bool check() const; 

        ///
        /// check whether a layout matches the template
        ///
        bool match( const SurfaceLayout & ) const;

        ///
        /// merge 2 templates. Return false, if they are confict with each other.
        ///
        static bool sMerge(
           SurfaceLayoutTemplate & result,
           const SurfaceLayoutTemplate & t1,
           const SurfaceLayoutTemplate & t2 );
    };

    ///
    /// effect port descriptor: describe a input or output port of the effect
    ///
    struct EffectPortDesc
    {
        //@{
        StrA                  sementic;
        SurfaceLayoutTemplate layout;
        //@}
    };

    ///
    /// effect descriptor: describe public interface of the effect
    ///
    struct EffectDesc
    {
        ///
        /// input output ports. Note that port semantic must be unique.
        ///
        DynaArray<EffectPortDesc> ports;
    };

    ///
    /// describe binding a surface to specific effect port.
    struct EffectPortBinding
    {
        StrA      port;       ///< effect port name
        Surface * surf;       ///< surface pointer
        StrA      semantic;   ///< semantic of the surface attribute that will be binded.
        //@{
        UInt32    firstLevel;
        UInt32    numLevels;
        UInt32    firstFace;
        UInt32    numFaces;
        //@}
    };

    ///
    /// define surface to effect binding
    ///
    struct EffectBindingDesc
    {
        DynaArray<EffectPortBinding> bindings;
    };

    ///
    /// effect binding handle
    ///
    typedef UIntPtr EffectBinding;

    ///
    /// Effect parameter value
    ///
    struct EffectParameterValue
    {
        int        type; ///< value type. Could be: string, float, int, vector4, matrix44, raw
        union
        {
            float  float1;
            int    int1;
            float  float4[4];
            float  float4x4[4][4];
            struct
            {
                void * ptr;
                size_t bytes;
            } raw;
        };
    };

    ///
    /// effect interface: represents a process kernel function
    ///
    struct Effect : public NoCopy
    {
        ///
        /// get effect descriptor
        ///
        virtual const EffectDesc & getDesc() const = 0;

        ///
        /// set private value of a parameter
        ///
        virtual void setParameter( const StrA & name, const EffectParameterValue & value ) = 0;

        ///
        /// clear private parameter value. So next time effect using this parameter,
        /// it'll use global/shared value.
        ///
        virtual void unsetParameter( const StrA & name ) = 0;

        ///
        /// check whether a surface is compatible with the effect
        ///
        virtual bool compatible( Surface * surf, const StrA & port ) = 0;

        ///
        /// create a binding handle
        ///
        virtual EffectBinding createBinding( const EffectBindingDesc & ) = 0;

        ///
        /// delete a binding handle
        ///
        virtual void deleteBinding( EffectBinding ) = 0;

        ///
        /// bind surfaces to effect
        ///
        virtual void bind( EffectBinding ) = 0;

        ///
        /// bind surface to effect
        ///
        inline void bind( const EffectBindingDesc & ebd ) { bind( createBinding( ebd ) ); }
    };

    // *************************************************************************
    // Graphics system
    // *************************************************************************

    ///
    /// surface creation parameters
    ///
    struct SurfaceCreationParameter
    {
        ///
        /// define binding to specific port of specific effect
        ///
        struct EffectBinding
        {
            StrA effect; ///< effect name
            StrA port;   ///< port name
        };

        ///
        /// define required bindings of the resources
        ///
        DynaArray<EffectBinding> binding;

        ///
        /// surface data layout
        ///
        SurfaceLayoutTemplate layout;

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
        std::map<StrA,StrA> hints;
    };

    ///
    /// effect factory
    ///
    struct EffectFactory
    {
        int       quality;                       ///< effect quality 
        Effect * (*creator)( GraphicsSystem & ); ///< effect creator
    };

    ///
    /// Major interface of new graphics system
    ///
    class GraphicsSystem : public NoCopy
    {
    public:

        /// \name global effect parameter management
        //@{

        ///
        /// set global value of a parameter
        ///
        virtual void setGlobalEffectParameter( const StrA & name, const EffectParameterValue & value ) = 0;

        ///
        /// clear global parameter value.
        ///
        virtual void unsetGlobalEffectParameter( const StrA & name ) = 0;

        ///
        /// get value of global effect parameter
        ///
        virtual const EffectParameterValue * getGlobalEffectParameter( const StrA & name ) = 0;

        //@}

        /// \name effect management
        //@{

        virtual void     registerEffect( const Str & name, const EffectFactory & ) = 0;
        virtual Effect * getEffect( const StrA & name ) = 0;
        virtual void     deleteEffect( const StrA & name ) = 0;
        virtual void     deleteAllEffects() = 0;

        //@}

        /// \name surface management
        //@{

        virtual Surface * createSurface( const SurfaceCreationParameter & scp ) = 0;

        //@}
    };

    // *************************************************************************
    // D3D9 effect library (for effect developer)
    // *************************************************************************

    //@{

    ///
    /// D3D9 surface descriptor
    ///
    struct D3D9SurfaceDesc : public SurfaceDesc
    {
        // D3D9 specific fields
    };

    ///
    /// base D3D9 surface
    ///
    class D3D9Surface : public Surface
    {
    };

    ///
    /// D3D9 effect descriptor
    ///
    struct D3D9EffectDesc : public EffectDesc
    {
    };

    ///
    /// base D3D9 effect
    ///
    class D3D9Effect : public Effect
    {
    };

    ///
    /// clear effect on  
    class D3D9ClearEffect : public D3D9Effect
    {
    };

    //@}

    // *************************************************************************
    // D3D9 graphics system
    // *************************************************************************

    //@{

    ///
    /// D3D9 graphics system
    ///
    class D3D9GraphicsSystem : public GraphicsSystem
    {
        virtual Surface * createSurface( const SurfaceCreationParameter & scp )
        {
            // compose D3D9 surface descriptor:
            //  1. merge all surface templates
            //  2. merge other D3D9 effect specific requirements.
            //  3. check hints

            // create D3D9 surface using the descriptor.
        }

        D3D9Surface * createD3D9Surface( const D3D9SurfaceDesc & );
    };

    //@}

    // *************************************************************************
    // Sample code
    // *************************************************************************

    //@{

    GraphicsSystem gs;

    void RenderToCube()
    {
        // effect that do screen clear
        //
        // output ports: color0, depth0
        //
        Effect * clear = gs.getEffect( "Clear" );

        // effect that do present
        //
        // input ports: color0
        //
        Effect * present = gs.getEffect( "Present" );

        // effect that draws cube map on cube mesh
        //
        // input ports: cubemap, cubemesh
        //
        // output ports: color0, depth0
        //
        Effect * cubeOnCube = gs.getEffect( "CubeOnCube" );

        // single textured diffuse lighting to cubemap
        //
        // input ports: diffuseTexture
        //
        // output ports: cubeColor, cubeDepth
        //
        Effect * singleDiffuseToCube = gs.getEffect( "SingleDiffuseToCube" );

        SurfaceCreationParameter scp;

        // allocate color and depth surface
        // TODO: setup scp to bind to port Clear::color0 and Present::color0
        Surface * color0 = gs.createSurface( scp );

        // create cubedepth
        // TODO: setup scp to bind to port SingleDiffuseToCube::cubeDepth and Clear::depth0
        Surface * cubedepth = gs.createSurface( scp );

        // create cubemap
        // TODO: setup scp to bind to port CubeOnCube::cubemap and SingleDiffuseToCube::cubeColor
        Surface * cubemap = gs.createSurface( scp );

        EffectBindingDesc ebd;

        // bind surfaces to effects
        // TODO: setup ebd to bind color0 and depth to clear effect
        clear->bind( ebd );
        // ...

        // do rendering
        clear->setParameter( "Viewport", ... );
        clear->setParameter( "ClearColor", ... );
        clear->setParameter( "ClearDepth", ... );
        clear->setParameter( "ClearStencil", ... );
        clear->render();

        singleDiffuseToCube->setParameter( "MaterialColor", ... );
        singleDiffuseToCube->setParameter( "LightColor", ... );
        singleDiffuseToCube->setParameter( "LightPosition", ... );
        singleDiffuseToCube->setParameter( "ProjViewWorld", ... );
        singleDiffuseToCube->render();

        cubeOnCube->render();

        present->bind( ebd );
    }

    //@}
}}

// *****************************************************************************
//                           End of GNgfx2.h
// *****************************************************************************
#endif // __GN_GFX2_GNGFX2_H__
