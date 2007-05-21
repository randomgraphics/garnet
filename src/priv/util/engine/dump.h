#ifndef __GN_ENGINE_DUMP_H__
#define __GN_ENGINE_DUMP_H__
// *****************************************************************************
//! \file    engine/dump.h
//! \brief   Engine dump code
//! \author  chenli@@FAREAST (2007.5.17)
// *****************************************************************************

#include "resourceCommandBuffer.h"

///
/// macro to switch render engine command dump
///
#define GN_RENDER_ENGINE_COMMAND_DUMP_ENABLED GN_DEBUG_BUILD

///
/// macro to switch render engine API dump (includes everyting that directly called by render engine API)
///
#define GN_RENDER_ENGINE_API_DUMP_ENABLED GN_DEBUG_BUILD

namespace GN { namespace engine
{
    inline void dumpApiString( const StrA & text );

    ///
    /// TBD
    ///
    class RenderEngineApiDumper
    {
        static StrA ident;

        StrA mApiName;

    public:

        /// get current ident
        static const char * getIdent() { return ident.cptr(); }

        /// ctor
        RenderEngineApiDumper( const char * api ) : mApiName(api)
        {
            GN_ASSERT( !mApiName.empty() );

            dumpApiString( strFormat( "<%s>", api ) );

            ident += '\t';
        }

        /// dtor
        ~RenderEngineApiDumper()
        {
            GN_ASSERT( !ident.empty() );
            ident.remove( ident.size() - 1 );

            dumpApiString( strFormat( "</%s>", mApiName.cptr() ) );
        }
    };

    ///
    /// dump API text
    ///
    inline void dumpApiString( const StrA & text )
    {
        static Logger * sLogger = getLogger("GN.engine.dump.api");

        GN_DETAIL(sLogger)( "%s%s", RenderEngineApiDumper::getIdent(), text.cptr() );
    }

    ///
    /// dump command status
    ///
    inline void dumpCommandString( const StrA & text )
    {
        static Logger * sLogger = getLogger("GN.engine.dump.command");
        GN_DETAIL(sLogger)( "%s", text.cptr() );
    }

    ///
    /// dump resource command
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

        dumpCommandString( strFormat( "<ExecuteResourceCommand op=\"%s\" fence=\"%d\" %s/>",
            opstr,
            cmd.submittedAtThisFence,
            cmd.resource->desc.toString().cptr() ) );
    }

    ///
    /// dump postponed resource command
    ///
    inline void dumpPostponedResourceCommand( const ResourceCommand & cmd )
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

        dumpCommandString( strFormat( "<PostponeResourceCommand op=\"%s\" fence=\"%d\" wait_for_resource_fence=\"%d\" wait_for_draw_fence=\"%d\" %s/>",
            opstr,
            cmd.submittedAtThisFence,
            cmd.mustAfterThisResourceFence,
            cmd.mustAfterThisDrawFence,
            cmd.resource->desc.toString().cptr() ) );
    }
}}

// *****************************************************************************
//                           End of dump.h
// *****************************************************************************
#endif // __GN_ENGINE_DUMP_H__
