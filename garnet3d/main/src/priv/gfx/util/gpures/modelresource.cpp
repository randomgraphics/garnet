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
template<typename T>
static inline const T *
sFindNamedPtr( const std::map<StrA,T> & container, const StrA & name )
{
    typename std::map<StrA,T>::const_iterator iter = container.find( name );
    return ( container.end() == iter ) ? NULL : &iter->second;
}

//
//
// -----------------------------------------------------------------------------
static void
sApplyRenderState(
    GN::gfx::GpuContext::RenderStates               & dst,
    const GN::gfx::GpuContext::RenderStates         & src,
    const EffectResourceDesc::EffectRenderStateDesc & rsdesc )
{
    #define MERGE_SINGLE_RENDER_STATE( state ) dst.state = ( rsdesc.state.overridden ? rsdesc.state.value : src.state )

    MERGE_SINGLE_RENDER_STATE( depthTestEnabled );
    MERGE_SINGLE_RENDER_STATE( depthWriteEnabled );
    MERGE_SINGLE_RENDER_STATE( depthFunc );

    MERGE_SINGLE_RENDER_STATE( stencilEnabled );
    MERGE_SINGLE_RENDER_STATE( stencilPassOp );
    MERGE_SINGLE_RENDER_STATE( stencilFailOp );
    MERGE_SINGLE_RENDER_STATE( stencilZFailOp );

    MERGE_SINGLE_RENDER_STATE( blendEnabled );
    MERGE_SINGLE_RENDER_STATE( blendSrc );
    MERGE_SINGLE_RENDER_STATE( blendDst );
    MERGE_SINGLE_RENDER_STATE( blendOp );
    MERGE_SINGLE_RENDER_STATE( blendAlphaSrc );
    MERGE_SINGLE_RENDER_STATE( blendAlphaDst );
    MERGE_SINGLE_RENDER_STATE( blendAlphaOp );

    MERGE_SINGLE_RENDER_STATE( fillMode );
    MERGE_SINGLE_RENDER_STATE( cullMode );
    MERGE_SINGLE_RENDER_STATE( frontFace );
    MERGE_SINGLE_RENDER_STATE( msaaEnabled );

    MERGE_SINGLE_RENDER_STATE( blendFactors );
    MERGE_SINGLE_RENDER_STATE( colorWriteMask );
    MERGE_SINGLE_RENDER_STATE( viewport );
    MERGE_SINGLE_RENDER_STATE( scissorRect );

    #undef MERGE_SINGLE_RENDER_STATE
}

