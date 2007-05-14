#ifndef __GN_ENGINE_DRAWABLE_H__
#define __GN_ENGINE_DRAWABLE_H__
// *****************************************************************************
//! \file    engine/drawable.h
//! \brief   Drawable class
//! \author  chen@@CHENLI-HOMEPC (2007.2.21)
// *****************************************************************************

#include <map>

namespace GN { namespace engine
{
    ///
    /// drawable object.
    ///
    class Drawable
    {
        DrawContext     context;

    public:

        ///

        ///
        /// texture item
        ///
        struct TexItem
        {
            EffectItemID       binding; ///< effect item ID that this texture is binding to.
            Entity *           texture; ///< texture entity ID
        };

        ///
        /// uniform item
        ///
        struct UniItem
        {
            EffectItemID      binding; ///< effect item ID that this uniform is binding to.
            gfx::UniformValue value;   ///< uniform value.
        };
        //@}

        //@{
        Entity *               effect;
        Entity *               mesh;
        std::map<StrA,TexItem> textures;
        std::map<StrA,UniItem> uniforms;
        //@}

        ///
        /// clear to empty
        ///
        void clear()
        {
            effect = 0;
            mesh = 0;
            textures.clear();
            uniforms.clear();
            context.clearToNull();
        }

        ///
        /// is empty drawable or not
        ///
        bool empty() const { return 0 == effect || 0 == mesh; }

        ///
        /// load drawable from XML
        ///
        bool loadFromXmlNode( const XmlNode & node, const StrA & basedir );

        //@{

        bool hasUniform( const StrA & name ) const { return uniforms.end() != uniforms.find( name ); }

        //@}

        //@{

        void draw()
        {
            if( 0 == effect || 0 == mesh ) return;

            Effect * eff = entity2Object<Effect*>( effect );
            GN_ASSERT( eff );

            for( size_t i = 0; i < eff->getNumPasses(); ++i )
            {
                eff->passBegin( context, i );
                drawPass();
                eff->passEnd();
            }
        }

        void drawPass()
        {
            if( 0 == effect || 0 == mesh ) return;

            Effect * eff = entity2Object<Effect*>( effect );
            GN_ASSERT( eff );
    
            // bind textures
            BindTexture bt(eff);
            std::for_each( textures.begin(), textures.end(), bt );

            // bind uniforms
            BindUniform bu(eff);
            std::for_each( uniforms.begin(), uniforms.end(), bu );

            // bind mesh
            Mesh * m = entity2Object<Mesh*>( mesh );
            GN_ASSERT( m );
            m->updateContext( context );

            // commit changes
            eff->commitChanges();

            // apply context to renderer
            gRenderer.setContext( context );

            // do draw
            m->draw();
        }

        //@}

    private:

        struct BindTexture
        {
            Effect * eff;
            BindTexture( Effect * eff_ ) : eff(eff_) {}
            void operator()( const std::pair<StrA,TexItem> & i ) const
            {
                GN_ASSERT( i.second.binding );
                eff->setTexture( i.second.binding, i.second.texture );
            }
        };

        struct BindUniform
        {
            Effect * eff;
            BindUniform( Effect * eff_ ) : eff(eff_) {}
            void operator()( const std::pair<StrA,UniItem> & i ) const
            {
                GN_ASSERT( i.second.binding );
                eff->setUniform( i.second.binding, i.second.value );
            }
        };
    };
}}

// *****************************************************************************
//                           End of drawable.h
// *****************************************************************************
#endif // __GN_ENGINE_DRAWABLE_H__
