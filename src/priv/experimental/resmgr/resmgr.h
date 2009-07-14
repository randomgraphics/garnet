#ifndef __GN_SCENE_RESMGR_H__
#define __GN_SCENE_RESMGR_H__
// *****************************************************************************
/// \file
/// \brief   Resource manager used in scene model
/// \author  chenli@@REDMOND (2009.7.13)
// *****************************************************************************

namespace GN { namespace scene
{
    class ResourceManager
    {

    public:

        //@{

        struct ResourceClass : NoCopy
        {
            virtual bool load( const StrA & filename ) = 0;
            virtual void unload() = 0;
            virtual bool dump( const StrA & filename ) = 0;
        };

        typedef UInt32 ResourceHandle;

        //@}

    public:

        //@{

        ///
        /// ctor
        ///
        ResourceManager();

        ///
        /// dtor
        ///
        virtual ~ResourceManager();

        ///
        /// Clear the resource manager, delete all resources
        ///
        void clear();

        ///
        /// Return true for valid resource handle
        ///
        bool validResourceHandle( ResourceHandle ) const;

        ///
        /// Return true for valid resource name
        ///
        bool validResourceName( const StrA & ) const;

        ///
        /// Convert resource name to handle
        ///
        ResourceHandle getResourceHandle( const StrA & name );

        ///
        /// Convert resource handle to name
        ///
        const StrA & getResourceName( ResourceHandle handle );

        ///
        /// Acquire a reference to resource instance.
        ///
        /// Note: remember to release the resource after being used.
        ///
        ResourceClass & getResource( ResourceHandle );

        ///
        /// Remove resource out of the manager. Delete resource instance as well.
        ///
        void removeResource( ResourceHandle );

        ///
        /// Dispose/Unload resource instance from memory.
        ///
        /// Note that the resource itself is still keeped by the manager, and will
        /// be loaded into memory again when getResource() is called next time.
        ///
        void unloadResource( ResourceHandle );

        ///
        /// Unload all resource instances.
        ///
        void unloadAll();

        ///
        /// Preload all resources into memory
        ///
        bool preloadAll();

        //@}

        //@{
        RES & getResource( const StrA & );
        void  removeResource( const StrA & );
        void  unloadResource( const StrA & );
        //@}
    };

    ///
    /// graphics effect wrapper for resource management
    ///
    class EffectResource
    {
    public:

        //@{
        gfx::Effect * getEffect();
        //@}
    };

    ///
    /// graphics mesh wrapper for resource management
    ///
    class MeshResource
    {
    };

    ///
    /// Graphics resource manager (a singleton)
    ///
    class GraphicsResourceManager
    {
    public:

        //@{
        ResourceManagerTempl<gfx::Texture*>  textures;
        ResourceManagerTempl<gfx::Uniform*>  uniforms;
        ResourceManagerTempl<EffectResource> effects;
        ResourceManagerTempl<MeshResource>   meshes;
        //@}
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_RESMGR_H__
