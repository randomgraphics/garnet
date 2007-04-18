// inline methods of Effect class

//
//
// -----------------------------------------------------------------------------
GN_INLINE size_t GN::gfx::Effect::getNumPasses() const
{
    GN_GUARD_SLOW;
    GN_ASSERT( mTechniques.items.validHandle(mActiveTechnique) );
    return mTechniques.items[mActiveTechnique].passes.size();
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::Effect::passBegin(
    RendererContext & ctx, size_t passIdx ) const
{
    GN_GUARD_SLOW;

    GN_ASSERT( !mPassBegun );
    mPassBegun = true;
    mActiveContext = &ctx;
    mActivePass = passIdx;

    GN_ASSERT( mTechniques.items.validHandle(mActiveTechnique) );
    TechniqueData & t = mTechniques.items[mActiveTechnique];

    const PassData & p = t.passes[mActivePass];

    // update renderer context
    ctx.setRenderStateBlock( p.rsb );
    for( size_t i = 0; i < NUM_SHADER_TYPES; ++i )
    {
        GN_ASSERT( mShaders.items.validHandle( p.shaders[i] ) );

        ShaderData & sd = mShaders.items[p.shaders[i]];

        // bind shader
        ctx.setShader( (ShaderType)i, sd.value );

        // apply texture/sampler properties
        for( size_t iTexture = 0; iTexture < sd.textures.size(); ++iTexture )
        {
            const TextureRefData & trd = sd.textures[iTexture];
            GN_ASSERT( mTextures.items.validHandle(trd.id) );
            const TextureData & td = mTextures.items[trd.id];
            ctx.setTexture( trd.stage, td.value );
        }

        // apply dirty uniforms
        for( size_t iUniform = 0; iUniform < sd.dirtyUniforms.size(); ++iUniform )
        {
            GN_ASSERT( sd.dirtyUniforms[iUniform] < sd.uniforms.size() );
            const UniformRefData & urd = sd.uniforms[sd.dirtyUniforms[iUniform]];
            const UniformData & ud = mUniforms.items[urd.id];
            if( urd.ffp )
            {
                sSetFfpUniform( ctx, urd.ffpType, ud );
            }
            else
            {
                GN_ASSERT( urd.shaderUniformHandle );
                sd.value->setUniform( urd.shaderUniformHandle, ud.value );
            }
        }
        sd.dirtyUniforms.clear();
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::Effect::commitChanges() const
{
    GN_GUARD_SLOW;

    GN_ASSERT( mPassBegun && mActiveContext );

    GN_ASSERT( mTechniques.items.validHandle(mActiveTechnique) );
    TechniqueData & t = mTechniques.items[mActiveTechnique];

    const PassData & p = t.passes[mActivePass];

    // for each shader
    for( size_t iShader = 0; iShader < NUM_SHADER_TYPES; ++iShader )
    {
        GN_ASSERT( mShaders.items.validHandle( p.shaders[iShader] ) );

        ShaderData & sd = mShaders.items[p.shaders[iShader]];

        // apply texture/sampler properties
        for( size_t iTexture = 0; iTexture < sd.textures.size(); ++iTexture )
        {
            const TextureRefData & trd = sd.textures[iTexture];
            GN_ASSERT( mTextures.items.validHandle(trd.id) );
            const TextureData & td = mTextures.items[trd.id];
            mActiveContext->setTexture( trd.stage, td.value );
        }

        // apply dirty uniforms again, in case user may change uniforms between passBegin() and commitChanges()
        for( size_t iUniform = 0; iUniform < sd.dirtyUniforms.size(); ++iUniform )
        {
            GN_ASSERT( sd.dirtyUniforms[iUniform] < sd.uniforms.size() );
            const UniformRefData & urd = sd.uniforms[sd.dirtyUniforms[iUniform]];
            const UniformData & ud = mUniforms.items[urd.id];
            if( urd.ffp )
            {
                sSetFfpUniform( *mActiveContext, urd.ffpType, ud );
            }
            else
            {
                GN_ASSERT( urd.shaderUniformHandle );
                sd.value->setUniform( urd.shaderUniformHandle, ud.value );
            }
        }
        sd.dirtyUniforms.clear();
    }

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// uniform management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_INLINE bool GN::gfx::Effect::hasTechnique( const StrA & name, EffectItemID * id ) const
{
    EffectItemID i = mTechniques.find( name );
    if( id ) *id = i;
    return 0 != i;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE GN::gfx::EffectItemID GN::gfx::Effect::getTechniqueID( const StrA & name ) const
{
    GN_GUARD_SLOW;
    EffectItemID id = mTechniques.find( name );
    if( 0 == id ) GN_ERROR(sLogger)( "invalid technique name: %s", name.cptr() );
    return id;
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::Effect::setActiveTechnique( EffectItemID id ) const
{
    GN_GUARD_SLOW;

    if( !ok() )
    {
        GN_ERROR(sLogger)( "Uninitialized effect class." );
        return;
    }

    if( 0 == id )
    {
        // use the first technique.
        mActiveTechnique = mTechniques.items.first();
    }
    else if( !mTechniques.items.validHandle( id ) )
    {
        GN_ERROR(sLogger)( "Invalid technique id: %d.", id );
    }
    else
    {
        mActiveTechnique = id;
    }
    GN_ASSERT( mTechniques.items.validHandle( mActiveTechnique ) );

    GN_UNGUARD_SLOW;
}


//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::Effect::setActiveTechniqueByName( const StrA & name ) const
{
    GN_GUARD_SLOW;
    EffectItemID id = getTechniqueID( name );
    if( 0 == id ) return;
    setActiveTechnique( id );
    GN_UNGUARD_SLOW;
}


// *****************************************************************************
// uniform management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_INLINE bool GN::gfx::Effect::hasUniform( const StrA & name, EffectItemID * id ) const
{
    EffectItemID i = mUniforms.find( name );
    if( id ) *id = i;
    return 0 != i;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE GN::gfx::EffectItemID GN::gfx::Effect::getUniformID( const StrA & name ) const
{
    GN_GUARD_SLOW;
    EffectItemID id = mUniforms.find( name );
    if( 0 == id ) GN_ERROR(getLogger("GN.gfx.base.Effect.invalidParam"))( "invalid uniform name: %s", name.cptr() );
    return id;
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::Effect::setUniform( EffectItemID id, const UniformValue & value ) const
{
    GN_GUARD_SLOW;

    if( !mUniforms.items.validHandle(id) )
    {
        GN_ERROR(getLogger("GN.gfx.base.Effect.invalidParam"))( "invalid uniform id: %d", id );
        return;
    }

    UniformData & u = mUniforms.items[id];
    u.value = value;

    for( size_t i = 0; i < u.shaders.size(); ++i )
    {
        ShaderRefData & srd = u.shaders[i];
        ShaderData & sd = mShaders.items[srd.id];

        // avoid redunant index in dirty array
        // TODO: use sorted-array to increase search speed.
        if( sd.dirtyUniforms.end() == std::find( sd.dirtyUniforms.begin(), sd.dirtyUniforms.end(), srd.index ) )
        {
            sd.dirtyUniforms.push_back( srd.index );
        }
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::Effect::setUniformByName( const StrA & name, const UniformValue & value ) const
{
    GN_GUARD_SLOW;
    EffectItemID id = getUniformID( name );
    if( 0 == id ) return;
    setUniform( id, value );
    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// texture management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_INLINE bool GN::gfx::Effect::hasTexture( const StrA & name, EffectItemID * id ) const
{
    EffectItemID i = mTextures.find( name );
    if( id ) *id = i;
    return 0 != i;
}


//
//
// -----------------------------------------------------------------------------
GN_INLINE GN::gfx::EffectItemID GN::gfx::Effect::getTextureID( const StrA & name ) const
{
    GN_GUARD_SLOW;
    EffectItemID id = mTextures.find( name );
    if( 0 == id ) GN_ERROR(getLogger("GN.gfx.base.Effect.invalidParam"))( "invalid texture name: %s", name.cptr() );
    return id;
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::Effect::setTexture( EffectItemID id, const Texture * tex ) const
{
    GN_GUARD_SLOW;

    if( !mTextures.items.validHandle(id) )
    {
        GN_ERROR(getLogger("GN.gfx.base.Effect.invalidParam"))( "invalid texture id: %d", id );
        return;
    }

    mTextures.items[id].value = tex;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::Effect::setTextureByName( const StrA & name, const Texture * tex ) const
{
    GN_GUARD_SLOW;
    EffectItemID id = getTextureID( name );
    if( 0 == id ) return;
    setTexture( id, tex );
    GN_UNGUARD_SLOW;
}