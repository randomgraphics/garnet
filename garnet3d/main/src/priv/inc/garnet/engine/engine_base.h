#ifndef __GN_ENGINE_ENGINE_BASE_H__
#define __GN_ENGINE_ENGINE_BASE_H__
// *****************************************************************************
/// \file
/// \brief   Basic engine classes
/// \author  chen@@CHENLI-OLDPC (2011.2.24)
// *****************************************************************************

#include "standardUniforms.h"

namespace GN { namespace engine
{
    class Entity;

    /// Component class root. Can be attached to at most one entity at any moment.
    class Component : public RefCounter
    {
    protected:

        Component() : mEntity(NULL)
        {
        }

    public:

        virtual ~Component()
        {
        }

        Entity * getEntity() const { return mEntity; }

        virtual const Guid & getType() const = 0;

    private:

        friend class Entity;
        Entity * mEntity;
    };

    /// Entity class. Root class of game play object that could be placed into game world.
    class Entity : public NoCopy
    {
    protected:

        /// constructor
        Entity();

    public:

        /// destructor
        virtual ~Entity();

        /// Get entity ID
        int getID() const { return mID; }

        /// Get entity's component. No increasing reference counter of the component.
        Component * getComponent( const Guid & type ) const;

        /// Set entity's component.
        ///   - Decease reference counter of existing component by 1.
        ///   - Increase reference counter of the new component by 1.
        ///   - Null component pointer is allowed.
        void setComponent( const Guid & type, Component * comp );

        /// tempalte helpers
        //@{
        template<class T> T *  getComponent() const { return (T*)getComponent( T::sGetType() ); }
        template<class T> void setComponent( T * t ) { return setComponent( T::sGetType(), t ); }
        //@}

        //public: virtual void processEvent( uint32 eventid, uint64 param1, void * param2 ) = 0;

    private:

        typedef HashMap<Guid, Component*, Guid::Hash> ComponentMap;

        int           mID;
        ComponentMap  mComponents;
    };

    ///
    /// Managing standard uniforms
    ///
    class StandardUniformManager
    {
    public:

        //@{

        StandardUniformManager();
        ~StandardUniformManager();

        /// get pointer to specific standard uniform resource, assuming that it is initialized already.
        gfx::UniformResource * getGlobalUniformResource( StandardUniformType type ) const;

        void setGlobalUniform( StandardUniformType type, const void * data, size_t dataSize );

        template<typename T>
        void setGlobalUniform( StandardUniformType type, const T & value ) { setGlobalUniform( type, &value, sizeof(T) ); }

        void setTransform( const Matrix44f & proj, const Matrix44f & view );

        void setLight0(
                const Vector4f & diffuse,
                const Vector4f & ambient,
                const Vector4f & specular,
                const Vector3f & position,
                const Vector3f & direction );

        //@}

    private:

        AutoRef<gfx::UniformResource> mUniforms[StandardUniformType::NUM_STANDARD_UNIFORMS];
    };

    // --------------------------------------------------------------------------
    // Global engine functions
    // --------------------------------------------------------------------------

    //@{

    // Initialize basic systems (file, memory, thread and etc.)
    bool initialize();

    // shutdown everthing.
    void shutdown();

    /// Graphics initialization options
    struct GfxInitOptions
    {
        gfx::GpuOptions           gpuOptions;          ///< renderer options
        bool                      useMultithreadGpu;   ///< use multithread renderer or not.
        gfx::FontFaceCreationDesc defaultAsciiFont;    ///< default ascii font face creation descriptor.
        gfx::FontFaceCreationDesc defaultNonAsciiFont; ///< default non-ascii font face creation descriptor
    };

    bool inputInitialize( input::InputAPI );
    void inputShutdown();

    bool gfxInitialize( const GfxInitOptions & );
    void gfxShutdown();

    //bool soundInitialize();
    //void soundShutdown();

    //bool networkInitialize();
    //void networkShutdown();

    // Graphics stuff
    gfx::Gpu                 * getGpu();
    gfx::SpriteRenderer      * getSpriteRenderer();
    gfx::LineRenderer        * getLineRenderer();
    gfx::BitmapFont          * getDefaultFontRenderer();
    gfx::GpuResourceDatabase * getGdb();
    StandardUniformManager   * getStandardUniformManager();

    // Entity management
    Entity * findEntity( int id );

    /// Functions in this namespace are reserved for engine module itself,
    /// and should not be called by engine clients or entity/component developers.
    namespace impl
    {
        /// Entity management
        //@{
        int  onEntityCtor( Entity * );
        void onEntityDtor( int );
        //@}
    }

    //@}
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE_ENGINE_BASE_H__
