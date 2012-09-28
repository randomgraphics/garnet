// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetMessageCountLimit(
    UINT64 MessageCountLimit);
NullPtr<void (D3D11InfoQueueHook::*)(UINT64 &)> _SetMessageCountLimit_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, UINT64)> _SetMessageCountLimit_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ClearStoredMessages();
NullPtr<void (D3D11InfoQueueHook::*)()> _ClearStoredMessages_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)()> _ClearStoredMessages_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetMessage(
    UINT64 MessageIndex,
    D3D11_MESSAGE * pMessage,
    SIZE_T * pMessageByteLength);
NullPtr<void (D3D11InfoQueueHook::*)(UINT64 &, D3D11_MESSAGE * &, SIZE_T * &)> _GetMessage_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, UINT64, D3D11_MESSAGE *, SIZE_T *)> _GetMessage_post_ptr;
// -----------------------------------------------------------------------------
virtual UINT64 STDMETHODCALLTYPE GetNumMessagesAllowedByStorageFilter();
NullPtr<void (D3D11InfoQueueHook::*)()> _GetNumMessagesAllowedByStorageFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(UINT64)> _GetNumMessagesAllowedByStorageFilter_post_ptr;
// -----------------------------------------------------------------------------
virtual UINT64 STDMETHODCALLTYPE GetNumMessagesDeniedByStorageFilter();
NullPtr<void (D3D11InfoQueueHook::*)()> _GetNumMessagesDeniedByStorageFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(UINT64)> _GetNumMessagesDeniedByStorageFilter_post_ptr;
// -----------------------------------------------------------------------------
virtual UINT64 STDMETHODCALLTYPE GetNumStoredMessages();
NullPtr<void (D3D11InfoQueueHook::*)()> _GetNumStoredMessages_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(UINT64)> _GetNumStoredMessages_post_ptr;
// -----------------------------------------------------------------------------
virtual UINT64 STDMETHODCALLTYPE GetNumStoredMessagesAllowedByRetrievalFilter();
NullPtr<void (D3D11InfoQueueHook::*)()> _GetNumStoredMessagesAllowedByRetrievalFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(UINT64)> _GetNumStoredMessagesAllowedByRetrievalFilter_post_ptr;
// -----------------------------------------------------------------------------
virtual UINT64 STDMETHODCALLTYPE GetNumMessagesDiscardedByMessageCountLimit();
NullPtr<void (D3D11InfoQueueHook::*)()> _GetNumMessagesDiscardedByMessageCountLimit_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(UINT64)> _GetNumMessagesDiscardedByMessageCountLimit_post_ptr;
// -----------------------------------------------------------------------------
virtual UINT64 STDMETHODCALLTYPE GetMessageCountLimit();
NullPtr<void (D3D11InfoQueueHook::*)()> _GetMessageCountLimit_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(UINT64)> _GetMessageCountLimit_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE AddStorageFilterEntries(
    D3D11_INFO_QUEUE_FILTER * pFilter);
NullPtr<void (D3D11InfoQueueHook::*)(D3D11_INFO_QUEUE_FILTER * &)> _AddStorageFilterEntries_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, D3D11_INFO_QUEUE_FILTER *)> _AddStorageFilterEntries_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetStorageFilter(
    D3D11_INFO_QUEUE_FILTER * pFilter,
    SIZE_T * pFilterByteLength);
NullPtr<void (D3D11InfoQueueHook::*)(D3D11_INFO_QUEUE_FILTER * &, SIZE_T * &)> _GetStorageFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, D3D11_INFO_QUEUE_FILTER *, SIZE_T *)> _GetStorageFilter_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ClearStorageFilter();
NullPtr<void (D3D11InfoQueueHook::*)()> _ClearStorageFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)()> _ClearStorageFilter_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE PushEmptyStorageFilter();
NullPtr<void (D3D11InfoQueueHook::*)()> _PushEmptyStorageFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT)> _PushEmptyStorageFilter_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE PushCopyOfStorageFilter();
NullPtr<void (D3D11InfoQueueHook::*)()> _PushCopyOfStorageFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT)> _PushCopyOfStorageFilter_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE PushStorageFilter(
    D3D11_INFO_QUEUE_FILTER * pFilter);
NullPtr<void (D3D11InfoQueueHook::*)(D3D11_INFO_QUEUE_FILTER * &)> _PushStorageFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, D3D11_INFO_QUEUE_FILTER *)> _PushStorageFilter_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PopStorageFilter();
NullPtr<void (D3D11InfoQueueHook::*)()> _PopStorageFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)()> _PopStorageFilter_post_ptr;
// -----------------------------------------------------------------------------
virtual UINT STDMETHODCALLTYPE GetStorageFilterStackSize();
NullPtr<void (D3D11InfoQueueHook::*)()> _GetStorageFilterStackSize_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(UINT)> _GetStorageFilterStackSize_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE AddRetrievalFilterEntries(
    D3D11_INFO_QUEUE_FILTER * pFilter);
NullPtr<void (D3D11InfoQueueHook::*)(D3D11_INFO_QUEUE_FILTER * &)> _AddRetrievalFilterEntries_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, D3D11_INFO_QUEUE_FILTER *)> _AddRetrievalFilterEntries_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetRetrievalFilter(
    D3D11_INFO_QUEUE_FILTER * pFilter,
    SIZE_T * pFilterByteLength);
