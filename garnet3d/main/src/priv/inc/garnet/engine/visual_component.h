#ifndef __GN_ENGINE_GFX_COMPONENT_H__
#define __GN_ENGINE_GFX_COMPONENT_H__
// *****************************************************************************
/// \file
/// \brief   Graphics components
/// \author  chenli@@REDMOND (2011.2.28)
// *****************************************************************************

namespace GN { namespace engine
{
    ///
    /// Component that renders object onto screen
    ///
    class VisualComponent : public Entity
    {
        GN_ENGINE_DECLARE_ENTITY( VisualComponent, Entity );

    public:

        //@{

        /// Construct an empty visual component
        VisualComponent();

        /// destructor
        virtual ~VisualComponent();

        /// clear to empty component
        void clear();

        /// add new model to the node.
        /// Note: models cannot be shared between components, since the visual component need to
        /// setup model's uniforms to per-component value.
        bool addModel( gfx::ModelResource * model );

        /// Render all models in the component. If an spcial component is provided, it will be
        /// used to update the standard transformation uniforms.
        void draw( const SpacialComponent * sc ) const;

        /// Render the component to screen with specified transformation.
        void draw( const Matrix44f & proj, const Matrix44f & view, const SpacialComponent * sc ) const
        {
            getGdb()->setTransform( proj, view );
            draw( sc );
        }

        //@}

    protected:

        /// This method is called for each model in the visual component. The default
        /// implementation just calls ModelResource::draw(). Subclass could override
        /// this function to do custom rendering for each model.
        virtual void drawModelResource( uint32 modelIndex, gfx::ModelResource & model ) const
        {
            GN_UNUSED_PARAM(modelIndex);
            model.draw();
        }

    private:

        typedef DynaArray<AutoRef<gfx::ModelResource>,uint32> ModelManager;

        typedef FixedArray<
            AutoRef<gfx::UniformResource>,
            gfx::StandardUniform::Index::NUM_STANDARD_UNIFORMS
            > StandardUniformArray;

        ModelManager               mModels;
        StandardUniformArray       mStandardPerObjectUniforms;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE_GFX_COMPONENT_H__
