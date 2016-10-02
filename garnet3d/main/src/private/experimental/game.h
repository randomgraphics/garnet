#include <garnet/GNbase.h>

namespace GN
{
    namespace engine
    {
        /// Root class of game asset. Could be texture, mesh, or sound.
        class Asset : public NoCopy
        {
        };

        /// Texture used in game.
        class TextureAsset : public Asset
        {
        public:

            Texture * sLoadFromFile(...);
            Texture * sCreateBlankTexture(...);
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

        class Entity : public NoCopy
        {
        };

        /// Static mesh in the world
        /// It has physical, visual nodes
        class StaticMesh : public Entity
        {
        };

        /// Light entity. It has physical node
        class Light : public Entity
        {
        };

        class CoordinateIndicator : public Entity
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
        bool initialize();
        void shutdown();

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
    }
}