// *****************************************************************************
// local classes and functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResourceDesc::clear()
{
    effectResourceName.clear();
    effectResourceDesc.clear();
    textures.clear();
    uniforms.clear();
    meshResourceName.clear();
    meshResourceDesc.clear();
    subset.clear();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::ModelResourceDesc::loadFromXmlNode( const XmlNode & root, const char * basedir )
{
    GN_UNUSED_PARAM( root );
    GN_UNUSED_PARAM( basedir );
    GN_UNIMPL_WARNING();

    clear();

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResourceDesc::saveToXmlNode( const XmlNode & root )
{
    GN_UNUSED_PARAM( root );
    GN_UNIMPL_WARNING();
}

// *****************************************************************************
// TextureItem
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::ModelResource::Impl::TextureItem::TextureItem()
    : mOwner( 0 )
    , mHandle( 0 )
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
void GN::gfx::ModelResource::Impl::TextureItem::setHandle(
    Impl            & owner,
    size_t            effectParameterIndex,
    GpuResourceHandle newTextureHandle )
{
    if( mHandle == newTextureHandle ) return;

    GpuResourceDatabase & db = owner.mOwner.database();

    // disconnect from old handle
    if( mHandle )
    {
        GpuResource * r = db.getResource( mHandle );
        r->sigResourceChanged.disconnect( this );
    }

    Texture * tex;
    if( newTextureHandle )
    {
        // connect to new handle
        GpuResource * r = db.getResource( newTextureHandle );
        r->sigResourceChanged.connect( this, &TextureItem::onTextureChange );

        tex = ((TextureResource*)r)->getTexture();
    }
    else
    {
        tex = NULL;
    }

    // update stored handle value
    mOwner = &owner;
    mEffectParameterIndex = effectParameterIndex;
    mHandle = newTextureHandle;

    updateContext( tex );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::TextureItem::onTextureChange( GpuResource & r )
{
    GN_ASSERT( r.handle() == mHandle );

    Texture * tex = r.castTo<TextureResource>().getTexture();

    updateContext( tex );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::TextureItem::updateContext( Texture * tex )
{
    GN_ASSERT( mOwner );

    EffectResource * effect = (EffectResource*)mOwner->mOwner.database().getResource( mOwner->mEffect.handle );

    GN_ASSERT( mOwner->mPasses.size() == effect->getNumPasses() );

    const EffectResource::TextureProperties & prop = effect->getTextureProperties( mEffectParameterIndex );

    for( size_t i = 0; i < prop.bindings.size(); ++i )
    {
        const EffectResource::BindingLocation & location = prop.bindings[i];

        GN_ASSERT( location.pass < mOwner->mPasses.size() );
        GN_ASSERT( location.stage < GpuContext::MAX_TEXTURES );

        TextureBinding & binding = mOwner->mPasses[location.pass].gc.textures[location.stage];

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
    , mHandle( 0 )
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
void GN::gfx::ModelResource::Impl::UniformItem::setHandle(
    Impl            & owner,
    size_t            effectParameterIndex,
    GpuResourceHandle newUniformHandle )
{
    if( mHandle == newUniformHandle ) return;

    GpuResourceDatabase & db = owner.mOwner.database();

    // disconnect from old handle
    if( mHandle )
    {
        GpuResource * r = db.getResource( mHandle );
        r->sigResourceChanged.disconnect( this );
    }

    Uniform * uniform;
    if( newUniformHandle )
    {
        // connect to new handle
        GpuResource * r = db.getResource( newUniformHandle );
        r->sigResourceChanged.connect( this, &UniformItem::onUniformChange );

        uniform = ((UniformResource*)r)->getUniform();
    }
    else
    {
        uniform = NULL;
    }

    // update stored handle value
    mOwner = &owner;
    mEffectParameterIndex = effectParameterIndex;
    mHandle = newUniformHandle;

    updateContext( uniform );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::UniformItem::onUniformChange( GpuResource & r )
{
    GN_ASSERT( r.handle() == mHandle );

    Uniform * uniform = r.castTo<UniformResource>().getUniform();

    updateContext( uniform );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::UniformItem::updateContext( Uniform * uniform )
{
    GN_ASSERT( mOwner );

    EffectResource * effect = (EffectResource*)mOwner->mOwner.database().getResource( mOwner->mEffect.handle );

    GN_ASSERT( mOwner->mPasses.size() == effect->getNumPasses() );

    const EffectResource::UniformProperties & prop = effect->getUniformProperties( mEffectParameterIndex );

    for( size_t i = 0; i < prop.bindings.size(); ++i )
    {
        const EffectResource::BindingLocation & location = prop.bindings[i];

        GN_ASSERT( location.pass < mOwner->mPasses.size() );
        GN_ASSERT( location.stage < GpuContext::MAX_TEXTURES );

        GpuContext & gc = mOwner->mPasses[location.pass].gc;

        if( location.stage >= gc.uniforms.size() )
        {
            gc.uniforms.resize( location.stage + 1 );
        }

        gc.uniforms[location.stage].set( uniform );
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

    if( desc && !init( *desc ) )
    {
        clear();
        return false;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::setTexture( const char * effectParameterName, GpuResourceHandle handle )
{
    EffectResource * effect = GpuResource::castTo<EffectResource>( database().getResource( mEffect.handle ) );
    if( NULL == effect )
    {
        GN_ERROR(sLogger)( "Model %s is referencing a invalid effect handle!", modelName() );
        return;
    }

    size_t parameterIndex = effect->findTexture( effectParameterName );
    if( EffectResource::PARAMETER_NOT_FOUND == parameterIndex )
    {
        GN_ERROR(sLogger)( "%s is not a valid texture name for model %s!", effectParameterName, modelName() );
        return;
    }

    mTextures[parameterIndex].setHandle( *this, parameterIndex, handle );
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle
GN::gfx::ModelResource::Impl::getTexture( const char * effectParameterName ) const
{
    EffectResource * effect = GpuResource::castTo<EffectResource>( database().getResource( mEffect.handle ) );
    if( NULL == effect )
    {
        GN_ERROR(sLogger)( "Model %s is referencing a invalid effect handle!", modelName() );
        return 0;
    }

    size_t parameterIndex = effect->findTexture( effectParameterName );
    if( EffectResource::PARAMETER_NOT_FOUND == parameterIndex )
    {
        GN_ERROR(sLogger)( "%s is not a valid texture name for model %s!", effectParameterName, modelName() );
        return 0;
    }

    return mTextures[parameterIndex].getHandle();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::setUniform( const char * effectParameterName, GpuResourceHandle handle )
{
    EffectResource * effect = GpuResource::castTo<EffectResource>( database().getResource( mEffect.handle ) );
    if( NULL == effect )
    {
        GN_ERROR(sLogger)( "Model %s is referencing a invalid effect handle!", modelName() );
        return;
    }

    size_t parameterIndex = effect->findUniform( effectParameterName );
    if( EffectResource::PARAMETER_NOT_FOUND == parameterIndex )
    {
        GN_ERROR(sLogger)( "%s is not a valid uniform name for model %s!", effectParameterName, modelName() );
        return;
    }

    mUniforms[parameterIndex].setHandle( *this, parameterIndex, handle );
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle
GN::gfx::ModelResource::Impl::getUniform( const char * effectParameterName ) const
{
    EffectResource * effect = GpuResource::castTo<EffectResource>( database().getResource( mEffect.handle ) );
    if( NULL == effect )
    {
        GN_ERROR(sLogger)( "Model %s is referencing a invalid effect handle!", modelName() );
        return 0;
    }

    size_t parameterIndex = effect->findUniform( effectParameterName );
    if( EffectResource::PARAMETER_NOT_FOUND == parameterIndex )
    {
        GN_ERROR(sLogger)( "%s is not a valid uniform name for model %s!", effectParameterName, modelName() );
        return 0;
    }

    return mUniforms[parameterIndex].getHandle();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::setMesh(
    GpuResourceHandle          mesh,
    const MeshResourceSubset * subset )
{
    if( !database().isValidResourceHandle( mesh ) ||
        MeshResource::guid() != database().getResourceType(mesh) )
    {
        GN_ERROR(sLogger)( "invalid mesh handle" );
        return;
    }

    // detach with the old mesh
    if( mMesh.handle )
    {
        GpuResource & m = database().getResource( mMesh.handle )->castTo<MeshResource>();
        m.sigResourceChanged.disconnect( this );
    }

    mMesh.handle = mesh;

    if( subset )
    {
        mDesc.subset = *subset;
    }
    else
    {
        mDesc.subset.clear();
    }

    GpuResource & m = database().getResource( mMesh.handle )->castTo<MeshResource>();
    m.sigResourceChanged.connect( this, &Impl::onMeshChanged );

    onMeshChanged( m );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::draw() const
{
    MeshResource * mesh = GpuResource::castTo<MeshResource>( database().getResource( mMesh.handle ) );
    if( NULL == mesh ) return;
    const MeshResourceDesc & meshdesc = mesh->getDesc();

    Gpu & g = database().gpu();

    const GpuContext & currentContext = g.getContext();

    for( size_t i = 0; i < mPasses.size(); ++i )
    {
        GpuContext & gc = mPasses[i].gc;

        // copy render targets from current context
        gc.colortargets = currentContext.colortargets;
        gc.depthstencil = currentContext.depthstencil;

        // TODO: copy render states from current context
        sApplyRenderState( gc.rs, currentContext.rs, mPasses[i].rsdesc );
    }

    // determine the subset
    MeshResourceSubset subset = mDesc.subset;
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
bool GN::gfx::ModelResource::Impl::init( const ModelResourceDesc & desc )
{
    GpuResourceDatabase & db = database();

    // initialize effect handle
    mEffect.handle = 0;
    if( !desc.effectResourceName.empty() )
    {
        mEffect.handle = db.findResource( EffectResource::guid(), desc.effectResourceName );
        if( 0 == mEffect.handle )
        {
            GN_ERROR(sLogger)( "%s is not a valid effect name.", desc.effectResourceName.cptr() );
        }
    }
    else
    {
        mEffect.handle = db.findOrCreateResource( EffectResource::guid(), strFormat("%s.effect", modelName()) );
        db.getResource(mEffect.handle)->castTo<EffectResource>().reset( &desc.effectResourceDesc );
    }

    // fallback to dummy effect
    if( 0 == mEffect.handle )
    {
        mEffect.handle = db.findResource( EffectResource::guid(), "dummy" );
        if( 0 == mEffect.handle ) GN_ERROR(sLogger)( "No dummy effect defined in GPU resource database." );
    }
    if( 0 == mEffect.handle )
    {
        GN_ERROR(sLogger)( "Fail to initialize effect for model '%s'.", modelName() );
        return false;
    }

    // initialize mesh
    if( !desc.meshResourceName.empty() )
    {
        mMesh.handle = db.findResource( MeshResource::guid(), desc.meshResourceName );
        if( 0 == mMesh.handle )
        {
            GN_ERROR(sLogger)( "%s is not a valid mesh name.", desc.meshResourceName.cptr() );
        }
    }
    else
    {
        mMesh.handle = db.findOrCreateResource( MeshResource::guid(), strFormat("%s.model", modelName()) );
        db.getResource( mMesh.handle )->castTo<MeshResource>().reset( &desc.meshResourceDesc );
    }
    if( 0 == mMesh.handle )
    {
        mMesh.handle = db.findResource( MeshResource::guid(), "dummy" );
        if( 0 == mMesh.handle ) GN_ERROR(sLogger)( "No dummy mesh defined in GPU resource database." );
    }
    if( 0 == mMesh.handle )
    {
        return false;
    }
    GpuResource * mesh = db.getResource( mMesh.handle );
    mesh->sigResourceChanged.connect( this, &Impl::onMeshChanged );

    // attach to effect changing event
    GpuResource * effect = db.getResource( mEffect.handle );
    effect->sigResourceChanged.connect( this, &Impl::onEffectChanged );

    // store the descriptor (used in onEffectChanged())
    mDesc = desc;

    // trigger a effect changing event to initialize everthing else.
    onEffectChanged( *effect );

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::clear()
{
    GpuResourceDatabase & db = database();

    if( db.isValidResourceHandle( mMesh.handle ) )
    {
        GpuResource * mesh = db.getResource( mMesh.handle );
        mesh->sigResourceChanged.disconnect( this );
    }
    mMesh.handle = 0;

    if( db.isValidResourceHandle( mEffect.handle ) )
    {
        GpuResource * effect = db.getResource( mEffect.handle );
        effect->sigResourceChanged.disconnect( this );
    }
    mEffect.handle = 0;

    mPasses.clear();
    mTextures.clear();
    mUniforms.clear();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::onEffectChanged( GpuResource & r )
{
    GpuResourceDatabase & db = database();

    EffectResource & effect = r.castTo<EffectResource>();

    // initialize passes array
    mPasses.resize( effect.getNumPasses() );
    for( size_t i = 0; i < mPasses.size(); ++i )
    {
        RenderPass & pass = mPasses[i];

        pass.gc.clear();
        effect.applyToContext( i, pass.gc );

        pass.rsdesc = effect.getRenderState( i );
    }

    // reapply mesh
    if( mMesh.handle )
    {
        GpuResource * mesh = db.getResource( mMesh.handle );
        if( mesh ) onMeshChanged( *mesh );
    }

    // reapply textures
    mTextures.resize( effect.getNumTextures() );
    for( size_t i = 0; i < effect.getNumTextures(); ++i )
    {
        TextureItem & t = mTextures[i];

        const EffectResource::TextureProperties & tp = effect.getTextureProperties( i );

        const ModelResourceDesc::ModelTextureDesc * td = sFindNamedPtr( mDesc.textures, tp.parameterName );

        GpuResourceHandle texhandle;
        if( td )
        {
            if( !td->resourceName.empty() )
            {
                texhandle = TextureResource::loadFromFile( db, td->resourceName );
            }
            else
            {
                StrA texname = strFormat( "%s.texture.%s", modelName(), tp.parameterName.cptr() );
                texhandle = db.findOrCreateResource( TextureResource::guid(), texname );
                db.getResource( texhandle )->castTo<TextureResource>().reset( &td->desc );
            }
        }
        else
        {
            GN_WARN(sLogger)(
                "Effec texture parameter '%s' in effect '%s' is not defined in model '%s'.",
                tp.parameterName.cptr(),
                db.getResourceName( effect.handle() ),
                modelName() );

            texhandle = 0;
        }

        t.setHandle( *this, i, 0 );
        t.setHandle( *this, i, texhandle );
    }

    // reapply uniforms
    mUniforms.resize( effect.getNumUniforms() );
    for( size_t i = 0; i < effect.getNumUniforms(); ++i )
    {
        UniformItem & u = mUniforms[i];

        const EffectResource::UniformProperties & up = effect.getUniformProperties( i );

        const ModelResourceDesc::ModelUniformDesc * ud = sFindNamedPtr( mDesc.uniforms, up.parameterName );

        GpuResourceHandle uniformhandle;
        if( ud )
        {
            if( !ud->resourceName.empty() )
            {
                uniformhandle = db.findResource( UniformResource::guid(), ud->resourceName );
                if( 0 == uniformhandle )
                {
                    GN_ERROR(sLogger)( "Invalid uniform resource name '%s' in model '%s'.",
                        ud->resourceName.cptr(),
                        modelName() );
                }
            }
            else
            {
                StrA uniname = strFormat( "%s.uniform.%s", modelName(), up.parameterName.cptr() );

                const void * initialValue = ud->initialValue.cptr();
                if( !ud->initialValue.empty() && ud->initialValue.size() != ud->size )
                {
                    GN_ERROR(sLogger)(
                        "Incorrect initial data size of uniform '%s in model '%s'.",
                        up.parameterName.cptr(),
                        modelName() );
                    initialValue = NULL;
                }

                uniformhandle = db.findOrCreateResource( UniformResource::guid(), uniname );
                db.getResource(uniformhandle)->castTo<UniformResource>().reset( ud->size, initialValue );
            }
        }
        else
        {
            GN_WARN(sLogger)(
                "Effec uniform parameter '%s' in effect '%s' is not defined in model '%s'.",
                up.parameterName.cptr(),
                db.getResourceName( effect.handle() ),
                modelName() );

            uniformhandle = 0;
        }

        u.setHandle( *this, i, 0 );
        u.setHandle( *this, i, uniformhandle );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::onMeshChanged( GpuResource & r )
{
    MeshResource & mesh = r.castTo<MeshResource>( r );

    const MeshResourceDesc & meshdesc = mesh.getDesc();

    if( ( mDesc.subset.startidx + mDesc.subset.numidx ) > meshdesc.numidx ||
        ( mDesc.subset.basevtx + mDesc.subset.numvtx ) > meshdesc.numvtx )
    {
        GN_ERROR(sLogger)( "Mesh subset is out of range." );
    }

    for( size_t i = 0; i < mPasses.size(); ++i )
    {
        RenderPass & pass = mPasses[i];

        mesh.applyToContext( pass.gc );
    }
}

// *****************************************************************************
// GN::gfx::ModelResource
// *****************************************************************************

class ModelResourceInternal : public ModelResource
{
    ModelResourceInternal( GpuResourceDatabase & db, GpuResourceHandle handle )
        : ModelResource( db, handle )
    {
    }

public:

    static GpuResource *
    sCreateInstance( GpuResourceDatabase & db,
                     GpuResourceHandle     handle )
    {
        return new ModelResourceInternal( db, handle );
    }

    static void sDeleteInstance( GpuResource * p )
    {
        delete GpuResource::castTo<ModelResourceInternal>( p );
    }
};

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::registerModelResourceFactory( GpuResourceDatabase & db )
{
    if( db.hasResourceFactory( ModelResource::guid() ) ) return true;

    GpuResourceFactory factory = { &ModelResourceInternal::sCreateInstance, &ModelResourceInternal::sDeleteInstance };

    return db.registerResourceFactory( ModelResource::guid(), "Model Resource", factory );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::ModelResource::ModelResource( GpuResourceDatabase & db, GpuResourceHandle h )
    : GpuResource( db, h ), mImpl(NULL)
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
GpuResourceHandle GN::gfx::ModelResource::loadFromFile(
    GpuResourceDatabase & db,
    const char          * filename )
{
    StrA abspath = fs::resolvePath( fs::getCurrentDir(), filename );
    filename = abspath;

    GpuResourceHandle handle = db.findResource( guid(), filename );
    if( handle ) return handle;

    ModelResourceDesc desc;
    desc.clear();
    //if( !loadFromXmlFile( desc, filename ) ) return 0;

    GpuResourceHandle h = db.createResource( ModelResource::guid(), abspath );
    if( 0 == h ) return 0;

    ModelResource & m = db.getResource(h)->castTo<ModelResource>();

    m.reset( &desc );

    return h;
}

//
//
// -----------------------------------------------------------------------------
bool              GN::gfx::ModelResource::reset( const ModelResourceDesc * desc ) { return mImpl->reset( desc ); }
void              GN::gfx::ModelResource::setTexture( const char * effectParameterName, GpuResourceHandle handle ) { return mImpl->setTexture( effectParameterName, handle ); }
GpuResourceHandle GN::gfx::ModelResource::getTexture( const char * effectParameterName ) const { return mImpl->getTexture( effectParameterName ); }
void              GN::gfx::ModelResource::setUniform( const char * effectParameterName, GpuResourceHandle handle ) { return mImpl->setUniform( effectParameterName, handle ); }
GpuResourceHandle GN::gfx::ModelResource::getUniform( const char * effectParameterName ) const { return mImpl->getUniform( effectParameterName ); }
void              GN::gfx::ModelResource::setMesh( GpuResourceHandle mesh, const MeshResourceSubset * subset ) { mImpl->setMesh( mesh, subset ); }
//GpuResourceHandle GN::gfx::ModelResource::getMesh( MeshResourceSubset * subset ) const;
void              GN::gfx::ModelResource::draw() const { mImpl->draw(); }
