#ifndef __GN_ENGINE_DRAWCOMMAND_H__
#define __GN_ENGINE_DRAWCOMMAND_H__
// *****************************************************************************
//! \file    engine/drawCommand.h
//! \brief   ?
//! \author  chen@@CHENLI-HOMEPC (2007.5.1)
// *****************************************************************************

namespace GN { namespace engine
{
    ///
    /// draw command type
    ///
    enum DrawCommandType
    {
        //@{
        DCT_SET_CONTEXT,
        DCT_CLEAR,
        DCT_DRAW,
        DCT_DRAW_INDEXED,
        NUM_DRAW_COMMAND_TYPES
        //@}
    };

    ///
    ///
    ///
    struct DrawCommand
    {
        enum
        {
            ///
            /// maximum resources used per context
            ///
            MAX_RESOURCES_PER_CONTEXT = 64 * gfx::NUM_SHADER_TYPES + gfx::MAX_RENDER_TARGETS + 1
        };

        ///
        /// ...
        ///
        struct ResourceWaitingItem
        {
            ///
            /// ...
            ///
            GraphicsResourceId id;

            ///
            /// the draw must wait for resource update that happens at this fence.
            ///
            FenceId waitForUpdate;
        };

        DrawCommandType type;    ///< see DrawCommandType for details.

        FenceId         fence;   ///< fence ID of this draw

        DrawContext     context; ///< draw context

        //@{
        ResourceWaitingItem resourceWaitingList[MAX_RESOURCES_PER_CONTEXT];
        UInt32              resourceWaitingCount;
        //@}

        //@{
        union
        {
            struct
            {
                //@{
                float            r, g, b, a;
                float            z;
                UInt8            s;
                BitFields        flags;
                Vector4f       & color() { return *(Vector4f*)&r; }
                const Vector4f & color() const { return *(Vector4f*)&r; }
                //@}
            } clear; ///< clear parameters

            struct
            {
                //@{
                gfx::PrimitiveType primtype;
                UInt32             numprim;
                UInt32             startvtx;
                //@}
            } draw; ///< draw parameters

            struct
            {
                //@{
                gfx::PrimitiveType primtype;
                UInt32             numprim;
                UInt32             startvtx;
                UInt32             minvtxidx;
                UInt32             numvtx;
                UInt32             startidx;
                //@}
            } drawindexed; ///< drawindexed parameters
        };
        //@}
    };
}}

// *****************************************************************************
//                           End of drawCommand.h
// *****************************************************************************
#endif // __GN_ENGINE_DRAWCOMMAND_H__
