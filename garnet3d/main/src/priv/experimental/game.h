#include <garnet/garnet.h>

namespace GN
{
    namespace game
    {
        /// Root class of game asset. Could be texture, mesh, or sound.
        class Asset
        {
        };

        /// Texture used in game.
        class Texture : public Asset
        {
        };

        /// Graphics effect
        class GfxEffect : public Asset
        {
        };

        class WavSound : public Asset
        {
        };

        /// Root class of gameplay object that could be placed into game world.
        class Entity : public NoCopy
        {
        };

        /// Static mesh in the wolrd
        class StaticMesh : public Entity
        {
        };

        /// Camera
        class Camera : public Entity
        {
        };

        class CoordinateIndicator : public NoCopy
        {
        };

        /// One game level
        class Level
        {
        public:

            void preload() = 0;
        };

        /// The root of the game world
        class World
        {
        public:

            bool Initialize();
            void Shutdown();

            bool GfxInitialize();
            void GfxShutdown();

            bool InputInitialize();
            void InputShutdown();

            bool SoundInitialize();
            void SoundShutdown();

            bool NetworkInitialize();
            void NetworkShutdown();

        public:

            // Graphics stuff
            GN::gfx::Gpu                 * getGpu() const;
            GN::gfx::GpuResourceDatabase * getGdb() const;
            GN::gfx::SpriteRenderer      * getSpriteRenderer() const;
            GN::gfx::BitmapFont          * getBmfRenderer() const;
        };
    }
}
