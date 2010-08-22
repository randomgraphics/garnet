#include "pch.h"
#include "modelresource.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

// *****************************************************************************
// local classes and functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static StrA sResolveResourcePath( const StrA & basedir, const StrA & path )
{
    if( path.size() > 1 && path[0] == '@' )
    {
        return path;
    }
    else
    {
        return fs::resolvePath( basedir, path );
    }
}

//
//
// -----------------------------------------------------------------------------
static void
sApplyRenderStates(
    GN::gfx::GpuContext::RenderStates               & dst,
    const GN::gfx::GpuContext::RenderStates         & src,
    const EffectResourceDesc::EffectRenderStateDesc & renderstates )
{
    #define MERGE_SINGLE_RENDER_STATE( state ) dst.state = ( renderstates.state.overridden ? renderstates.state.value : src.state )

    MERGE_SINGLE_RENDER_STATE( depthTestEnabled );
    MERGE_SINGLE_RENDER_STATE( depthWriteEnabled );
    MERGE_SINGLE_RENDER_STATE( depthFunc );

    MERGE_SINGLE_RENDER_STATE( stencilEnabled );
    MERGE_SINGLE_RENDER_STATE( stencilPassOp );
    MERGE_SINGLE_RENDER_STATE( stencilFailOp );
    MERGE_SINGLE_RENDER_STATE( stencilZFailOp );

    MERGE_SINGLE_RENDER_STATE( independentAlphaBlending );
    for( size_t i = 0; i < GpuContext::MAX_COLOR_RENDER_TARGETS; ++i )
    {
        MERGE_SINGLE_RENDER_STATE( alphaBlend[i].blendEnabled );
        MERGE_SINGLE_RENDER_STATE( alphaBlend[i].blendSrc );
        MERGE_SINGLE_RENDER_STATE( alphaBlend[i].blendDst );
        MERGE_SINGLE_RENDER_STATE( alphaBlend[i].blendOp );
        MERGE_SINGLE_RENDER_STATE( alphaBlend[i].blendAlphaSrc );
        MERGE_SINGLE_RENDER_STATE( alphaBlend[i].blendAlphaDst );
        MERGE_SINGLE_RENDER_STATE( alphaBlend[i].blendAlphaOp );
    }
    MERGE_SINGLE_RENDER_STATE( blendFactors );

    MERGE_SINGLE_RENDER_STATE( fillMode );
    MERGE_SINGLE_RENDER_STATE( cullMode );
    MERGE_SINGLE_RENDER_STATE( frontFace );
    MERGE_SINGLE_RENDER_STATE( msaaEnabled );

    MERGE_SINGLE_RENDER_STATE( colorWriteMask );
    MERGE_SINGLE_RENDER_STATE( viewport );
    MERGE_SINGLE_RENDER_STATE( scissorRect );

    #undef MERGE_SINGLE_RENDER_STATE
}

//
// get value of integer attribute
// -----------------------------------------------------------------------------
template<typename T>
static bool sGetRequiredIntAttrib( T & result, const XmlElement & node, const char * attribName )
{
    const XmlAttrib * a = node.findAttrib( attribName );
    if( !a || 0 == string2Integer<T>( result, a->value.cptr() ) )
    {
        GN_ERROR(sLogger)( "Integer attribute \"%s\" of element <%s> is either missing or invalid.",
            attribName, node.name.cptr() );
        return false;
    }
    else
    {
        return true;
    }
}

//
// get value of integer attribute
// -----------------------------------------------------------------------------
static void sBinaryEncode( StrA & result, const UInt8 * data, size_t size )
{
    static const char TABLE[] =
    {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    };

    result.clear();
    result.setCaps( size * 2 );

    for( size_t i = 0; i < size; ++i, ++data )
    {
        UInt8 u8 = *data;

        result.append( TABLE[u8>>4] );
        result.append( TABLE[u8&0xF] );
    }
}

//
// get value of integer attribute
// -----------------------------------------------------------------------------
static bool sBinaryDecode( DynaArray<UInt8> & data, const StrA & s )
{
    /*static UInt8 TABLE[] =
    {

        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    };*/

    data.clear();

    if( 0 != (s.size() % 2) )
    {
        GN_ERROR(sLogger)( "Invalid binary string." );
        return false;
    }

    data.resize( s.size() / 2 );

    for( size_t i = 0; i < data.size(); ++i )
    {
        char hi = s[i*2];
        char lo = s[i*2+1];

        UInt8 u8 = 0;

        if( '0' <= hi && hi <= '9' )
        {
            u8 = (hi-'0')<<4;
        }
        else if( 'A' <= hi && hi <= 'F' )
        {
            u8 = (hi-'A'+10)<<4;
        }
        else
        {
            GN_ERROR(sLogger)( "Invalid character in binary string: %d", hi );
            return false;
        }

        if( '0' <= lo && lo <= '9' )
        {
            u8 += (lo-'0');
        }
        else if( 'A' <= lo && lo <= 'F' )
        {
            u8 += (lo-'A'+10);
        }
        else
        {
            GN_ERROR(sLogger)( "Invalid character in binary string: %d", lo );
            return false;
        }

        data[i] = u8;
    }

    return true;
}

