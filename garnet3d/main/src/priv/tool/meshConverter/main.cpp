#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.tool.meshConverter");

static const char * inputFile = "media::\\boxes\\boxes.ase";
static const char * outputFile = "app::boxes.scene.xml";

///
/// write world description to file
// -----------------------------------------------------------------------------
bool GN::util::SimpleWorldDesc::saveToFile( const char * filename )
{
    // check dirname
    if( NULL == filename )
    {
        GN_ERROR(sLogger)( "NULL directory name" );
        return false;
    }

    // convert to full path
    StrA fullpath = fs::resolvePath( fs::getCurrentDir(), filename );
    filename = fullpath;
    StrA dirname = fs::dirName( fullpath );

    if( !fs::isDir( dirname ) )
    {
        GN_ERROR(sLogger)( "%s is not a directory", dirname.cptr() );
        return false;
    }

    const SimpleWorldDesc & desc = *this;

    // write meshes
    int meshindex = 0;
    std::map<StrA,StrA> meshNameMapping;
    for( std::map<StrA,MeshResourceDesc>::const_iterator i = desc.meshes.begin();
        i != desc.meshes.end();
        ++i )
    {
        const StrA & oldMeshName = i->first;
        const MeshResourceDesc & mesh = i->second;

        StrA newMeshName = strFormat( "%d.mesh.bin", meshindex );

        if( !mesh.saveToFile( dirname + "\\" + newMeshName ) ) return false;

        meshNameMapping[oldMeshName] = newMeshName;

        ++meshindex;
    }

    // create a new XML document
    XmlDocument xmldoc;
    XmlNode * root = xmldoc.createNode(XML_ELEMENT);

    // write models
    for( size_t i = 0; i < desc.models.size(); ++i )
    {
        ModelResourceDesc model = desc.models[i];

        std::map<StrA,StrA>::iterator iter = meshNameMapping.find( model.meshResourceName );
        if( iter != meshNameMapping.end() )
        {
            model.meshResourceName = iter->second;
        }

        if( !model.saveToXmlNode( *root, dirname ) ) return false;
    }

    // TODO: write entities

    // write XML document
    AutoObjPtr<File> fp( fs::openFile( filename, "wt" ) );
    if( !fp ) return false;
    return xmldoc.writeToFile( *fp, *root, false );
}

//
// Main entry
// -----------------------------------------------------------------------------
int main( int argc, const char * argv[] )
{
    printf( "\nGarnet mesh converter V0.1.\n" );

    GN_UNUSED_PARAM( argc );
    GN_UNUSED_PARAM( argv );

    SimpleWorldDesc swd;

    GN_INFO(sLogger)( "Load from file     : %s", inputFile );
    if( !swd.loadFromFile( inputFile ) ) return -1;

    GN_INFO(sLogger)( "Write to directory : %s", outputFile );
    if( !swd.saveToFile( outputFile ) ) return -1;

    GN_INFO(sLogger)( "Conversion done successfully." );
    return 0;
}
