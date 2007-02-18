#ifndef __GN_SCENE_GNSCENE_H__
#define __GN_SCENE_GNSCENE_H__
// *****************************************************************************
//! \file    garnet/GNscene.h
//! \brief   public header of scene module
//! \author  chenli@@FAREAST (2007.2.17)
// *****************************************************************************

#include "GNcore.h"
#include "GNgfx.h"
#include "GNutil.h"

namespace GN
{
    ///
    /// namespace for scene module
    ///
    namespace scene
    {
    }
}

namespace GN { namespace scene
{
    class Scene; // forward declaration

    typedef UInt32 NodeId; ///< scene node ID

    ///
    /// basic scene node
    ///
    class SceneNode
    {
        Scene  &     mScene;
        const NodeId mId;

    public:

        // *********************************************************************
        /// \name ctor / dtor
        // *********************************************************************

        //@{

    protected:

        ///
        /// constructor, with automatic generated id.
        ///
        SceneNode( Scene & s );

        ///
        /// constructor, with automatic generated id.
        ///
        SceneNode( Scene & s, NodeId id );

    public:

        ///
        /// destructor
        ///
        virtual ~SceneNode();

        //@}

        // *********************************************************************
        /// \name common node properties
        // *********************************************************************

        //@{

    public:

        ///
        /// get unique node ID
        ///
        NodeId getId() const { GN_ASSERT(mId>0); return mId; }

        ///
        /// get unique node name
        ///
        const StrA & getName() const;

        //@}

        // *********************************************************************
        /// \name node location
        // *********************************************************************

        //@{

    public:

        const Vector3 &    getPosition() const { return mPosition; }
        void               setPosition( const Vector3 & );

        const Quaternion & getOrientation() const { return mOrientation; }
        void               setOrientation( const Quaternion & );

        const Matrix44f &  getWorldMatrix() const { if( mRebuildWorldMatrix ) rebuildWorldMatrix(); return mWorldMatrix; }
        const Matrix44f &  getInvWorldMatrix() const { if( mRebuildWorldMatrix ) rebuildWorldMatrix(); return mInvWorldMatrix; }

    private:

        Vector3f          mPosition;
        Vector3f          mOrientation;

        mutable Matrix44f mWorldMatrix;
        mutable Matrix44f mInvWorldMatrix;
        mutable bool      mRebuildWorldMatrix;

    private:

        virtual void rebuildWorldMatrix() const;

        //@}

        // *********************************************************************
        /// \name animation
        // *********************************************************************

        //@{

    public:

        virtual void frameUpdate() = 0;
        virtual void draw() = 0;

        //@}

        // *********************************************************************
        /// \name serialization
        // *********************************************************************

        //@{

        ///
        /// save to XML
        ///
        virtual bool saveToXmlFile( File & ) const = 0;

        ///
        /// load from XML
        ///
        virtual bool loadFromXml( const XmlNode & ) const = 0;

        ///
        /// load from XML
        ///
        inline bool loadFromXmlFile( File & ) const
        {
            GN_GUARD;

            XmlDocument doc;
            XmlParseResult xpr;
            if( !doc.parse( xpr, fp ) )
            {
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
            return loadFromXml( *xpr.root );

            GN_UNGUARD;
        }

        //@}
    };

    ///
    /// Solid renderable object
    ///
    class Solid : public SceneNode
    {
        UInt32 renderable;

    public:

        // *********************************************************************
        /// \name ctor / dtor
        // *********************************************************************

        //@{

        ///
        /// ctor
        ///
        Solid( Scene & s ) : SceneNode(s) {}

        //@}


        // *********************************************************************
        /// \name from SceneNode
        // *********************************************************************

        //@{

        virtual void frameUpdate() {}
        virtual void draw() {}

        //@}

    };

    ///
    /// scene camera
    ///
    class Camera : public SceneNode
    {
    public:

        // *********************************************************************
        /// \name ctor / dtor
        // *********************************************************************

        //@{

        ///
        /// ctor
        ///
        Camera( Scene & s ) : SceneNode(s) {}

        //@}

        // *********************************************************************
        /// \name from SceneNode
        // *********************************************************************

        //@{

        virtual void frameUpdate() {}
        virtual void draw() {}

        //@}

        // *********************************************************************
        /// \name camera specific properties
        // *********************************************************************

        //@{

    public:

        const Matrix44f &  getViewMatrix() const { return getInvWorldMatrix(); }
        const Matrix44f &  getInvViewMatrix() const { return getWorldMatrix(); }

        const Rectf &      getViewport() const;
        void               setViewport( const Rectf & );

        const Matrix44f &  getProjMatrix() const;
        void               setProjMatrix( const Matrix44f & );

        //@}
    };

    ///
    /// camera contoller
    ///
    class CameraContoller : public SceneNode
    {
    };

    ///
    /// Terrain object
    ///
    class Terrain : public SceneNode
    {
    };

    ///
    /// Scene object. Represent a self-contained world.
    ///
    /// Note that there might be multiple instance of Scene object.
    ///
    class Scene
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        Scene();
        virtual ~Scene();
        //@}

        // ********************************
        // misc
        // ********************************

        //@{

        ///
        /// update the scene, with fixed time step
        ///
        void frameUpdate();

        //@}

        // ********************************
        // node instance management
        // ********************************
    public:

        //@{

        // These functions are called automatically by ctor/dtor of SceneNode.
        // Do not call them in your own code.
        NodeId addNode( SceneNode * );

    private:

        std::vector<SceneNode*> mNodes;

        //@}
    };

    ///
    /// scene resource manager. This is singleton.
    ///    
    class SceneResourceManager : public StdClass, public Singleton
    {
        GN_DECLARE_STDCLASS( SceneResourceManager, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        SceneResourceManager()          { clear(); }
        virtual ~SceneResourceManager() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        // ********************************
        // private variables
        // ********************************
    private:

        // ********************************
        // private functions
        // ********************************
    private:
    };

    ///
    /// backend thread that handles all tasks that need to touch renderer device.
    ///
    /// Note that this is a singleton.
    ///    
    class SceneRenderer : public StdClass, public Singleton<SceneRenderer>
    {
        GN_DECLARE_STDCLASS( SceneRenderer, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        SceneRenderer()          { clear(); }
        virtual ~SceneRenderer() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        void waitForIdle();

        // ********************************
        // private variables
        // ********************************
    private:

        // ********************************
        // private functions
        // ********************************
    private:
    };

    bool loadSceneFromFile( Scene & s, const StrA & filename );
}}

// *****************************************************************************
//                           End of GNscene.h
// *****************************************************************************
#endif // __GN_SCENE_GNSCENE_H__