// *****************************************************************************
// local classes and functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResourceDesc::clear()
{
    effect.clear();
    textures.clear();
    uniforms.clear();
    mesh.clear();
    subset.clear();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::ModelResourceDesc::loadFromXml( const XmlNode & root, const char * basedir )
{
    clear();

    const XmlElement * rootElement = root.toElement();
    if( !rootElement || rootElement->name != "model" )
    {
        GN_ERROR(sLogger)( "Root node must be a XML element named <model>." );
        return false;
    }

    bool effectFound = false;
    bool meshFound = false;
    bool subsetFound = false;
    for( const XmlNode * n = rootElement->firstc; n != NULL; n = n->nexts )
    {
        const XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "effect" == e->name )
        {
            if( effectFound )
            {
                GN_WARN(sLogger)( "Extra <effect> elements are ignored." );
            }
            else
            {
                const XmlAttrib * a = e->findAttrib( "ref" );
                if( !a )
                {
                    GN_ERROR(sLogger)( "\"ref\" attribute of <effect> element is missing." );
                    return false;
                }
                effect = sResolveResourcePath( basedir, a->value );

                effectFound = true;
            }
        }
        else if( "mesh" == e->name )
        {
            if( meshFound )
            {
                GN_WARN(sLogger)( "Extra <mesh> elements are ignored." );
            }
            else
            {
                const XmlAttrib * a = e->findAttrib( "ref" );
                if( !a )
                {
                    GN_ERROR(sLogger)( "\"ref\" attribute of <mesh> element is missing." );
                    return false;
                }
                mesh = sResolveResourcePath( basedir, a->value );

                meshFound = true;
            }
        }
        else if( "subset" == e->name )
        {
            if( subsetFound )
            {
                GN_WARN(sLogger)( "Redundant <subset> elements are ignored." );
            }
            else
            {
                if( !sGetRequiredIntAttrib( subset.basevtx, *e, "basevtx" ) ||
                    !sGetRequiredIntAttrib( subset.numvtx, *e, "numvtx" ) ||
                    !sGetRequiredIntAttrib( subset.startidx, *e, "startidx" ) ||
                    !sGetRequiredIntAttrib( subset.numidx, *e, "numidx" ) )
                {
                    GN_ERROR(sLogger)( "Invalid <subset> element." );
                    return false;
                }
            }
        }
        else if( "texture" == e->name )
        {
            const XmlAttrib * a = e->findAttrib( "shaderParameter" );
            if( !a )
            {
                GN_ERROR(sLogger)( "\"shaderParameter\" attribute of <texture> element is missing." );
                return false;
            }

            ModelTextureDesc & td = textures[a->value];

            a = e->findAttrib( "ref" );
            if( a )
            {
                td.resourceName = sResolveResourcePath( basedir, a->value );
            }
            else
            {
                GN_TODO( "read texture descriptor" );
            }
        }
        else if( "uniform" == e->name )
        {
            const XmlAttrib * a = e->findAttrib( "shaderParameter" );
            if( !a )
            {
                GN_ERROR(sLogger)( "\"shaderParameter\" attribute of <uniform> element is missing." );
                return false;
            }

            ModelUniformDesc & ud = uniforms[a->value];

            a = e->findAttrib( "size" );
            if( !a )
            {
                GN_ERROR(sLogger)( "\"size\" attribute of <uniform> element is missing." );
                return false;
            }
            if( 0 == string2Integer( ud.size, a->value ) )
            {
                GN_ERROR(sLogger)( "\"size\" attribute of <uniform> element is not a valid integer." );
                return false;
            }

            const XmlElement * binnode = e->findChildElement( "initialValue" );
            if( binnode && !sBinaryDecode( ud.initialValue, binnode->text ) )
            {
                GN_ERROR(sLogger)( "Invalid uniform initial data." );
                return false;
            }
        }
        else
        {
            GN_WARN(sLogger)( "Ignore unrecognized element <%s>.", e->name.cptr() );
        }
    }

    if( !effectFound || !meshFound )
    {
        GN_ERROR(sLogger)( "<effect> and <mesh> element are required." );
        return false;
    }

    // done
    return true;
}

