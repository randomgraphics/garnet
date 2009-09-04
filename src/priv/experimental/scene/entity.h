#ifndef __GN_SCENE_ENTITY_H__
#define __GN_SCENE_ENTITY_H__
// *****************************************************************************
/// \file
/// \brief   SceneManager entity classes
/// \author  chen@@CHENLI-HOMEPC (2009.3.29)
// *****************************************************************************

namespace GN { namespace scene
{
    class SceneManager;

    ///
    /// basic scene entity class
    ///
    class SceneEntity
    {
    public:

        enum Type
        {
            GEOMETRY,
        };

        /// dtor
        virtual ~SceneEntity() {}

        /// get scene
        SceneManager & getSceneManager() const { return mSceneManager; }

        /// get type
        Type getType() const { return mType; }

    protected:

        /// ctor
        SceneEntity( SceneManager & s, Type t ) : mSceneManager( s ), mType(t) {}

    private:

        SceneManager & mSceneManager;
        const Type     mType;
    };

    ///
    /// Geometry Entity
    ///
    class GeometryEntity : public SceneEntity
    {
        struct GeometryBlock
        {
            gfx::Effect              effect;
            DynaArray<gfx::Drawable> drawables;

            GeometryBlock( gfx::Renderer & r ) : effect(r) {}
        };

        struct StandardUniform
        {
            StandardSceneParameterType type;
            AutoRef<gfx::Uniform>      uniform;
        };

        DynaArray<StandardUniform>    mStandardPerObjectUniforms; ///< standard per-object parameters
        DynaArray<GeometryBlock*>     mBlocks;
        SpatialSceneGraph::Node       mTransform;

    public:

        /// ctor
        GeometryEntity( SceneManager & s );

        /// dtor
        virtual ~GeometryEntity();

        /// add new geometry block to the node
        void addModel( const gfx::Effect * effect, const gfx::GpuMesh * mesh, const gfx::GpuMeshSubset * subset = NULL );

        /// get number of geometry blocks
        size_t getNumGeometryBlocks() const { return mBlocks.size(); }

        /// get specific geometry block
        const GeometryBlock & getGeometryBlock( size_t i ) const { return *mBlocks[i]; }

        /// get specific geometry block
        GeometryBlock & getGeometryBlock( size_t i ) { return *mBlocks[i]; }

        /// get the transformation
        const SpatialSceneGraph::Node & getTransform() const { return mTransform; }

        /// get the transformation
        SpatialSceneGraph::Node & getTransform() { return mTransform; }

        /// draw the geometry
        void draw();
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_ENTITY_H__
