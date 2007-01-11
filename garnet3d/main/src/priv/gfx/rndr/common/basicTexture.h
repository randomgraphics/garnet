#ifndef __GN_GFXCOMMON_BASICTEXTURE_H__
#define __GN_GFXCOMMON_BASICTEXTURE_H__
// *****************************************************************************
/// \file    basicTexture.h
/// \brief   贴图基类
/// \author  chenlee (2005.10.1)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// 贴图类的基类
    ///
    class BasicTexture : public Texture
    {
        // ********************************
        /// \name constructor
        // ********************************

        //@{
    protected :
        BasicTexture() : mLocked( false ) {}
        //@}

        // ********************************
        //  public functions
        // ********************************
    public:

        ///
        /// texture is being locked or not.
        ///
        bool isLocked() const { return mLocked; }

        // ********************************
        //  protected functions
        // ********************************
    protected:

        ///
        /// Basic lock operation. Each lock function must call this function first,
        /// to ensure the lock operation is valid.
        ///
        /// This function will also clip lock area to ensure it is not out of texture size.
        ///
        bool basicLock( size_t face, size_t level, const Boxi * area, LockFlag flag, Boxi & clippedArea )
        {
            if( isLocked() )
            {
                GN_ERROR(sLogger)( "dupilcate lock! lock/unlock() must be called accordinglly!" );
                return false;
            }

            // check face
            if( face >= getDesc().faces )
            {
                GN_ERROR(sLogger)( "invalid lock face : %d", face );
                return false;
            }

            // check level
            if( level >= getDesc().levels )
            {
                GN_ERROR(sLogger)( "invalid lock level : %d", level );
                return false;
            }

            // check flag
            if( flag >= NUM_LOCK_FLAGS )
            {
                GN_ERROR(sLogger)( "invalid lock flag : %d", flag );
                return false;
            }

            // get texture size
            Vector3<UInt32> sz = getMipSize( level );

            // make sure lock area is valid
            if( area )
            {
                clippedArea = *area;
                if( !sAdjustOffsetAndRange( clippedArea.x, clippedArea.w, (int)sz.x ) ||
                    !sAdjustOffsetAndRange( clippedArea.y, clippedArea.h, (int)sz.y ) ||
                    !sAdjustOffsetAndRange( clippedArea.z, clippedArea.d, (int)sz.z ) )
                    return false;
            }
            else
            {
                clippedArea.x = 0;
                clippedArea.y = 0;
                clippedArea.z = 0;
                clippedArea.w = (int)sz.x;
                clippedArea.h = (int)sz.y;
                clippedArea.d = (int)sz.z;
            }
            
            // success            
            mLocked = 1;
            return true;
        }

        ///
        /// Basic unlock operation. Should be called in pair with baseLock().
        ///
        bool basicUnlock()
        {
            if( isLocked() )
            {
                mLocked = 0;
                return true;
            }
            else
            {
                GN_ERROR(sLogger)( "dupilcate unlock! lock/unlock() must be called accordinglly!" );
                return false;
            }
        }

        // ********************************
        //  private variables
        // ********************************
    private:

        ///
        /// 贴图的锁定标志
        ///
        bool mLocked;

        static Logger * sLogger;

        // ********************************
        //  private functions
        // ********************************
    private:

        static inline bool
        sAdjustOffsetAndRange( int & offset, int & length, int maxLength )
        {
            if( offset >= maxLength )
            {
                GN_ERROR(sLogger)( "offset is beyond the end of valid range." );
                return false;
            }
            if( 0 == length ) length = maxLength;
            if( offset + length > maxLength ) length = maxLength - offset;
            return true;
        }
    };
}}

// *****************************************************************************
//                           End of basicTexture.h
// *****************************************************************************
#endif // __GN_GFXCOMMON_BASICTEXTURE_H__