//
//
// -----------------------------------------------------------------------------
XmlElement * GN::gfx::ModelResourceDesc::saveToXml( XmlNode & root, const char * basedir ) const
{
    XmlElement * rootElement = root.toElement();
    if( !rootElement )
    {
        GN_ERROR(sLogger)( "Root node must be a XML element." );
        return NULL;
    }

    if( NULL == basedir )
    {
        GN_ERROR(sLogger)( "NULL basedir pointer." );
        return NULL;
    }

    XmlDocument & doc = rootElement->doc;

    XmlElement * modelNode = doc.createElement(NULL);
    modelNode->name = "model";

    // create effect node
    XmlElement * effectNode = doc.createElement(modelNode);
    effectNode->name = "effect";
    if( effect.empty() )
    {
        GN_ERROR(sLogger)( "Effect name can not be empty." );
        return NULL;
    }
    else
    {
        XmlAttrib * a = doc.createAttrib( effectNode );
        a->name = "ref";
        a->value = fs::relPath( effect, basedir );
    }

    // create mesh node
    XmlElement * meshNode = doc.createElement(modelNode);
    meshNode->name = "mesh";
    if( mesh.empty() )
    {
        GN_ERROR(sLogger)( "Mesh name can not be empty." );
        return NULL;
    }
    else
    {
        XmlAttrib * a = doc.createAttrib( meshNode );
        a->name = "ref";
        a->value = fs::relPath( mesh, basedir );
    }

    // create subset node
    XmlElement * subsetNode = doc.createElement(modelNode);
    subsetNode->name = "subset";
    if( !subset.isWholeMesh() )
    {
        XmlAttrib * a = doc.createAttrib( subsetNode );
        a->name = "basevtx";
        a->value = stringFormat( "%u", subset.basevtx );

        a = doc.createAttrib( subsetNode );
        a->name = "numvtx";
        a->value = stringFormat( "%u", subset.numvtx );

        a = doc.createAttrib( subsetNode );
        a->name = "startidx";
        a->value = stringFormat( "%u", subset.startidx );

        a = doc.createAttrib( subsetNode );
        a->name = "numidx";
        a->value = stringFormat( "%u", subset.numidx );
    }

    // create texture nodes
    for( const StringMap<char,ModelTextureDesc>::KeyValuePair * i = textures.first();
         i != NULL;
         i = textures.next( i ) )
    {
        const StrA             & texname = i->key;
        const ModelTextureDesc & texdesc = i->value;

        XmlElement * textureNode = doc.createElement(modelNode);
        textureNode->name = "texture";

        XmlAttrib * a = doc.createAttrib( textureNode );
        a->name = "shaderParameter";
        a->value = texname;
        if( texdesc.resourceName.empty() )
        {
            //texdesc.desc.saveToXml( basedir );
            GN_UNIMPL();
            return NULL;
        }
        else
        {
            a = doc.createAttrib( textureNode );
            a->name = "ref";
            a->value = fs::relPath( texdesc.resourceName, basedir );
        }
    }

    // create uniform nodes
    for( const StringMap<char,ModelUniformDesc>::KeyValuePair * i = uniforms.first();
         i != NULL;
         i = uniforms.next( i ) )
    {
        const StrA             & uniname = i->key;
        const ModelUniformDesc & unidesc = i->value;

        XmlElement * uniformNode = doc.createElement(modelNode);
        uniformNode->name = "uniform";

        XmlAttrib * a = doc.createAttrib( uniformNode );
        a->name = "shaderParameter";
        a->value = uniname;

        if( unidesc.resourceName.empty() )
        {
            a = doc.createAttrib( uniformNode );
            a->name = "size";
            a->value = stringFormat( "%u", unidesc.size );

            if( !unidesc.initialValue.empty() )
            {
                XmlElement * bin = doc.createElement( uniformNode );
                bin->name = "initialValue";
                sBinaryEncode( bin->text, unidesc.initialValue.cptr(), unidesc.initialValue.size() );
            }
        }
        else
        {
            a = doc.createAttrib( uniformNode );
            a->name = "ref";
            a->value = fs::relPath( unidesc.resourceName, basedir );
        }
    }

    // done
    modelNode->setParent( &root );
    return modelNode;
}

