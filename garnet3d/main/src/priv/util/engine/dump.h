#ifndef __GN_ENGINE_DUMP_H__
#define __GN_ENGINE_DUMP_H__
// *****************************************************************************
//! \file    engine/dump.h
//! \brief   Engine dump code
//! \author  chenli@@FAREAST (2007.5.17)
// *****************************************************************************

#include "resourceCommandBuffer.h"

///
/// macro to switch engine dump
///
#define GN_ENGINE_DUMP_ENABLED (GN_BUILD_VARIANT != GN_RETAIL_BUILD)

namespace GN { namespace engine
{
    ///
    /// TBD
    ///
    inline void dumpString( const StrA & text )
    {
        static Logger * sLogger = getLogger("GN.engine.dump");
        GN_DETAIL(sLogger)( "%s", text.cptr() );
    }

    ///
    /// TBD
    ///
    inline void dumpResourceCommand( const ResourceCommand & cmd )
    {
        const char * opstr;
        switch( cmd.op )
        {
            case GROP_LOAD       : opstr = "LOAD"; break;
            case GROP_DECOMPRESS : opstr = "DECOMPRESS"; break;
            case GROP_COPY       : opstr = cmd.resource->data ? "COPY" : "CRETE and COPY"; break;
            case GROP_DISPOSE    : opstr = "DISPOSE"; break;
            default              : GN_UNEXPECTED(); opstr = ""; break;
        }

        dumpString( strFormat( "<ResourceCommand op=\"%s\" %s/>",
            opstr,
            cmd.resource->desc.toString().cptr() ) );
    }
}}

// *****************************************************************************
//                           End of dump.h
// *****************************************************************************
#endif // __GN_ENGINE_DUMP_H__
