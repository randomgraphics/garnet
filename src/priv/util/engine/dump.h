#ifndef __GN_ENGINE_DUMP_H__
#define __GN_ENGINE_DUMP_H__
// *****************************************************************************
/// \file
/// \brief   Engine dump code
/// \author  chenli@@FAREAST (2007.5.17)
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
        StrA mApiName;

    public:

        /// get current ident
        static int & sGetIdent()
        {
            static int i = 1;
            return i;
        }

        /// ctor
        RenderEngineApiDumper( const char * api ) : mApiName(api)
        {
            GN_ASSERT( !mApiName.empty() );
            dumpApiString( strFormat( "<%s>", api ) );
            ++sGetIdent();
        }

        /// dtor
        ~RenderEngineApiDumper()
        {
            GN_ASSERT( sGetIdent() > 0 );
            --sGetIdent();
            dumpApiString( strFormat( "</%s>", mApiName.cptr() ) );
        }
    };

    ///
    /// dump API text
    ///
    inline void dumpApiString( const StrA & text )
    {
        StrA ident;
        for( int i = 0; i < RenderEngineApiDumper::sGetIdent(); ++i ) ident += '\t';

        GN_DETAIL(getLogger("GN.engine.dump.api"))( "%s%s", ident.cptr(), text.cptr() );
    }

    ///
    /// dump command status
    ///
    inline void dumpCommandString( const StrA & text )
    {
        GN_DETAIL(getLogger("GN.engine.dump.command"))( "%s", text.cptr() );
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
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE_DUMP_H__
