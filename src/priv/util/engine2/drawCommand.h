#ifndef __GN_ENGINE2_DRAWCOMMAND_H__
#define __GN_ENGINE2_DRAWCOMMAND_H__
// *****************************************************************************
/// \file
/// \brief   ?
/// \author  chen@@CHENLI-HOMEPC (2007.5.1)
// *****************************************************************************

#include "fence.h"

namespace GN { namespace engine
{
    ///
    /// draw command type
    ///
    enum DrawCommandType
    {
        //@{
        DCT_DRAW,
        DCT_PRESENT,
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
            /// draw will wait for at least 3 resources: 1 kernel, 1 parameter set and 1 binding
            ///
            MAX_RESOURCES_PER_DRAW = 256
        };

        ///
        /// command type.
        ///
        DrawCommandType type;

        ///
        /// bytes of the command, including this header
        ///
        UInt32 bytes;

        ///
        /// fence ID of this draw
        ///
        FenceId fence;

        //@{
        FixedArray<ResourceWaitingItem,MAX_RESOURCES_PER_DRAW> resourceWaitingList;
        UInt32                                                 resourceWaitingCount;
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
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE2_DRAWCOMMAND_H__
