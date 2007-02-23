#ifndef __GN_SCENE_DRAWABLE_H__
#define __GN_SCENE_DRAWABLE_H__
// *****************************************************************************
//! \file    scene/drawable.h
//! \brief   Drawable class
//! \author  chen@@CHENLI-HOMEPC (2007.2.21)
// *****************************************************************************

#include <map>

namespace GN { namespace scene
{
    ///
    /// drawable object.
    ///
    struct Drawable
    {
        ///
        /// texture item
        ///
        struct TexItem
        {
            gfx::EffectItemID binding; ///< effect item ID that this texture is binding to.
            ResourceId        texid;   ///< texture resource ID.
        };

        ///
        /// uniform item
        ///
        struct UniItem
        {
            gfx::EffectItemID binding; ///< effect item ID that this uniform is binding to.
            gfx::UniformValue value;   ///< uniform value.
        };
        //@}
        
        //@{
        ResourceId             effect;
        ResourceId             mesh;
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
        }

        ///
        /// load drawable from XML
        ///
        bool loadFromXmlNode( const XmlNode & node, const StrA & basedir );

        ///
        /// load drawable from XML file
        ///
        bool loadFromXmlFile( File & fp, const StrA & basedir );

        ///
        /// load drawable from XML file
        ///
        bool loadFromXmlFile( const StrA & filename );

        ///
        /// set uniform value by name
        ///

        //@{

        void draw()
        {
            if( 0 == effect || 0 == mesh ) return;
            
            ResourceManager & rm = ResourceManager::sGetInstance();
            gfx::Effect * eff = rm.getResourceT<gfx::Effect>( effect );
            GN_ASSERT( eff );

            for( size_t i = 0; i < eff->getNumPasses(); ++i )
            {
                eff->passBegin( i );
                drawPass();
                eff->passEnd();
            }
        }

        void drawPass()
        {
            if( 0 == effect || 0 == mesh ) return;

            ResourceManager & rm = ResourceManager::sGetInstance();
            gfx::Renderer   & r  = gRenderer;

            gfx::Effect * eff = rm.getResourceT<gfx::Effect>( effect );
            GN_ASSERT( eff );
    
            // bind textures
            BindTexture bt(eff);
            std::for_each( textures.begin(), textures.end(), bt );

            // bind uniforms
            BindUniform bu(eff);
            std::for_each( uniforms.begin(), uniforms.end(), bu );

            // bind mesh
            gfx::Mesh * m = rm.getResourceT<gfx::Mesh>( mesh );
            GN_ASSERT( m );
            m->updateContext( r );

            // commit changes
            eff->commitChanges();

            // do draw
            m->draw( r );
        }

        //@}

    private:

        struct BindTexture
        {
            gfx::Effect * eff;
            BindTexture( gfx::Effect * eff_ ) : eff(eff_) {}
            void operator()( const std::pair<StrA,TexItem> & i ) const
            {
                eff->setTexture(
                    i.second.binding,
                    gSceneResMgr.getResourceT<gfx::Texture>(i.second.texid) );
            }
        };

        struct BindUniform
        {
            gfx::Effect * eff;
            BindUniform( gfx::Effect * eff_ ) : eff(eff_) {}
            void operator()( const std::pair<StrA,UniItem> & i ) const
            {
                eff->setUniform( i.second.binding, i.second.value );
            }
        };
    };
}}

// *****************************************************************************
//                           End of drawable.h
// *****************************************************************************
#endif // __GN_SCENE_DRAWABLE_H__
