#ifndef __GN_SCENE_QUAD_H__
#define __GN_SCENE_QUAD_H__
// *****************************************************************************
//! \file    scene/quad.h
//! \brief   Quad renderer
//! \author  chen@@CHENLI-HOMEPC (2007.3.13)
// *****************************************************************************

namespace GN { namespace scene
{
    ///
    /// screen aligned quad renderer
    ///
    class QuadRenderer : public Singleton<QuadRenderer>
    {
    public:

        //@{

        void drawBegin();
        void drawEnd();
        void draw();

        //@}
    };
}}

// *****************************************************************************
//                           End of quad.h
// *****************************************************************************
#endif // __GN_SCENE_QUAD_H__
