#include <garnet/GNbase.h>

namespace GN
{
    namespace engine
    {
        class World;








        /// Root class of game asset. Could be texture, mesh, or sound.
        class Asset : public NoCopy
        {
        };

        /// Texture used in game.
        class TextureAsset : public Asset
        {
        public:

            Texture * sLoadFromFile( World &, ...);
            Texture * sCreateBlankTexture( World &, ...);
        };

        class EffectAsset : public Asset
        {
        };

        // combination of effect, texture and other parameters
        // Completly determines now a surface is rendered.
        class MaterialAsset : public Asset
        {
        };

        class TriangleMeshAsset : public Asset
        {
        };

        class WavSoundAsset : public Asset
        {
        };






        class Component : public NoCopy
        {
            protected: Component() {}
            public: virtual ~Component() {}
            public: virtual const Guid & getType() const = 0;
        };

        class PhysicalComponent : public Component
        {
        public:

            static const Guid & sGetType();

            const Vector4 & getScale() const;
            const Vector4 & getPosition() const;
            const Vector4 & getRotation() const;
        };

        class VisualComponent : public Component
        {
            public: static const Guid & sGetType();
        };

        /// Root class of game play object that could be placed into game world.
        class Entity : public NoCopy
        {
            protected:       Entity( World & );
            public: virtual ~Entity();

            public:    Component * GetComponent( const Guid & type );
            protected: void        AddComponent( const Guid & type, Component & );
            protected: void        DelComponent( const Guid & type );

            //public: virtual void processEvent( uint32 eventid, uint64 param1, void * param2 ) = 0;
        };

        /// Static mesh in the wolrd
        /// It has physical, visual nodes
        class EStaticMesh : public Entity
        {
        };

        /// Light entity. It has physical node
        class ELight : public Entity
        {
        };

        class CoordinateIndicator : public Entity
        {
        };

        class Light : public Entity
        {
        };

        /// One game level
        class Level : public Entity
        {
        };

        /// coordinateindicator. It has physical and visual node
        class CoordinateIndicator : public Entity
        {
        };

        /// the game world (singleton)
        /// Use C style interface to hide implementation details.
        namespace world
        {
            bool initialize();
            void shutdown();

            // Initialize basic systems (file, memory, thread and etc.)
            bool Initialize();

            // shutdown everthing.
            void Shutdown();

            bool inputInitialize();
            void inputShutdown();

            bool soundInitialize();
            void soundShutdown();

            bool networkInitialize();
            void networkShutdown();

            // Graphics stuff
            GN::gfx::Gpu                 * getGpu() const;
            GN::gfx::GpuResourceDatabase * getGdb() const;
            GN::gfx::SpriteRenderer      * getSpriteRenderer() const;
            GN::gfx::BitmapFont          * getBmfRenderer() const;
        };
    }
}
