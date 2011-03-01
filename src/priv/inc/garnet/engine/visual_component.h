#ifndef __GN_ENGINE_GFX_COMPONENT_H__
#define __GN_ENGINE_GFX_COMPONENT_H__
// *****************************************************************************
/// \file
/// \brief   Graphics components
/// \author  chenli@@REDMOND (2011.2.28)
// *****************************************************************************

#include "standardUniforms.h"

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
        int  addModel( gfx::ModelResource * model );

        /// load models from file, and add them to the component
        bool addModelsFromFile( const char * filename );

        /// Render the component to screen
        void draw() const;

        //@}

    private:

        typedef HandleManager<AutoRef<gfx::ModelResource>,int> ModelManager;

        typedef FixedArray<AutoRef<gfx::UniformResource>,StandardUniformType::NUM_STANDARD_UNIFORMS> StandardUniformArray;

        ModelManager               mModels;
        StandardUniformArray       mStandardPerObjectUniforms;

    private:

        gfx::UniformResource * getPerObjectUniform( StandardUniformType type );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE_GFX_COMPONENT_H__
