#ifndef __GN_BASE_DEBUGVAR_H__
#define __GN_BASE_DEBUGVAR_H__
// *****************************************************************************
/// \file
/// \brief   interface of runtime adjustable debug variable
/// \author  chen@@CHENLI-HOMEPC (2007.4.18)
// *****************************************************************************

namespace GN {
// predefine
class DebugVarManager;

///
/// Unique debug varialbe ID
///
typedef uint32 DebugVariableId;

///
/// base class of debug variable
///
class BasicDebugVariable {
    DebugVarManager & mManager;
    DebugVariableId   mId; // unique ID of the variable
    const std::string        mName;
    const std::string        mType;
    DynaArray<uint8>  mValue;

public:
    //@{

    BasicDebugVariable(size_t size, const std::string & name, const std::string & type);

    BasicDebugVariable(const BasicDebugVariable &);

    //@}

    //@{

    DebugVariableId getId() const { return mId; }

    size_t getSize() const { return mValue.size(); }

    const void * getValue() const { return mValue.data(); }

    const std::string & getName() const { return mName; }

    const std::string & getType() const { return mType; }

    //@}
};

///
/// debug variable template
///
template<typename T>
class DebugVar : public BasicDebugVariable {
    T & mValue;

public:
    //@{

    DebugVar(const std::string & name = std::string::EMPTYSTR());

    DebugVar(const T &, const std::string & name = std::string::EMPTYSTR());

    //@}

    //@{

    operator const T &() const { return mValue; }

    DebugVar & operator=(const T &);

    //@}
};

DebugVar<bool>     b;
DebugVar<int>      i;
DebugVar<float>    f;
DebugVar<Vector3f> v;

///
/// debug variable manager
///
class DebugVarManager {
public:
    /// \name variable instance management. Used by BasicDebugVariable only. Do _NOT_ call them in your own code.
    //@{
    DebugVariableId registerVariable(BasicDebugVariable *);
    void            removeVarialbe(BasicDebugVariable *);
    //@}

    ///
    /// get variable by ID
    ///
    BasicDebugVariable * getVarById(DebugVariableId);

    /// \name iterate all variables
    //@{
    BasicDebugVariable * getVarByIndex(size_t index);
    size_t               getNumVariables() const;
    //@}
};
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_DEBUGVAR_H__
