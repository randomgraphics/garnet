// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetMessageCountLimit, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(UINT64, MessageCountLimit)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, ClearStoredMessages, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetMessage, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(UINT64, MessageIndex),
    DEFINE_METHOD_PARAMETER(D3D11_MESSAGE *, pMessage),
    DEFINE_METHOD_PARAMETER(SIZE_T *, pMessageByteLength)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, UINT64, STDMETHODCALLTYPE, GetNumMessagesAllowedByStorageFilter, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, UINT64, STDMETHODCALLTYPE, GetNumMessagesDeniedByStorageFilter, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, UINT64, STDMETHODCALLTYPE, GetNumStoredMessages, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, UINT64, STDMETHODCALLTYPE, GetNumStoredMessagesAllowedByRetrievalFilter, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, UINT64, STDMETHODCALLTYPE, GetNumMessagesDiscardedByMessageCountLimit, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, UINT64, STDMETHODCALLTYPE, GetMessageCountLimit, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, AddStorageFilterEntries, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(D3D11_INFO_QUEUE_FILTER *, pFilter)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetStorageFilter, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(D3D11_INFO_QUEUE_FILTER *, pFilter),
    DEFINE_METHOD_PARAMETER(SIZE_T *, pFilterByteLength)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, ClearStorageFilter, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, PushEmptyStorageFilter, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, PushCopyOfStorageFilter, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, PushStorageFilter, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(D3D11_INFO_QUEUE_FILTER *, pFilter)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, PopStorageFilter, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, UINT, STDMETHODCALLTYPE, GetStorageFilterStackSize, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, AddRetrievalFilterEntries, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(D3D11_INFO_QUEUE_FILTER *, pFilter)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetRetrievalFilter, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(D3D11_INFO_QUEUE_FILTER *, pFilter),
    DEFINE_METHOD_PARAMETER(SIZE_T *, pFilterByteLength)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, ClearRetrievalFilter, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, PushEmptyRetrievalFilter, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, PushCopyOfRetrievalFilter, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, PushRetrievalFilter, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(D3D11_INFO_QUEUE_FILTER *, pFilter)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, PopRetrievalFilter, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, UINT, STDMETHODCALLTYPE, GetRetrievalFilterStackSize, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, AddMessage, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(D3D11_MESSAGE_CATEGORY, Category),
    DEFINE_METHOD_PARAMETER(D3D11_MESSAGE_SEVERITY, Severity),
    DEFINE_METHOD_PARAMETER(D3D11_MESSAGE_ID, ID),
    DEFINE_METHOD_PARAMETER(LPCSTR, pDescription)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, AddApplicationMessage, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(D3D11_MESSAGE_SEVERITY, Severity),
    DEFINE_METHOD_PARAMETER(LPCSTR, pDescription)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetBreakOnCategory, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(D3D11_MESSAGE_CATEGORY, Category),
    DEFINE_METHOD_PARAMETER(BOOL, bEnable)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetBreakOnSeverity, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(D3D11_MESSAGE_SEVERITY, Severity),
    DEFINE_METHOD_PARAMETER(BOOL, bEnable)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetBreakOnID, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(D3D11_MESSAGE_ID, ID),
    DEFINE_METHOD_PARAMETER(BOOL, bEnable)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, BOOL, STDMETHODCALLTYPE, GetBreakOnCategory, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(D3D11_MESSAGE_CATEGORY, Category)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, BOOL, STDMETHODCALLTYPE, GetBreakOnSeverity, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(D3D11_MESSAGE_SEVERITY, Severity)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, BOOL, STDMETHODCALLTYPE, GetBreakOnID, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(D3D11_MESSAGE_ID, ID)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, SetMuteDebugOutput, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(BOOL, bMute)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, BOOL, STDMETHODCALLTYPE, GetMuteDebugOutput, PARAMETER_LIST_0())
