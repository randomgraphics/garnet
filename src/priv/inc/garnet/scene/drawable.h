#ifndef __GN_SCENE_DRAWABLE_H__
#define __GN_SCENE_DRAWABLE_H__
// *****************************************************************************
//! \file    scene/drawable.h
//! \brief   Drawable class
//! \author  chen@@CHENLI-HOMEPC (2007.2.21)
// *****************************************************************************

namespace GN { namespace scene
{
    struct Drawable
    {
        //@{
        UInt32                                        effect;
        UInt32                                        mesh;
        std::map<gfx::EffectItemID,UInt32>            textures;
        std::map<gfx::EffectItemID,gfx::UniformValue> uniforms;
        //@}

        ///
        /// load drawable from XML
        ///
        bool loadFromXmlNode( const XmlNode & node, const StrA & basedir );

        ///
        /// load drawable from XML file
        ///
        bool loadFromXmlFile( File & fp, const StrA & basedir );

        //@{

        void draw()
        {
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
            ResourceManager & rm = ResourceManager::sGetInstance();
            gfx::Renderer   & r  = gRenderer;

            gfx::Effect * eff = rm.getResourceT<gfx::Effect>( effect );
            GN_ASSERT( eff );
    
            // bind textures
            //std::for_each( textures.begin(), textures.end(), ... );

            // bind uniforms
            //std::for_each( uniforms.begin(), uniforms.end(), ... );

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
    };
}}

// *****************************************************************************
//                           End of drawable.h
// *****************************************************************************
#endif // __GN_SCENE_DRAWABLE_H__
