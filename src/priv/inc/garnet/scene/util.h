#ifndef __GN_SCENE_UTIL_H__
#define __GN_SCENE_UTIL_H__
// *****************************************************************************
/// \file
/// \brief   Scene utilities
/// \author  chen@@CHENLI-HOMEPC (2007.4.1)
// *****************************************************************************

namespace GN { namespace scene
{
    ///
    /// load something from XML file
    ///
    template<class T>
    inline bool loadFromXmlFile( T & t, File & fp, const StrA & basedir )
    {
        GN_GUARD;

        XmlDocument doc;
        XmlParseResult xpr;
        if( !doc.parse( xpr, fp ) )
        {
            static Logger * sLogger = getLogger( "GN.scene.util" );
            GN_ERROR(sLogger)(
                "Fail to parse XML file (%s):\n"
                "    line   : %d\n"
                "    column : %d\n"
                "    error  : %s",
                fp.name().cptr(),
                xpr.errLine,
                xpr.errColumn,
                xpr.errInfo.cptr() );
            return false;
        }
        GN_ASSERT( xpr.root );
        return t.loadFromXmlNode( *xpr.root, basedir );

        GN_UNGUARD;
    }

    ///
    /// load something from XML file
    ///
    template<class T>
    inline bool loadFromXmlFile( T & t, const StrA & filename )
    {
        GN_GUARD;

        GN_INFO(getLogger( "GN.scene.util" ))( "Load '%s'", filename.cptr() );

        AutoObjPtr<File> fp( core::openFile( filename, "rt" ) );
        if( !fp ) return false;

        StrA basedir = dirName( filename );

        return loadFromXmlFile( t, *fp, basedir );

        GN_UNGUARD;
    }
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_UTIL_H__
