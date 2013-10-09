// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetMessageCountLimit(
    _In_  UINT64 MessageCountLimit);
NullPtr<void (D3D11InfoQueueHook::*)(_In_  UINT64 &)> _SetMessageCountLimit_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  UINT64)> _SetMessageCountLimit_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ClearStoredMessages();
NullPtr<void (D3D11InfoQueueHook::*)()> _ClearStoredMessages_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)()> _ClearStoredMessages_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetMessage(
    _In_  UINT64 MessageIndex,
    _Out_writes_bytes_opt_(*pMessageByteLength)  D3D11_MESSAGE * pMessage,
    _Inout_  SIZE_T * pMessageByteLength);
NullPtr<void (D3D11InfoQueueHook::*)(_In_  UINT64 &, _Out_writes_bytes_opt_(*pMessageByteLength)  D3D11_MESSAGE * &, _Inout_  SIZE_T * &)> _GetMessage_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  UINT64, _Out_writes_bytes_opt_(*pMessageByteLength)  D3D11_MESSAGE *, _Inout_  SIZE_T *)> _GetMessage_post_ptr;
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
    _In_  D3D11_INFO_QUEUE_FILTER * pFilter);
NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_INFO_QUEUE_FILTER * &)> _AddStorageFilterEntries_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_INFO_QUEUE_FILTER *)> _AddStorageFilterEntries_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetStorageFilter(
    _Out_writes_bytes_opt_(*pFilterByteLength)  D3D11_INFO_QUEUE_FILTER * pFilter,
    _Inout_  SIZE_T * pFilterByteLength);
NullPtr<void (D3D11InfoQueueHook::*)(_Out_writes_bytes_opt_(*pFilterByteLength)  D3D11_INFO_QUEUE_FILTER * &, _Inout_  SIZE_T * &)> _GetStorageFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _Out_writes_bytes_opt_(*pFilterByteLength)  D3D11_INFO_QUEUE_FILTER *, _Inout_  SIZE_T *)> _GetStorageFilter_post_ptr;
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
    _In_  D3D11_INFO_QUEUE_FILTER * pFilter);
NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_INFO_QUEUE_FILTER * &)> _PushStorageFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_INFO_QUEUE_FILTER *)> _PushStorageFilter_post_ptr;
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
    _In_  D3D11_INFO_QUEUE_FILTER * pFilter);
NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_INFO_QUEUE_FILTER * &)> _AddRetrievalFilterEntries_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_INFO_QUEUE_FILTER *)> _AddRetrievalFilterEntries_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetRetrievalFilter(
    _Out_writes_bytes_opt_(*pFilterByteLength)  D3D11_INFO_QUEUE_FILTER * pFilter,
    _Inout_  SIZE_T * pFilterByteLength);
NullPtr<void (D3D11InfoQueueHook::*)(_Out_writes_bytes_opt_(*pFilterByteLength)  D3D11_INFO_QUEUE_FILTER * &, _Inout_  SIZE_T * &)> _GetRetrievalFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _Out_writes_bytes_opt_(*pFilterByteLength)  D3D11_INFO_QUEUE_FILTER *, _Inout_  SIZE_T *)> _GetRetrievalFilter_post_ptr;
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
    _In_  D3D11_INFO_QUEUE_FILTER * pFilter);
NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_INFO_QUEUE_FILTER * &)> _PushRetrievalFilter_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_INFO_QUEUE_FILTER *)> _PushRetrievalFilter_post_ptr;
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
    _In_  D3D11_MESSAGE_CATEGORY Category,
    _In_  D3D11_MESSAGE_SEVERITY Severity,
    _In_  D3D11_MESSAGE_ID ID,
    _In_  LPCSTR pDescription);
NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_MESSAGE_CATEGORY &, _In_  D3D11_MESSAGE_SEVERITY &, _In_  D3D11_MESSAGE_ID &, _In_  LPCSTR &)> _AddMessage_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_MESSAGE_CATEGORY, _In_  D3D11_MESSAGE_SEVERITY, _In_  D3D11_MESSAGE_ID, _In_  LPCSTR)> _AddMessage_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE AddApplicationMessage(
    _In_  D3D11_MESSAGE_SEVERITY Severity,
    _In_  LPCSTR pDescription);
NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_MESSAGE_SEVERITY &, _In_  LPCSTR &)> _AddApplicationMessage_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_MESSAGE_SEVERITY, _In_  LPCSTR)> _AddApplicationMessage_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetBreakOnCategory(
    _In_  D3D11_MESSAGE_CATEGORY Category,
    _In_  BOOL bEnable);
NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_MESSAGE_CATEGORY &, _In_  BOOL &)> _SetBreakOnCategory_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_MESSAGE_CATEGORY, _In_  BOOL)> _SetBreakOnCategory_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetBreakOnSeverity(
    _In_  D3D11_MESSAGE_SEVERITY Severity,
    _In_  BOOL bEnable);
NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_MESSAGE_SEVERITY &, _In_  BOOL &)> _SetBreakOnSeverity_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_MESSAGE_SEVERITY, _In_  BOOL)> _SetBreakOnSeverity_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetBreakOnID(
    _In_  D3D11_MESSAGE_ID ID,
    _In_  BOOL bEnable);
NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_MESSAGE_ID &, _In_  BOOL &)> _SetBreakOnID_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_MESSAGE_ID, _In_  BOOL)> _SetBreakOnID_post_ptr;
// -----------------------------------------------------------------------------
virtual BOOL STDMETHODCALLTYPE GetBreakOnCategory(
    _In_  D3D11_MESSAGE_CATEGORY Category);
NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_MESSAGE_CATEGORY &)> _GetBreakOnCategory_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(BOOL, _In_  D3D11_MESSAGE_CATEGORY)> _GetBreakOnCategory_post_ptr;
// -----------------------------------------------------------------------------
virtual BOOL STDMETHODCALLTYPE GetBreakOnSeverity(
    _In_  D3D11_MESSAGE_SEVERITY Severity);
NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_MESSAGE_SEVERITY &)> _GetBreakOnSeverity_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(BOOL, _In_  D3D11_MESSAGE_SEVERITY)> _GetBreakOnSeverity_post_ptr;
// -----------------------------------------------------------------------------
virtual BOOL STDMETHODCALLTYPE GetBreakOnID(
    _In_  D3D11_MESSAGE_ID ID);
NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_MESSAGE_ID &)> _GetBreakOnID_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(BOOL, _In_  D3D11_MESSAGE_ID)> _GetBreakOnID_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE SetMuteDebugOutput(
    _In_  BOOL bMute);
NullPtr<void (D3D11InfoQueueHook::*)(_In_  BOOL &)> _SetMuteDebugOutput_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(_In_  BOOL)> _SetMuteDebugOutput_post_ptr;
// -----------------------------------------------------------------------------
virtual BOOL STDMETHODCALLTYPE GetMuteDebugOutput();
NullPtr<void (D3D11InfoQueueHook::*)()> _GetMuteDebugOutput_pre_ptr;
NullPtr<void (D3D11InfoQueueHook::*)(BOOL)> _GetMuteDebugOutput_post_ptr;
