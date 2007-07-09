#ifndef __GN_GFX2_GFX2BASE_H__
#define __GN_GFX2_GFX2BASE_H__
// *****************************************************************************
/// \file
/// \brief   basic graphics system implementation
/// \author  chenli@@FAREAST (2007.6.15)
// *****************************************************************************

#include "GNgfx2.h"

namespace GN { namespace gfx
{
    class BaseKernel;
    class BaseGraphicsSystem;

    ///
    /// array of items with unique name
    ///
    template<typename T>
    class NamedArray
    {
        struct Item
        {
            StrA   name;
            size_t index;
            T      value;
        };

        StrA                  mTypeName;
        DynaArray<Item>       mItems;
        std::map<StrA,size_t> mNames;

        const T * getByIndex( size_t index ) const
        {
            if( index >= mItems.size() )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "%s index is out of range.", mTypeName.cptr() );
                return 0;
            }
            return &mItems[index].value;
        }

        T * getByIndex( size_t index )
        {
            if( index >= mItems.size() )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "%s index is out of range.", mTypeName.cptr() );
                return 0;
            }
            return &mItems[index].value;
        }

        const T * getByName( const StrA & name ) const
        {
            std::map<StrA,size_t>::const_iterator i = mNames.find( name );
            if( mNames.end() == i )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "invalid %s name '%s'.", mTypeName.cptr(), name.cptr() );
                return 0;
            }
            return &mItems[i->second].value;
        }

        T * getByName( const StrA & name )
        {
            std::map<StrA,size_t>::const_iterator i = mNames.find( name );
            if( mNames.end() == i )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "invalid %s name '%s'.", mTypeName.cptr(), name.cptr() );
                return 0;
            }
            return &mItems[i->second].value;
        }

        const StrA & getNameByIndex( size_t index ) const
        {
            if( index >= mItems.size() )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "%s index is out of range.", mTypeName.cptr() );
                return StrA::EMPTYSTR;
            }
            return mItems[index].name;
        }

        size_t getIndexByName( const StrA & name ) const
        {
            std::map<StrA,size_t>::const_iterator i = mNames.find( name );
            if( mNames.end() == i )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "invalid %s name '%s'.", mTypeName.cptr(), name.cptr() );
                return (size_t)-1;
            }
            return i->second;
        }

        size_t doAdd( const StrA & name, const T & value )
        {
            std::map<StrA,size_t>::const_iterator i = mNames.find( name );
            if( mNames.end() != i )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "%s named '%s' does exist already.", mTypeName.cptr() );
                return (size_t)-1;
            }

            Item item;
            item.name  = name;
            item.index = mItems.size();
            item.value = value;

            mNames[name] = mItems.size();
            mItems.append( item );

            return item.index;
        }

    public:

        //@{

        NamedArray( const char * typeName = "item" ) : mTypeName(typeName)
        {
        }

        ~NamedArray()
        {
        }

        bool         empty() const { return mItems.empty(); }
        size_t       size() const { GN_ASSERT( mItems.size() == mNames.size() ); return mItems.size(); }
        const T    * get( size_t index ) const { return getByIndex( index ); }
        const T    * get( const StrA & name ) const { return getByName( name ); }
        const StrA & getName( size_t index ) const { return getNameByIndex( index ); }
        size_t       getIndex( const StrA & name ) const { return getIndexByName( name ); }
        size_t       add( const StrA & name, const T & value ) { return doAdd( name, value ); }

        ///
        /// this function does not allow invalid index
        ///
        const T    & at( size_t index ) const { GN_ASSERT( index < size() ); return mItems[index].value; }

        ///
        /// this function does not allow invalid name
        ///
        const T    & at( const StrA & name ) const { std::map<StrA,size_t>::const_iterator i = mNames.find(name); GN_ASSERT( mNames.end() != i ); return mItems[i->second].value; }

        //@}
    };

    ///
    /// base kernel class
    ///
    class BaseKernel : public Kernel
    {
    };

    ///
    /// D3D9 graphics system
    ///
    class BaseGraphicsSystem : public GraphicsSystem, public StdClass
    {
        GN_DECLARE_STDCLASS( BaseGraphicsSystem, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        BaseGraphicsSystem()          { clear(); }
        virtual ~BaseGraphicsSystem() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // from GraphicsSystem
        // ********************************
    public:

        //@{

        virtual void     registerKernelFactory( const StrA & name, KernelFactory factory, int quality );
        virtual Kernel * getKernel( const StrA & name );
        virtual void     deleteAllKernels();

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        struct KernelItem
        {
            KernelFactory factory;
            int           quality;
            Kernel *      instance;
        };

        NamedHandleManager<KernelItem,UInt32> mKernels;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_GFX2BASE_H__