NullPtr<void (D3D11InfoQueueHook::*)(D3D11_INFO_QUEUE_FILTER * &, SIZE_T * &)> _GetRetrievalFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, D3D11_INFO_QUEUE_FILTER *, SIZE_T *)> _GetRetrievalFilter_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ClearRetrievalFilter();
NullPtr<void (D3D11InfoQueueHook::*)()> _ClearRetrievalFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)()> _ClearRetrievalFilter_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE PushEmptyRetrievalFilter();
NullPtr<void (D3D11InfoQueueHook::*)()> _PushEmptyRetrievalFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT)> _PushEmptyRetrievalFilter_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE PushCopyOfRetrievalFilter();
NullPtr<void (D3D11InfoQueueHook::*)()> _PushCopyOfRetrievalFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT)> _PushCopyOfRetrievalFilter_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE PushRetrievalFilter(
    D3D11_INFO_QUEUE_FILTER * pFilter);
NullPtr<void (D3D11InfoQueueHook::*)(D3D11_INFO_QUEUE_FILTER * &)> _PushRetrievalFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, D3D11_INFO_QUEUE_FILTER *)> _PushRetrievalFilter_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PopRetrievalFilter();
NullPtr<void (D3D11InfoQueueHook::*)()> _PopRetrievalFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)()> _PopRetrievalFilter_post_ptr;
// -----------------------------------------------------------------------------
virtual UINT STDMETHODCALLTYPE GetRetrievalFilterStackSize();
NullPtr<void (D3D11InfoQueueHook::*)()> _GetRetrievalFilterStackSize_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(UINT)> _GetRetrievalFilterStackSize_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE AddMessage(
    D3D11_MESSAGE_CATEGORY Category,
    D3D11_MESSAGE_SEVERITY Severity,
    D3D11_MESSAGE_ID ID,
    LPCSTR pDescription);
NullPtr<void (D3D11InfoQueueHook::*)(D3D11_MESSAGE_CATEGORY &, D3D11_MESSAGE_SEVERITY &, D3D11_MESSAGE_ID &, LPCSTR &)> _AddMessage_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, D3D11_MESSAGE_CATEGORY, D3D11_MESSAGE_SEVERITY, D3D11_MESSAGE_ID, LPCSTR)> _AddMessage_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE AddApplicationMessage(
    D3D11_MESSAGE_SEVERITY Severity,
    LPCSTR pDescription);
NullPtr<void (D3D11InfoQueueHook::*)(D3D11_MESSAGE_SEVERITY &, LPCSTR &)> _AddApplicationMessage_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, D3D11_MESSAGE_SEVERITY, LPCSTR)> _AddApplicationMessage_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetBreakOnCategory(
    D3D11_MESSAGE_CATEGORY Category,
    BOOL bEnable);
NullPtr<void (D3D11InfoQueueHook::*)(D3D11_MESSAGE_CATEGORY &, BOOL &)> _SetBreakOnCategory_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, D3D11_MESSAGE_CATEGORY, BOOL)> _SetBreakOnCategory_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetBreakOnSeverity(
    D3D11_MESSAGE_SEVERITY Severity,
    BOOL bEnable);
NullPtr<void (D3D11InfoQueueHook::*)(D3D11_MESSAGE_SEVERITY &, BOOL &)> _SetBreakOnSeverity_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, D3D11_MESSAGE_SEVERITY, BOOL)> _SetBreakOnSeverity_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetBreakOnID(
    D3D11_MESSAGE_ID ID,
    BOOL bEnable);
NullPtr<void (D3D11InfoQueueHook::*)(D3D11_MESSAGE_ID &, BOOL &)> _SetBreakOnID_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, D3D11_MESSAGE_ID, BOOL)> _SetBreakOnID_post_ptr;
// -----------------------------------------------------------------------------
virtual BOOL STDMETHODCALLTYPE GetBreakOnCategory(
    D3D11_MESSAGE_CATEGORY Category);
NullPtr<void (D3D11InfoQueueHook::*)(D3D11_MESSAGE_CATEGORY &)> _GetBreakOnCategory_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(BOOL, D3D11_MESSAGE_CATEGORY)> _GetBreakOnCategory_post_ptr;
// -----------------------------------------------------------------------------
virtual BOOL STDMETHODCALLTYPE GetBreakOnSeverity(
    D3D11_MESSAGE_SEVERITY Severity);
NullPtr<void (D3D11InfoQueueHook::*)(D3D11_MESSAGE_SEVERITY &)> _GetBreakOnSeverity_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(BOOL, D3D11_MESSAGE_SEVERITY)> _GetBreakOnSeverity_post_ptr;
// -----------------------------------------------------------------------------
virtual BOOL STDMETHODCALLTYPE GetBreakOnID(
    D3D11_MESSAGE_ID ID);
NullPtr<void (D3D11InfoQueueHook::*)(D3D11_MESSAGE_ID &)> _GetBreakOnID_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(BOOL, D3D11_MESSAGE_ID)> _GetBreakOnID_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE SetMuteDebugOutput(
    BOOL bMute);
NullPtr<void (D3D11InfoQueueHook::*)(BOOL &)> _SetMuteDebugOutput_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(BOOL)> _SetMuteDebugOutput_post_ptr;
// -----------------------------------------------------------------------------
virtual BOOL STDMETHODCALLTYPE GetMuteDebugOutput();
NullPtr<void (D3D11InfoQueueHook::*)()> _GetMuteDebugOutput_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(BOOL)> _GetMuteDebugOutput_post_ptr;
