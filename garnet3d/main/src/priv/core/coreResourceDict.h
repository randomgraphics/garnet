#ifndef __GN_CORE_CORERESOURCEDICT_H__
#define __GN_CORE_CORERESOURCEDICT_H__
// *****************************************************************************
//! \file    core/coreResourceDict.h
//! \brief   Resource dictionaries in core module
//! \author  chenlee (2006.2.11)
// *****************************************************************************

#include "garnet/GNcore.h"
#include "garnet/GNgfx.h"
#include "garnet/gfx/effect.h"

namespace GN { namespace core
{
    //!
    //! Search resoure file on disk.
    //!
    //! Return path of the resouce. Return empty string if not found.
    //!
    GN_PUBLIC StrA searchResourceFile( const StrA & name );

    //!
    //! Raw resource dictionary class of core module
    //!
    class CoreRawResourceDict : public StdClass
    {
        GN_DECLARE_STDCLASS( CoreRawResourceDict, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        CoreRawResourceDict()          { clear(); }
        virtual ~CoreRawResourceDict() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() { mDict.clear(); }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        // ********************************
        // private variables
        // ********************************
    private:

        RawResourceDictionary mDict;

        // ********************************
        // private functions
        // ********************************
    private:
    };

    //!
    //! Shader dictionary class of core module
    //!
    class CoreShaderDict : public StdClass
    {
        GN_DECLARE_STDCLASS( CoreShaderDict, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        CoreShaderDict()          { clear(); }
        virtual ~CoreShaderDict() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() { mDict.clear(); }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        // ********************************
        // private variables
        // ********************************
    private:

        gfx::ShaderDictionary mDict;

        // ********************************
        // private functions
        // ********************************
    private:
    };

    //!
    //! Texture dictionary class of core module
    //!
    class CoreTextureDict : public StdClass
    {
        GN_DECLARE_STDCLASS( CoreTextureDict, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        CoreTextureDict()          { clear(); }
        virtual ~CoreTextureDict() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() { mDict.clear(); }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        // ********************************
        // private variables
        // ********************************
    private:

        gfx::TextureDictionary mDict;

        // ********************************
        // private functions
        // ********************************
    private:
    };

    //!
    //! Core effect dictionary.
    //!
    class CoreEffectDict : public StdClass
    {
        GN_DECLARE_STDCLASS( CoreEffectDict, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        CoreEffectDict()          { clear(); }
        virtual ~CoreEffectDict() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() { mDict.clear(); }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        // ********************************
        // private variables
        // ********************************
    private:

        gfx::EffectDictionary mDict;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of coreResourceDict.h
// *****************************************************************************
#endif // __GN_CORE_CORERESOURCEDICT_H__
