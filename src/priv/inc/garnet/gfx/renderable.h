#ifndef __GN_GFX_RENDERABLE_H__
#define __GN_GFX_RENDERABLE_H__
// *****************************************************************************
//! \file    gfx/renderable.h
//! \brief   basic renderable object
//! \author  chenli@@FAREAST (2007.1.30)
// *****************************************************************************

#include "garnet/GNgfx.h"
#include "garnet/gfx/effect.h"
#include "garnet/gfx/fatmesh.h"
#include <map>

namespace GN { namespace gfx
{
    ///
    /// renderable subset descriptor (single mesh and effect pair)
    ///
    struct RenderableSubsetDesc
    {
        StrA                effect;   ///< effect name
        StrA                mesh;     ///< mesh name
        std::map<StrA,StrA> textures; ///< textures. Key is texture ID in effect; value is texture resource name.
    };

    ///
    /// renderable descriptor
    ///
    struct RenderableDesc
    {
        std::vector<RenderableSubsetDesc> subsets;

        void clear() { subsets.clear(); }
    };

    ///
    /// basic renderable class
    ///
    class Renderable : public StdClass
    {
        GN_DECLARE_STDCLASS( Renderable, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        Renderable()          { clear(); }
        virtual ~Renderable() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init(
                const RenderableDesc &,
                ResourceManager<gfx::FatMesh*> *,
                ResourceManager<gfx::Effect*> *,
                ResourceManager<gfx::Texture*> * );
        void quit();
    private:
        void clear()
        {
            mDesc.clear();
        }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        size_t getSubsetCount() const { return mSubsets.size(); }

        UInt32 getEffectHandle( size_t subset ) const { GN_ASSERT( subset < mSubsets.size() ); return mSubsets[subset].effect; }

        void draw() const { for( size_t i = 0; i < mSubsets.size(); ++i ) { drawSubset( i ); } }

        void drawSubset( size_t i ) const;

        // ********************************
        // private variables
        // ********************************
    private:

        struct TexItem
        {
            EffectItemID id;
            UInt32       handle;
        };

        struct Subset
        {
            UInt32 effect;
            UInt32 mesh;
            std::vector<TexItem> textures;
        };

        RenderableDesc mDesc;

        std::vector<Subset> mSubsets;

        ResourceManager<gfx::FatMesh*> * mMeshMgr;
        ResourceManager<gfx::Effect*>  * mEffectMgr;
        ResourceManager<gfx::Texture*> * mTextureMgr;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of renderable.h
// *****************************************************************************
#endif // __GN_UTIL_RENDERABLE_H__
