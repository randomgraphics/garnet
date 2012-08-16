#pragma once

// Critical Section c++ wrapper
class CritSec
{
public:

    CritSec()
    {
        InitializeCriticalSection(&_cs);
    }

    ~CritSec()
    {
        DeleteCriticalSection(&_cs);
    }

    CRITICAL_SECTION * GetRawCS() const
    {
        return &_cs;
    }

    BOOL TryEnter() const
    {
        return TryEnterCriticalSection(&_cs);
    }

    void Enter() const
    {
        EnterCriticalSection(&_cs);
    }

    void Leave() const
    {
        LeaveCriticalSection(&_cs);
    }

    // This class works with both CritSec and raw CRITICAL_SECTION
    class AutoLock
    {
        CRITICAL_SECTION * _pcs;

    public:

        explicit AutoLock(CRITICAL_SECTION & cs)
            : _pcs(&cs)
        {
            if(NULL != _pcs)
            {
                EnterCriticalSection(_pcs);
            }
        }

        explicit AutoLock(CritSec & cs)
            : _pcs(cs.GetRawCS())
        {
            if(NULL != _pcs)
            {
                EnterCriticalSection(_pcs);
            }
        }

        ~AutoLock()
        {
            Unlock();
        }

        void Unlock()
        {
            if (NULL != _pcs)
            {
                LeaveCriticalSection(_pcs);
                _pcs = NULL;
            }
        }
    };

private:

    mutable CRITICAL_SECTION _cs;
};