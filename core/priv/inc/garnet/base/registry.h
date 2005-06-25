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
    //! 可变类型
    //!
    class Variant
    {
        bool   mValid;
        double mNumber;
        StrA   mStr;

        //!
        //! Update string value based on boolean value
        //!
        void updateBoolStr( bool b )
        {
            mStr = b ? "yes" : "no";
        }

        //!
        //! Update string value based on numberic value
        //!
        void updateStr()
        {
            mStr.format( "%f", mNumber );
        }

        //!
        //! Update numeric value based on string value
        //!
        void updateNumber()
        {
            if ( 0 == strCmpI( "yes", mStr.cstr() ) )
                mNumber = 1.0;
            else if ( 0 == strCmpI( "no", mStr.cstr() ) )
                mNumber = 0.0;
            else
            {
                float r;
                if( 1 == ::sscanf( mStr.cstr(), "%f", &r ) )
                {
                    mNumber = r;
                }
            }
        }

    public:

        //! \name Constructors
        //@{
        Variant() : mValid(false) {}
        Variant( const Variant & var ) : mValid(var.mValid), mNumber(var.mNumber), mStr(var.mStr) {}
        Variant( bool var ) : mValid(true), mNumber(var) { updateBoolStr(var); }
        Variant( int var ) : mValid(true), mNumber(var) { updateStr(); }
        Variant( unsigned int var ) : mValid(true), mNumber(var) { updateStr(); }
        Variant( float var ) : mValid(true), mNumber(var) { updateStr(); }
        Variant( double var ) : mValid(true), mNumber(var) { updateStr(); }
        Variant( const StrA & var ) : mValid(true), mStr(var) { updateNumber(); }
        //@}

        //!
        //! Contain valid value or not?
        //!
        bool isValid() const { return mValid; }

        //! \name Get variant value
        //@{
        operator bool() const           { GN_ASSERT(mValid); return !!mNumber; }
        operator int() const            { GN_ASSERT(mValid); return (int)mNumber; }
        operator unsigned int() const   { GN_ASSERT(mValid); return (unsigned int)mNumber; }
        operator float() const          { GN_ASSERT(mValid); return (float)mNumber; }
        operator double() const         { GN_ASSERT(mValid); return mNumber; }
        operator const StrA &() const   { GN_ASSERT(mValid); return mStr; }
        //@}

        //! \name Set variant value
        //@{
        Variant & operator = ( const Variant & newVal ) { mValid = newVal.mValid; mNumber = newVal.mNumber; mStr = newVal.mStr; return *this; }
        Variant & operator = ( bool newVal )            { mValid = true; mNumber = newVal; updateBoolStr(newVal); return *this; }
        Variant & operator = ( int newVal )             { mValid = true; mNumber = newVal; updateStr(); return *this; }
        Variant & operator = ( unsigned int newVal )    { mValid = true; mNumber = newVal; updateStr(); return *this; }
        Variant & operator = ( float newVal )           { mValid = true; mNumber = newVal; updateStr(); return *this; }
        Variant & operator = ( double newVal )          { mValid = true; mNumber = newVal; updateStr(); return *this; }
        Variant & operator = ( const StrA & newVal )    { mValid = true; mStr = newVal; updateNumber(); return *this; }
        //@}
    };
    
    //!
    //! 注册表类型.
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

