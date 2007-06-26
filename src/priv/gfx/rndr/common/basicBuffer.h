#ifndef __GN_GFX_BASICBUFFER_H__
#define __GN_GFX_BASICBUFFER_H__
// *****************************************************************************
/// \file
/// \brief   Basic vertex/index buffer class
/// \author  chenlee (2006.3.8)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// Basic vertex buffer class
    ///
    class BasicVtxBuf : public VtxBuf
    {
        // ********************************
        /// \name constructor
        // ********************************

        //@{
    protected :
        BasicVtxBuf() : mLocked( false ) {}
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
        /// This function will also clip lock bytes into valid range.
        ///
        bool basicLock( size_t offset, size_t & bytes, LockFlag flag )
        {
            if( isLocked() )
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common.BasicVtxBuffer");
                GN_ERROR(sLogger)( "dupilcate lock! lock/unlock() must be called accordinglly!" );
                return false;
            }

            const VtxBufDesc & desc = getDesc();

            if( offset >= desc.bytes )
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common.BasicVtxBuffer");
                GN_ERROR(sLogger)( "offset is beyond the end of vertex buffer!" );
                return false;
            }

            if( flag >= NUM_LOCK_FLAGS )
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common.BasicVtxBuffer");
                GN_ERROR(sLogger)( "invalid lock flag: %d!", flag );
                return false;
            }

            // adjust offset and bytes
            if( 0 == bytes ) bytes = desc.bytes;
            if( offset + bytes > desc.bytes ) bytes = desc.bytes - offset;

            // success            
            mLocked = true;
            return true;
        }

        ///
        /// Basic unlock operation. Should be called in pair with baseLock().
        ///
        bool basicUnlock()
        {
            if( isLocked() )
            {
                mLocked = false;
                return true;
            }
            else
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common.BasicVtxBuffer");
                GN_ERROR(sLogger)( "dupilcate unlock! lock/unlock() must be called accordinglly!" );
                return false;
            }
        }

        // ********************************
        //  private variables
        // ********************************
    private:

        ///
        /// 锁定标志
        ///
        bool mLocked;

        // ********************************
        //  private functions
        // ********************************
    private:
    };

    ///
    /// Basic index buffer class
    ///
    class BasicIdxBuf : public IdxBuf
    {
        // ********************************
        /// \name constructor
        // ********************************

        //@{
    protected :
        BasicIdxBuf() : mLocked( false ) {}
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
        /// This function will also clip numiIdx into valid range.
        ///
        bool basicLock( size_t startidx, size_t & numidx, LockFlag flag )
        {
            if( isLocked() )
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common.BasicIdxBuffer");
                GN_ERROR(sLogger)( "dupilcate lock! lock/unlock() must be called accordinglly!" );
                return false;
            }

            const IdxBufDesc & desc = getDesc();

            if( startidx >= desc.numidx )
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common.BasicIdxBuffer");
                GN_ERROR(sLogger)( "startidx is beyond the end of index buffer!" );
                return false;
            }

            if( flag >= NUM_LOCK_FLAGS )
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common.BasicIdxBuffer");
                GN_ERROR(sLogger)( "invalid lock flag: %d!", flag );
                return false;
            }

            // adjust startidx and numidx
            if( 0 == numidx ) numidx = desc.numidx;
            if( startidx + numidx > desc.numidx ) numidx = desc.numidx - startidx;

            // success            
            mLocked = true;
            return true;
        }

        ///
        /// Basic unlock operation. Should be called in pair with baseLock().
        ///
        bool basicUnlock()
        {
            if( isLocked() )
            {
                mLocked = false;
                return true;
            }
            else
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common.BasicIdxBuffer");
                GN_ERROR(sLogger)( "dupilcate unlock! lock/unlock() must be called accordinglly!" );
                return false;
            }
        }

        // ********************************
        //  private variables
        // ********************************
    private:

        ///
        /// 锁定标志
        ///
        bool mLocked;

        // ********************************
        //  private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_BASICBUFFER_H__
