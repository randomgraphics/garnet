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
    class VisualComponent : public Component
    {
    public:

        //@{

        /// Get visual component's type ID.
        static const Guid & sGetType();

        /// get component type ID
        virtual const Guid & getType() const { return sGetType(); }

        /// Construct an empty visual component
        VisualComponent();

        /// destructor
        virtual ~VisualComponent();

        /// clear to empty component
        void clear();

        /// add new model to the node. return the model ID, or 0 for failure.
        /// Note: models cannot be shared between components, since the component needs to setup
        /// model's uniforms to per-component value.
        int addModel( gfx::ModelResource * model );

        /// Render all models in the component
        void draw() const;

        /// Render the component to screen with specified transformation.
        void draw( const Matrix44f & proj, const Matrix44f & view ) const
        {
            getGdb()->setTransform( proj, view );
            draw();
        }

        //@}

    protected:

        /// This method is called for each model in the visual component. The default
        /// implementation just calls ModelResource::draw(). Subclass could override
        /// this function to do custom rendering for each model.
        virtual void drawModelResource( gfx::ModelResource & model ) const
        {
            model.draw();
        }

    private:

        typedef HandleManager<AutoRef<gfx::ModelResource>,int> ModelManager;

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
