#ifndef __GN_CORE_GNCORE_H__
#define __GN_CORE_GNCORE_H__
// *****************************************************************************
//! \file    GNcore.h
//! \brief   Public interface of garnet core module.
//! \author  chenlee (2005.8.7)
// *****************************************************************************

#include "GNbase.h"

//!
//! global raw resource dictionary
//!
#define gRawResourceDict (GN::core::RawDataDictionary::sGetInstance())

//!
//! pointer to global raw resource dictionary
//!
#define gRawResourceDictPtr (GN::core::RawDataDictionary::sGetInstancePtr())

namespace GN
{
    /*namespace gfx
    {
        struct Renderer;
        class ShaderDictionary;
        class TextureDictionary;

        namespace effect
        {
            class EffectDictionary;
        }
    }

    namespace input
    {
        struct Input;
    }

    class Core
    {
    public:
        gfx::Renderer & getRenderer();
        gfx::Renderer * getRendererPtr();
        gfx::ShaderDictionary & getShaderDict();
        gfx::ShaderDictionary * getShaderDictPtr();
        gfx::TextureDictionary & getTextureDict();
        gfx::TextureDictionary * getTextureDictPtr();
        gfx::effect::EffectDictionary & getEffectDict();
        gfx::effect::EffectDictionary * getEffectDictPtr();
        input::Input & getInput();
        input::Input * getInputPtr();
    };*/

    //!
    //! namespace for garnet core module
    //!
    namespace core
    {
        //!
        //! Represent raw data block
        //!
        struct RawData : public NoCopy
        {
            //!
            //! get data size
            //!
            virtual size_t size() const = 0;

            //!
            //! get data pointer
            //!
            virtual void * data() const = 0;
        };

        //!
        //! Raw resource manager class (singleton)
        //!
        typedef ResourceManager<RawData*,true> RawDataDictionary;

        //!
        //! Global log signal
        //!
        extern GN_PUBLIC Signal2<void,const LogDesc &, const char *> gSigLog;
    }
}

// *****************************************************************************
//                           End of GNcore.h
// *****************************************************************************
#endif // __GN_CORE_GNCORE_H__
