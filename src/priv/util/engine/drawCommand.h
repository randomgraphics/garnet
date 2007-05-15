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
        DCT_SET_UNIFORM,
        DCT_CLEAR,
        DCT_DRAW,
        DCT_DRAW_INDEXED,
        NUM_DRAW_COMMAND_TYPES
        //@}
    };

    ///
    /// function prototype to handle draw commands
    ///
    typedef void (*DrawFunction)( RenderEngine & engine, const void * param, size_t paramBytes );

    ///
    /// TBD
    ///
    struct DrawCommandHeader
    {
        ///
        /// TBD
        ///
        struct ResourceWaitingItem
        {
            ///
            /// TBD
            ///
            GraphicsResourceItem * resource;

            ///
            /// the draw must wait for resource update that happens at this fence.
            ///
            FenceId waitForUpdate;
        };

        enum
        {
            ///
            /// maximum resources used per context
            ///
            MAX_RESOURCES_PER_CONTEXT = 64 * gfx::NUM_SHADER_TYPES + gfx::MAX_RENDER_TARGETS + 1
        };

        ///
        /// bytes of the command, including this header
        ///
        UInt32 bytes;

        ///
        /// fence ID of this draw
        ///
        FenceId fence;

        ///
        /// function pointer that handles the command.
        ///
        DrawFunction func;

        //@{
        ResourceWaitingItem resourceWaitingList[MAX_RESOURCES_PER_CONTEXT];
        UInt32              resourceWaitingCount;
        //@}

        ///
        /// return pointer to parameter array
        ///
        void * param() const
        {
            return (void*)( this + 1 );
        }

        ///
        /// return pointer to next command
        ///
        DrawCommandHeader * next() const
        {
            return (DrawCommandHeader*)( ((UInt8*)this) + bytes );
        }
    };

    /*
    /// TBD
    ///
    struct DrawCommand
    {
        DrawCommandType type;    ///< see DrawCommandType for details.


        //@{
        ResourceWaitingItem resourceWaitingList[MAX_RESOURCES_PER_CONTEXT];
        UInt32              resourceWaitingCount;
        //@}

        DrawContext     context; ///< draw context, for DCT_SET_CONTEXT only

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
    };*/
}}

// *****************************************************************************
//                           End of drawCommand.h
// *****************************************************************************
#endif // __GN_ENGINE_DRAWCOMMAND_H__