// *****************************************************************************
// TextureItem
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::ModelResource::Impl::TextureItem::TextureItem()
    : mOwner( 0 )
{
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::ModelResource::Impl::TextureItem::~TextureItem()
{
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::TextureItem::setResource(
    Impl            & owner,
    size_t            effectParameterIndex,
    TextureResource * newTexture )
{
    if( mResource.get() == newTexture ) return;

    // disconnect from old handle
    if( mResource ) mResource->sigTextureChanged.disconnect( this );

    Texture * tex;
    if( newTexture )
    {
        // connect to new handle
        newTexture->sigTextureChanged.connect( this, &TextureItem::onTextureChange );

        tex = newTexture->texture();
    }
    else
    {
        tex = NULL;
    }

    // update stored handle value
    mOwner = &owner;
    mEffectParameterIndex = effectParameterIndex;
    mResource.set( newTexture );

    updateContext( tex );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::TextureItem::onTextureChange( TextureResource & r )
{
    GN_ASSERT( &r == mResource );
    GN_UNUSED_PARAM( r );

    updateContext( mResource->texture() );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::TextureItem::updateContext( Texture * tex )
{
    GN_ASSERT( mOwner );

    GN_ASSERT( mOwner->mEffectResource );

    EffectResource * effect = mOwner->mEffectResource;

    GN_ASSERT( mOwner->mPasses.size() == effect->numPasses() );

    const EffectResource::TextureProperties & prop = effect->textureProperties( mEffectParameterIndex );

    for( size_t i = 0; i < prop.bindings.size(); ++i )
    {
        const EffectResource::BindingLocation & location = prop.bindings[i];

        GN_ASSERT( location.pass < mOwner->mPasses.size() );
        GN_ASSERT( location.gpuProgramParameterIndex < GpuContext::MAX_TEXTURES );

        TextureBinding & binding = mOwner->mPasses[location.pass].gc.textures[location.gpuProgramParameterIndex];

        binding.texture.set( tex );
        binding.sampler = prop.sampler;
    }
}

// *****************************************************************************
// UniformItem
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::ModelResource::Impl::UniformItem::UniformItem()
    : mOwner( 0 )
{
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::ModelResource::Impl::UniformItem::~UniformItem()
{
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::UniformItem::setResource(
    Impl            & owner,
    size_t            effectParameterIndex,
    UniformResource * newUniform )
{
    if( mResource.get() == newUniform ) return;

    // disconnect from old handle
    if( mResource ) mResource->sigUniformChanged.disconnect( this );

    Uniform * uniform;
    if( newUniform )
    {
        // connect to new handle
        newUniform->sigUniformChanged.connect( this, &UniformItem::onUniformChange );

        uniform = newUniform->uniform();
    }
    else
    {
        uniform = NULL;
    }

    // update stored handle value
    mOwner = &owner;
    mEffectParameterIndex = effectParameterIndex;
    mResource.set( newUniform );

    updateContext( uniform );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::UniformItem::onUniformChange( UniformResource & r )
{
    GN_ASSERT( &r == mResource );
    GN_UNUSED_PARAM( r );

    updateContext( mResource->uniform() );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::UniformItem::updateContext( Uniform * uniform )
{
    GN_ASSERT( mOwner );

    GN_ASSERT( mOwner->mEffectResource );

    EffectResource * effect = mOwner->mEffectResource;

    GN_ASSERT( mOwner->mPasses.size() == effect->numPasses() );

    const EffectResource::UniformProperties & prop = effect->uniformProperties( mEffectParameterIndex );

    for( size_t i = 0; i < prop.bindings.size(); ++i )
    {
        const EffectResource::BindingLocation & location = prop.bindings[i];

        GN_ASSERT( location.pass < mOwner->mPasses.size() );
        GN_ASSERT( location.gpuProgramParameterIndex < GpuContext::MAX_TEXTURES );

        GpuContext & gc = mOwner->mPasses[location.pass].gc;

        if( location.gpuProgramParameterIndex >= gc.uniforms.size() )
        {
            gc.uniforms.resize( location.gpuProgramParameterIndex + 1 );
        }

        gc.uniforms[location.gpuProgramParameterIndex].set( uniform );
    }
}

// *****************************************************************************
// GN::gfx::ModelResource::Impl - Initialize and shutdown
// *****************************************************************************

// *****************************************************************************
// GN::gfx::ModelResource::Impl - public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::ModelResource::Impl::reset( const ModelResourceDesc * desc )
{
    clear();

    if( desc && !fromDesc( *desc ) )
    {
        clear();
        return false;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
AutoRef<ModelResource>
GN::gfx::ModelResource::Impl::makeClone( const char * nameOfTheClone ) const
{
    AutoRef<ModelResource> clone = getGdb().createResource<ModelResource>( nameOfTheClone );

    if( !clone ) return AutoRef<ModelResource>::NULLREF;

    clone->mImpl->copyFrom( *this );

    return clone;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::ModelResource::Impl::setTextureResource( const char * effectParameterName, GpuResource * texture )
{
    if( texture && !getGdb().validResource( TextureResource::guid(), texture ) )
    {
        GN_ERROR(sLogger)( "Invalid texture resource pointer.", modelName() );
        return false;
    }

    EffectResource * effect = mEffectResource;
    if( NULL == effect )
    {
        GN_ERROR(sLogger)( "Model %s is not referencing any effect!", modelName() );
        return false;
    }

    size_t parameterIndex = effect->findTexture( effectParameterName );
    if( EffectResource::PARAMETER_NOT_FOUND == parameterIndex )
    {
        GN_ERROR(sLogger)( "%s is not a valid texture name for model %s!", effectParameterName, modelName() );
        return false;
    }

    mTextures[parameterIndex].setResource( *this, parameterIndex, GpuResource::castTo<TextureResource>(texture) );

    return true;
}

//
//
// -----------------------------------------------------------------------------
AutoRef<TextureResource>
GN::gfx::ModelResource::Impl::textureResource( const char * effectParameterName ) const
{
    EffectResource * effect = mEffectResource;
    if( NULL == effect )
    {
        GN_ERROR(sLogger)( "Model %s is not referencing any effect!", modelName() );
        return AutoRef<TextureResource>::NULLREF;
    }

    size_t parameterIndex = effect->findTexture( effectParameterName );
    if( EffectResource::PARAMETER_NOT_FOUND == parameterIndex )
    {
        GN_ERROR(sLogger)( "%s is not a valid texture name for model %s!", effectParameterName, modelName() );
        return AutoRef<TextureResource>::NULLREF;
    }

    return mTextures[parameterIndex].getResource();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::ModelResource::Impl::setUniformResource( const char * effectParameterName, GpuResource * uniform )
{
    if( uniform && !getGdb().validResource( UniformResource::guid(), uniform ) )
    {
        GN_ERROR(sLogger)( "Invalid uniform resource pointer.", modelName() );
        return false;
    }

    EffectResource * effect = mEffectResource;
    if( NULL == effect )
    {
        GN_ERROR(sLogger)( "Model %s is not referencing any effect!", modelName() );
        return false;
    }

    size_t parameterIndex = effect->findUniform( effectParameterName );
    if( EffectResource::PARAMETER_NOT_FOUND == parameterIndex )
    {
        GN_ERROR(sLogger)( "%s is not a valid uniform name for model %s!", effectParameterName, modelName() );
        return false;
    }

    mUniforms[parameterIndex].setResource( *this, parameterIndex, GpuResource::castTo<UniformResource>(uniform) );

    return true;
}

//
//
// -----------------------------------------------------------------------------
AutoRef<UniformResource>
GN::gfx::ModelResource::Impl::uniformResource( const char * effectParameterName ) const
{
    EffectResource * effect = mEffectResource;
    if( NULL == effect )
    {
        AutoRef<UniformResource> & dummy = mDummyUniforms["NULL_EFFECT"];
        if( !dummy )
        {
            GN_ERROR(sLogger)( "Model %s is not referencing any effect!", modelName() );
            dummy = getGdb().createResource<UniformResource>( NULL );
            AutoRef<Uniform> u( getGdb().getGpu().createUniform( sizeof(float) ) );
            dummy->setUniform( u );
        }

        return dummy;
    }

    size_t parameterIndex = effect->findUniform( effectParameterName );
    if( EffectResource::PARAMETER_NOT_FOUND == parameterIndex )
    {
        AutoRef<UniformResource> & dummy = mDummyUniforms[effectParameterName?effectParameterName:"NULL_PARAMETER"];
        if( !dummy )
        {
            GN_ERROR(sLogger)( "%s is not a valid uniform name for model %s!", effectParameterName?effectParameterName:"<NULL name>", modelName() );
            dummy = getGdb().createResource<UniformResource>( NULL );
            AutoRef<Uniform> u( getGdb().getGpu().createUniform( sizeof(float) ) );
            dummy->setUniform( u );
        }
        return dummy;
    }

    return mUniforms[parameterIndex].getResource();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::ModelResource::Impl::setMeshResource(
    GpuResource              * resource,
    const MeshResourceSubset * subset )
{
    // check mesh resource pointer
    if( resource && !getGdb().validResource( MeshResource::guid(), resource ) )
    {
        GN_ERROR(sLogger)( "invalid mesh resource pointer" );
        return false;
    }

    MeshResource * mesh = GpuResource::castTo<MeshResource>(resource);

    // bind mesh signal with the old mesh
    if( mMeshResource != mesh )
    {
        if( mMeshResource ) mMeshResource->sigMeshChanged.disconnect( this );
        if( mesh ) mesh->sigMeshChanged.connect( this, &Impl::onMeshChanged );
    }

    // update mesh resource pointer
    mMeshResource.set( mesh );

    // update mesh subset
    if( subset )
    {
        mMeshSubset = *subset;
    }
    else
    {
        mMeshSubset.clear();
    }

    // update GPU contexts
    for( size_t i = 0; i < mPasses.size(); ++i )
    {
        RenderPass & pass = mPasses[i];

        if( mMeshResource )
        {
            mMeshResource->applyToContext( pass.gc );
        }
    }

    updateVertexFormat();

    return true;
}

//
//
// -----------------------------------------------------------------------------
AutoRef<MeshResource>
GN::gfx::ModelResource::Impl::meshResource( MeshResourceSubset * subset ) const
{
    if( subset )*subset = mMeshSubset;
    return mMeshResource;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::ModelResource::Impl::setEffectResource( GpuResource * resource )
{
    if( resource && !getGdb().validResource( EffectResource::guid(), resource ) )
    {
        GN_ERROR(sLogger)( "Invalid effect resource pointer.", modelName() );
        return false;
    }

    EffectResource * effect = GpuResource::castTo<EffectResource>(resource);

    // rebind changing signal
    if( effect != mEffectResource )
    {
        if( mEffectResource ) mEffectResource->sigEffectChanged.disconnect( this );
        if( effect ) effect->sigEffectChanged.connect( this, &Impl::onEffectChanged );
    }

    // update effect resource pointer
    mEffectResource.set( effect );

    // initialize passes array
    size_t numpasses = effect ? effect->numPasses() : 0;
    mPasses.resize( numpasses );
    for( size_t i = 0; i < mPasses.size(); ++i )
    {
        RenderPass & pass = mPasses[i];

        pass.gc.clear();
        effect->applyToContext( i, pass.gc );

        pass.renderstates = effect->renderStates( i );
    }

    // reapply mesh
    GN_VERIFY( setMeshResource( mMeshResource, &mMeshSubset ) );

    // reapply textures
    size_t numtextures = effect ? effect->numTextures() : 0;
    mTextures.resize( numtextures );
    for( size_t i = 0; i < numtextures; ++i )
    {
        TextureItem & ti = mTextures[i];

        AutoRef<TextureResource> texres = ti.getResource();

        if( !texres )
        {
            const EffectResource::TextureProperties & tp = mEffectResource->textureProperties( i );
            StrA texname = stringFormat( "%s.texture.%s", modelName(), tp.parameterName.cptr() );
            texres = getGdb().findOrCreateResource<TextureResource>( texname );
            if( !texres ) return false;
        }

        ti.setResource( *this, i, NULL );
        ti.setResource( *this, i, texres );
    }

    // reapply uniforms
    size_t numuniforms = effect ? effect->numUniforms() : 0;
    mUniforms.resize( numuniforms );
    for( size_t i = 0; i < numuniforms; ++i )
    {
        UniformItem & ui = mUniforms[i];

        AutoRef<UniformResource> unires = ui.getResource();

        if( !unires )
        {
            const EffectResource::UniformProperties & up = mEffectResource->uniformProperties( i );
            StrA uniname = stringFormat( "%s.uniform.%s", modelName(), up.parameterName.cptr() );
            unires = getGdb().findOrCreateResource<UniformResource>( uniname );
            if( !unires ) return false;
            AutoRef<Uniform> u = unires->uniform();
            if( !u && up.size > 0 )
            {
                u.attach( getGdb().getGpu().createUniform( up.size ) );
                if( !u ) return false;
            }
            unires->setUniform( u );
        }

        ui.setResource( *this, i, NULL );
        ui.setResource( *this, i, unires );
    }

    // update vertex format
    updateVertexFormat();

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::draw() const
{
    MeshResource * mesh = mMeshResource;
    if( NULL == mesh ) return;

    const MeshResourceDesc & meshdesc = mesh->getDesc();

    Gpu & g = getGdb().getGpu();

    const GpuContext & currentContext = g.getContext();

    for( size_t i = 0; i < mPasses.size(); ++i )
    {
        GpuContext & gc = mPasses[i].gc;

        // copy render targets from current context
        gc.colortargets = currentContext.colortargets;
        gc.depthstencil = currentContext.depthstencil;

        // apply render states
        sApplyRenderStates( gc.rs, currentContext.rs, mPasses[i].renderstates );
    }

    // determine the subset
    MeshResourceSubset subset = mMeshSubset;
    if( 0 == subset.basevtx && 0 == subset.numvtx )
    {
        subset.numvtx = meshdesc.numvtx;
    }
    if( 0 == subset.startidx && 0 == subset.numidx )
    {
        subset.numidx = meshdesc.numidx;
    }

    // draw
    for( size_t i = 0; i < mPasses.size(); ++i )
    {
        const GpuContext & gc = mPasses[i].gc;

        g.bindContext( gc );

        // do rendering
        if( gc.idxbuf )
        {
            g.drawIndexed(
                meshdesc.prim,
                subset.numidx,
                subset.basevtx,
                0, // startvtx,
                subset.numvtx,
                subset.startidx );
        }
        else
        {
            g.draw(
                meshdesc.prim,
                subset.numvtx,
                subset.basevtx );
        }

    }
}

// *****************************************************************************
// GN::gfx::ModelResource::Impl - private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::ModelResource::Impl::fromDesc( const ModelResourceDesc & desc )
{
    GpuResourceDatabase & db = getGdb();

    // initialize effect
    if( !desc.effect.empty() )
    {
        AutoRef<EffectResource> effect = db.findResource<EffectResource>( desc.effect );
        if( 0 == effect )
        {
            effect = EffectResource::loadFromFile( db, desc.effect );
            if( 0 == effect )
            {
                GN_ERROR(sLogger)( "%s is not a valid effect resource name.", desc.effect.cptr() );
                return false;
            }
        }
        if( !setEffectResource( effect ) ) return false;
    }

    // initialize mesh
    if( !desc.mesh.empty() )
    {
        AutoRef<MeshResource> mesh = db.findResource<MeshResource>( desc.mesh );
        if( 0 == mesh )
        {
            mesh = MeshResource::loadFromFile( db, desc.mesh );
            if( 0 == mesh )
            {
                GN_ERROR(sLogger)( "%s is not a valid mesh resource name.", desc.mesh.cptr() );
                return false;
            }
        }
        if( !setMeshResource( mesh, &desc.subset ) ) return false;
    }

    // setup textures
    GN_ASSERT( mTextures.size() == (mEffectResource ? mEffectResource->numTextures() : 0) );
    for( size_t i = 0; i < mTextures.size(); ++i )
    {
        TextureItem & t = mTextures[i];

        const EffectResource::TextureProperties & tp = mEffectResource->textureProperties( i );

        const ModelResourceDesc::ModelTextureDesc * td = desc.textures.find( tp.parameterName );

        AutoRef<TextureResource> texres;

        if( td )
        {
            if( !td->resourceName.empty() )
            {
                texres = TextureResource::loadFromFile( db, td->resourceName );
            }
            else
            {
                StrA texname = stringFormat( "%s.texture.%s", modelName(), tp.parameterName.cptr() );
                texres = db.findOrCreateResource<TextureResource>( texname );
                if( texres ) texres->reset( &td->desc );
            }
        }
        else
        {
            GN_ERROR(sLogger)(
                "Effec texture parameter '%s' in effect '%s' is not defined in model '%s'.",
                tp.parameterName.cptr(),
                mEffectResource->name(),
                modelName() );

            return false;
        }

        t.setResource( *this, i, NULL );
        t.setResource( *this, i, texres );
    }

    // setup uniforms
    GN_ASSERT( mUniforms.size() == (mEffectResource?mEffectResource->numUniforms() : 0) );
    for( size_t i = 0; i < mUniforms.size(); ++i )
    {
        UniformItem & u = mUniforms[i];

        const EffectResource::UniformProperties & up = mEffectResource->uniformProperties( i );

        const ModelResourceDesc::ModelUniformDesc * ud = desc.uniforms.find( up.parameterName );

        AutoRef<UniformResource> unires;
        if( ud )
        {
            if( !ud->resourceName.empty() )
            {
                unires = db.findResource<UniformResource>( ud->resourceName );
                if( !unires )
                {
                    GN_ERROR(sLogger)( "Invalid uniform resource name '%s' in model '%s'.",
                        ud->resourceName.cptr(),
                        modelName() );
                }
            }
            else
            {
                StrA uniname = stringFormat( "%s.uniform.%s", modelName(), up.parameterName.cptr() );

                const void * initialValue = ud->initialValue.cptr();
                if( !ud->initialValue.empty() && ud->initialValue.size() != ud->size )
                {
                    GN_ERROR(sLogger)(
                        "Incorrect initial data size of uniform '%s in model '%s'.",
                        up.parameterName.cptr(),
                        modelName() );
                    initialValue = NULL;
                }

                unires = db.findOrCreateResource<UniformResource>( uniname );
                if( unires ) unires->reset( ud->size, initialValue );
            }
        }
        else
        {
            GN_ERROR(sLogger)(
                "Effec uniform parameter '%s' in effect '%s' is not defined in model '%s'.",
                up.parameterName.cptr(),
                mEffectResource->name(),
                modelName() );

            return false;
        }

        u.setResource( *this, i, NULL );
        u.setResource( *this, i, unires );
    }

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::clear()
{
    if( mEffectResource )
    {
        mEffectResource->sigEffectChanged.disconnect( this );
        mEffectResource.clear();
    }

    if( mMeshResource )
    {
        mMeshResource->sigMeshChanged.disconnect( this );
        mMeshResource.clear();
    }

    mPasses.clear();
    mTextures.clear();
    mUniforms.clear();
    mDummyUniforms.clear();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::copyFrom( const Impl & other )
{
    clear();

    GN_VERIFY( setEffectResource( other.mEffectResource ) );
    GN_ASSERT( mPasses.size() == other.mPasses.size() );

    GN_VERIFY( setMeshResource( other.mMeshResource, &other.mMeshSubset ) );

    GN_ASSERT( mTextures.size() == other.mTextures.size() );
    for( size_t i = 0; i < other.mTextures.size(); ++i )
    {
        mTextures[i].setResource( *this, i, other.mTextures[i].getResource() );
    }

    GN_ASSERT( mUniforms.size() == other.mUniforms.size() );
    for( size_t i = 0; i < other.mUniforms.size(); ++i )
    {
        mUniforms[i].setResource( *this, i, other.mUniforms[i].getResource() );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::onEffectChanged( EffectResource & r )
{
    GN_ASSERT( &r == mEffectResource );

    GN_VERIFY( setEffectResource( &r ) );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::onMeshChanged( MeshResource & r )
{
    GN_ASSERT( &r == mMeshResource );

    GN_VERIFY( setMeshResource( &r, &mMeshSubset ) );
}

/*static void UpdateVertexBinding( VertexElement & ve, const char * fullBindingName )
{
    const char * p = fullBindingName;
    while( *(p+1) ) ++p; // Move p to point to the last character.

    ve.bindingIndex = 0;
    while( '0' <= *p && *p <= '9' ) --p;
    ++p;
    const char * bindingEnd = p;
    while( '0' <= *p && *p <= '9' )
    {
        ve.bindingIndex = ve.bindingIndex * 10 + (*p - '0');
    }

    memset( ve.binding, 0, sizeof(ve.binding) );
    memcpy( ve.binding, fullBindingName, math::getmin<size_t>( sizeof(ve.binding), bindingEnd - fullBindingName ) );
    ve.binding[sizeof(ve.binding)-1] = '0';
}*/

//
// Update vertex format based on current effect and mesh
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::updateVertexFormat()
{
    for( size_t pi = 0; pi < mPasses.size(); ++pi )
    {
        VertexBinding & vtxbind = mPasses[pi].gc.vtxbind;

        vtxbind.clear();

        if( NULL == mMeshResource || NULL == mEffectResource || NULL == mPasses[pi].gc.gpuProgram )
        {
            continue;
        }

        const MeshVertexFormat & vtxfmt = mMeshResource->getDesc().vtxfmt;

        const GpuProgramParameterDesc & gppdesc = mPasses[pi].gc.gpuProgram->getParameterDesc();

        for( size_t vi = 0; vi < vtxfmt.numElements; ++vi )
        {
            const MeshVertexElement & mve = vtxfmt.elements[vi];

            size_t ai = mEffectResource->findAttribute( mve.semantic );
            if( EffectResource::PARAMETER_NOT_FOUND != ai )
            {
                const EffectResource::AttributeProperties & ap = mEffectResource->attributeProperties( ai );
                for( size_t bi = 0; bi < ap.bindings.size(); ++bi )
                {
                    const EffectResource::BindingLocation & bl = ap.bindings[bi];

                    if( bl.pass == pi && bl.gpuProgramParameterIndex < gppdesc.attributes.count() )
                    {
                        VertexElement ve;
                        ve.format    = mve.format;
                        ve.stream    = mve.stream;
                        ve.offset    = mve.offset;
                        ve.attribute = (UInt16)bl.gpuProgramParameterIndex;

                        vtxbind.append( ve );
                    }
                }
            }
            else
            {
                GN_WARN(sLogger)( "Mesh semantic '%s' is not found in Effect's attribute list", mve.semantic );
            }
        }
    }
}

// *****************************************************************************
// GN::gfx::ModelResource
// *****************************************************************************

class ModelResourceInternal : public ModelResource
{
    ModelResourceInternal( GpuResourceDatabase & db )
        : ModelResource( db )
    {
    }

public:

    static GpuResource *
    sCreateInstance( GpuResourceDatabase & db )
    {
        return new ModelResourceInternal( db );
    }
};

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::registerModelResourceFactory( GpuResourceDatabase & db )
{
    if( db.hasResourceFactory( ModelResource::guid() ) ) return true;

    GpuResourceFactory factory = { &ModelResourceInternal::sCreateInstance };

    return db.registerResourceFactory( ModelResource::guid(), "Model Resource", factory );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::ModelResource::ModelResource( GpuResourceDatabase & db )
    : GpuResource( db ), mImpl(NULL)
{
    mImpl = new Impl(*this);
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::ModelResource::~ModelResource()
{
    delete mImpl;
}

//
//
// -----------------------------------------------------------------------------
const Guid & GN::gfx::ModelResource::guid()
{
    static const Guid MODEL_GUID = { 0x24a6e5eb, 0xeb76, 0x440f, { 0xaa, 0x9d, 0x6a, 0x59, 0x34, 0x2f, 0x89, 0x2e } };
    return MODEL_GUID;
}

//
//
// -----------------------------------------------------------------------------
AutoRef<ModelResource>
GN::gfx::ModelResource::loadFromFile(
    GpuResourceDatabase & db,
    const char          * filename )
{
    if( NULL == filename )
    {
        GN_INFO(sLogger)( "Null filename string." );
        return AutoRef<ModelResource>::NULLREF;
    }

    // Reuse existing resource, if possible
    AutoRef<ModelResource> m( db.findResource<ModelResource>( filename ) );
    if( m ) return m;

    // convert to full (absolute) path
    StrA abspath = fs::resolvePath( fs::getCurrentDir(), filename );
    filename = abspath;

    // Try search for existing resource again with full path
    m = db.findResource<ModelResource>( filename );
    if( m ) return m;

    ModelResourceDesc desc;
    if( !loadFromXmlFile( desc, filename ) ) return AutoRef<ModelResource>::NULLREF;

    m = db.createResource<ModelResource>( abspath );
    if( !m || !m->reset( &desc ) ) return AutoRef<ModelResource>::NULLREF;

    return m;
}

//
//
// -----------------------------------------------------------------------------
bool                        GN::gfx::ModelResource::reset( const ModelResourceDesc * desc ) { return mImpl->reset( desc ); }
AutoRef<ModelResource>      GN::gfx::ModelResource::makeClone( const char * nameOfTheClone ) const { return mImpl->makeClone( nameOfTheClone ); }
void                        GN::gfx::ModelResource::setTextureResource( const char * effectParameterName, GpuResource * texture ) { mImpl->setTextureResource( effectParameterName, texture ); }
AutoRef<TextureResource>    GN::gfx::ModelResource::textureResource( const char * effectParameterName ) const { return mImpl->textureResource( effectParameterName ); }
void                        GN::gfx::ModelResource::setUniformResource( const char * effectParameterName, GpuResource * uniform ) { mImpl->setUniformResource( effectParameterName, uniform ); }
AutoRef<UniformResource>    GN::gfx::ModelResource::uniformResource( const char * effectParameterName ) const { return mImpl->uniformResource( effectParameterName ); }
void                        GN::gfx::ModelResource::setMeshResource( GpuResource * mesh, const MeshResourceSubset * subset ) { mImpl->setMeshResource( mesh, subset ); }
AutoRef<MeshResource>       GN::gfx::ModelResource::meshResource( MeshResourceSubset * subset ) const { return mImpl->meshResource( subset ); }
void                        GN::gfx::ModelResource::setEffectResource( GpuResource * effect ) { mImpl->setEffectResource( effect ); }
AutoRef<EffectResource>     GN::gfx::ModelResource::effectResource() const { return mImpl->effectResource(); }
void                        GN::gfx::ModelResource::draw() const { mImpl->draw(); }
