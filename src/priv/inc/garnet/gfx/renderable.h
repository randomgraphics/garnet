#ifndef __GN_GFX_RENDERABLE_H__
#define __GN_GFX_RENDERABLE_H__
// *****************************************************************************
//! \file    gfx/renderable.h
//! \brief   basic renderable object
//! \author  chenli@@FAREAST (2007.1.30)
// *****************************************************************************

#include "garnet/GNgfx.h"
#include "garnet/gfx/effect.h"
#include "garnet/gfx/mesh.h"
#include <map>

namespace GN { namespace gfx
{
    ///
    /// basic renderable class
    ///
    struct Renderable : public RefCounter
    {
        struct TexItem
        {
            EffectItemID     id;
            AutoRef<Texture> tex;
        };

        struct Subset
        {
            AutoRef<Effect>      effect;
            AutoRef<Mesh>        mesh;
            std::vector<TexItem> textures;
        };

        std::vector<Subset> subsets;
        
    public:

        ///
        /// clear to empty
        ///
        void clear() { subsets.clear(); }

        ///
        /// draw specific subset
        ///
        void drawSubset( size_t i ) const;

        ///
        /// draw the whole renderable object
        ///
        void draw() const
        {
            for( size_t i = 0; i < subsets.size(); ++i )
            {
                drawSubset( i );
            }
        }

        ///
        /// load from XML.
        ///
        /// \param root
        ///     Root XML node that contains definition of the renderable object.
        /// \param basedir
        ///     Base resource directory. All relative subresource path will be resolved based on it.
        /// \param meshmgr, effmgr, texmgr
        ///     Subresoruce managers
        ///
        bool loadFromXml(
                const XmlNode                  * root,
                const StrA                     & basedir,
                ResourceManager<gfx::Mesh*>    & meshmgr,
                ResourceManager<gfx::Effect*>  & effmgr,
                ResourceManager<gfx::Texture*> & texmgr );

        ///
        /// load from XML file
        ///
        bool loadFromXmlFile(
                File                           & fp,
                const StrA                     & basedir,
                ResourceManager<gfx::Mesh*>    & meshmgr,
                ResourceManager<gfx::Effect*>  & effmgr,
                ResourceManager<gfx::Texture*> & texmgr );
    };
}}

// *****************************************************************************
//                           End of renderable.h
// *****************************************************************************
#endif // __GN_UTIL_RENDERABLE_H__
