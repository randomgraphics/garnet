#ifndef __GN_BASE_REGISTRY_H__
#define __GN_BASE_REGISTRY_H__
// *****************************************************************************
//! \file    registry.h
//! \brief   tree-like registry structure.
//! \author  chenlee (2005.6.15)
// *****************************************************************************

#include <map>

namespace GN
{
    //!
    //! ◊¢≤·±Ì¿‡–Õ.
    //!
    class Registry
    {
        std::map<StrA,Variant> mKeys;

    public:

        //!
        //! Constructor
        //!
        Registry() {}

        //!
        //! Destructor
        //!
        ~Registry() {}

        //!
        //! Reset to empty registry.
        //!
        void reset() { mKeys.clear(); }

        //!
        //! Get value of specific registry key
        //!
        //! \return
        //!     Return NULL if the key does not exist.
        //!
        const Variant * getKey( const StrA & name ) const;

        //!
        //! Get value of specific registry key
        //!
        //! \return
        //!     Return default value if the key is not exist.
        //!
        const Variant & getKey( const StrA & name, const Variant & defaultValue ) const;

        //!
        //! Create a new key.
        //!
        //! \return
        //!     Return false, if the key exist and overwriteExisting is false.
        //!
        bool setKey( const StrA & name, const Variant & value, bool overwriteExisting = true );
    };
}

// *****************************************************************************
//                           End of registry.h
// *****************************************************************************
#endif // __GN_BASE_REGISTRY_H__
