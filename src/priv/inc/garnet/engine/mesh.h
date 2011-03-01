#ifndef __GN_ENGINE_MESH_H__
#define __GN_ENGINE_MESH_H__
// *****************************************************************************
/// \file
/// \brief   Mesh entities
/// \author  chen@@chenli-homepc (2011.2.28)
// *****************************************************************************

namespace GN { namespace engine
{
    ///
    /// Static mesh entity
    ///
    class StaticMesh : public Entity
    {
    public:

        /// constructor
        StaticMesh();

        /// destructor
        ~StaticMesh();

        /// get the spacial component
        const SpacialComponent & spacial() const { return mSpacial; }

        /// get the spacial component
        SpacialComponent & spacial() { return mSpacial; }

        /// get the visual component
        const VisualComponent & visual() const { return mVisual; }

        /// get the visual component
        VisualComponent & visual() { return mVisual; }

        /// Load all models from the file as a single static mesh
        bool loadAllModelsFromFile( const char * fileName );

        /// Draw the mesh
        void draw( const Matrix44f & proj, const Matrix44f & view ) const
        {
            engine::getStandardUniformManager()->setTransform( proj, view );
            mVisual.draw();
        }

    private:

        template<class REF_COUNTED_CLASS>
        class StackRefCounter : public REF_COUNTED_CLASS
        {
        public:

            virtual ~StackRefCounter()
            {
                decref();
            }

        protected:

            virtual void selfDestruct() const
            {
                // do nothing here.
            }
        };

        StackRefCounter<SpacialComponent> mSpacial;
        StackRefCounter<VisualComponent>  mVisual;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE_MESH_H__
