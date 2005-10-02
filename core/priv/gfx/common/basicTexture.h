#ifndef __GN_GFXCOMMON_BASICTEXTURE_H__
#define __GN_GFXCOMMON_BASICTEXTURE_H__
// *****************************************************************************
//! \file    basicTexture.h
//! \brief   贴图基类
//! \author  chenlee (2005.10.1)
// *****************************************************************************

namespace GN { namespace gfx
{
    //!
    //! 贴图类的基类
    //!
    class basicTexture : public Texture
    {
        // ********************************
        //! \name constructor
        // ********************************

        //@{
    protected :
        basicTexture() : mLocked( false ) {}
        //@}

        // ********************************
        //  public functions
        // ********************************
    public:

        //!
        //! texture is being locked or not.
        //!
        bool isLocked() const { return mLocked; }

        // ********************************
        //  protected functions
        // ********************************
    protected:

        //!
        //! Basic lock operation. Each lock function must call this function first,
        //! to ensure the lock operation is valid.
        //!
        bool baseLock()
        {
            if( !isLocked() )
            {
                mLocked = 1;
                return true;
            }
            else
            {
                GN_ERROR( "dupilcate lock! lock/unlock() "
                         "must be called accordinglly!" );
                return false;
            }
        }

        //!
        //! Basic unlock operation. Should be called in pair with baseLock().
        //!
        bool baseUnlock()
        {
            if( isLocked() )
            {
                mLocked = 0;
                return true;
            }
            else
            {
                GN_ERROR( "dupilcate unlock! lock/unlock() "
                         "must be called accordinglly!" );
                return false;
            }
        }

        // ********************************
        //  private variables
        // ********************************
    private:

        //!
        //! 贴图的锁定标志
        //!
        bool mLocked;
    };
}}

// *****************************************************************************
//                           End of basicTexture.h
// *****************************************************************************
#endif // __GN_GFXCOMMON_BASICTEXTURE_H__
