#ifndef __GN_BASE_DEBUGVAR_H__
#define __GN_BASE_DEBUGVAR_H__
// *****************************************************************************
//! \file    base/debugvar.h
//! \brief   interface of runtime adjustable debug variable
//! \author  chen@@CHENLI-HOMEPC (2007.4.18)
// *****************************************************************************

namespace GN
{
    // predefine
    class DebugVarManager;

    ///
    /// Unique debug varialbe ID
    ///
    UInt32 DebugVariableId;

    ///
    /// base class of debug variable
    ///
    class BasicDebugVariable
    {
        DebugVarManager & mManager;

        DebugVariableId mId;

        const StrA mType;

        const StrA mName;

        StrA       mValue;

    public:

        //@{

        BasicDebugVariable( const StrA & type, const StrA & name );

        BasicDebugVariable( const BasicDebugVariable & );

        //@}

        //@{

        DebugVariableId getId() const { return mId; }

        const StrA & getType() const { return mType; }

        const StrA & getName() const { return mName; }

        //@}

        //@{

        virtual void toString( StrA & ) const = 0; ///< get variable value as string
        virtual bool fromString( StrA & ) = 0; ///< set variable value as string.

        //@}
    };

    ///
    /// debug variable template
    ///
    template<typename T>
    class DebugVar : public BasicDebugVariable
    {
        T mValue;

    public:

        //@{

        DebugVar( const StrA & name = StrA::EMPTYSTR );

        DebugVar( const T &, const StrA & name = StrA::EMPTYSTR );

        //@}

        //@{

        operator const T & () const { return mValue; }

        DebugVar & operator =( const T & );

        //@}
    };

    ///
    /// debug variable manager (singleton)
    ///
    class DebugVarManager : public CrossDllSingleton<DebugVarManager>
    {
    public:

        /// \name variable instance management. Used by BasicDebugVariable only. Do _NOT_ call them in your own code.
        //@{
        DebugVariableId registerVariable( BasicDebugVariable * );
        void            removeVarialbe(  BasicDebugVariable * );
        //@}

        ///
        /// get variable by ID
        ///
        BasicDebugVariable * getVarById( DebugVariableId );

        /// \name iterate all variables
        //@{
        BasicDebugVariable * getVarByIndex( size_t index );
        size_t               getNumVariables() const;
        //@}
    };
}

// *****************************************************************************
//                           End of debugvar.h
// *****************************************************************************
#endif // __GN_BASE_DEBUGVAR_H__
