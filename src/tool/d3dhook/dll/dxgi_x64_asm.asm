.code
extern mProcs:QWORD
CompatString_wrapper proc
	jmp mProcs[0*8]
CompatString_wrapper endp
CompatValue_wrapper proc
	jmp mProcs[1*8]
CompatValue_wrapper endp
;;CreateDXGIFactory_wrapper proc
;;	jmp mProcs[2*8]
;;CreateDXGIFactory_wrapper endp
;;CreateDXGIFactory1_wrapper proc
;;	jmp mProcs[3*8]
;;CreateDXGIFactory1_wrapper endp
;;CreateDXGIFactory2_wrapper proc
;;	jmp mProcs[4*8]
;;CreateDXGIFactory2_wrapper endp
D3DKMTCloseAdapter_wrapper proc
	jmp mProcs[5*8]
D3DKMTCloseAdapter_wrapper endp
D3DKMTCreateAllocation_wrapper proc
	jmp mProcs[6*8]
D3DKMTCreateAllocation_wrapper endp
D3DKMTCreateContext_wrapper proc
	jmp mProcs[7*8]
D3DKMTCreateContext_wrapper endp
D3DKMTCreateDevice_wrapper proc
	jmp mProcs[8*8]
D3DKMTCreateDevice_wrapper endp
D3DKMTCreateSynchronizationObject_wrapper proc
	jmp mProcs[9*8]
D3DKMTCreateSynchronizationObject_wrapper endp
D3DKMTDestroyAllocation_wrapper proc
	jmp mProcs[10*8]
D3DKMTDestroyAllocation_wrapper endp
D3DKMTDestroyContext_wrapper proc
	jmp mProcs[11*8]
D3DKMTDestroyContext_wrapper endp
D3DKMTDestroyDevice_wrapper proc
	jmp mProcs[12*8]
D3DKMTDestroyDevice_wrapper endp
D3DKMTDestroySynchronizationObject_wrapper proc
	jmp mProcs[13*8]
D3DKMTDestroySynchronizationObject_wrapper endp
D3DKMTEscape_wrapper proc
	jmp mProcs[14*8]
D3DKMTEscape_wrapper endp
D3DKMTGetContextSchedulingPriority_wrapper proc
	jmp mProcs[15*8]
D3DKMTGetContextSchedulingPriority_wrapper endp
D3DKMTGetDeviceState_wrapper proc
	jmp mProcs[16*8]
D3DKMTGetDeviceState_wrapper endp
D3DKMTGetDisplayModeList_wrapper proc
	jmp mProcs[17*8]
D3DKMTGetDisplayModeList_wrapper endp
D3DKMTGetMultisampleMethodList_wrapper proc
	jmp mProcs[18*8]
D3DKMTGetMultisampleMethodList_wrapper endp
D3DKMTGetRuntimeData_wrapper proc
	jmp mProcs[19*8]
D3DKMTGetRuntimeData_wrapper endp
D3DKMTGetSharedPrimaryHandle_wrapper proc
	jmp mProcs[20*8]
D3DKMTGetSharedPrimaryHandle_wrapper endp
D3DKMTLock_wrapper proc
	jmp mProcs[21*8]
D3DKMTLock_wrapper endp
D3DKMTOpenAdapterFromHdc_wrapper proc
	jmp mProcs[22*8]
D3DKMTOpenAdapterFromHdc_wrapper endp
D3DKMTOpenResource_wrapper proc
	jmp mProcs[23*8]
D3DKMTOpenResource_wrapper endp
D3DKMTPresent_wrapper proc
	jmp mProcs[24*8]
D3DKMTPresent_wrapper endp
D3DKMTQueryAdapterInfo_wrapper proc
	jmp mProcs[25*8]
D3DKMTQueryAdapterInfo_wrapper endp
D3DKMTQueryAllocationResidency_wrapper proc
	jmp mProcs[26*8]
D3DKMTQueryAllocationResidency_wrapper endp
D3DKMTQueryResourceInfo_wrapper proc
	jmp mProcs[27*8]
D3DKMTQueryResourceInfo_wrapper endp
D3DKMTRender_wrapper proc
	jmp mProcs[28*8]
D3DKMTRender_wrapper endp
D3DKMTSetAllocationPriority_wrapper proc
	jmp mProcs[29*8]
D3DKMTSetAllocationPriority_wrapper endp
D3DKMTSetContextSchedulingPriority_wrapper proc
	jmp mProcs[30*8]
D3DKMTSetContextSchedulingPriority_wrapper endp
D3DKMTSetDisplayMode_wrapper proc
	jmp mProcs[31*8]
D3DKMTSetDisplayMode_wrapper endp
D3DKMTSetDisplayPrivateDriverFormat_wrapper proc
	jmp mProcs[32*8]
D3DKMTSetDisplayPrivateDriverFormat_wrapper endp
D3DKMTSetGammaRamp_wrapper proc
	jmp mProcs[33*8]
D3DKMTSetGammaRamp_wrapper endp
D3DKMTSetVidPnSourceOwner_wrapper proc
	jmp mProcs[34*8]
D3DKMTSetVidPnSourceOwner_wrapper endp
D3DKMTSignalSynchronizationObject_wrapper proc
	jmp mProcs[35*8]
D3DKMTSignalSynchronizationObject_wrapper endp
D3DKMTUnlock_wrapper proc
	jmp mProcs[36*8]
D3DKMTUnlock_wrapper endp
D3DKMTWaitForSynchronizationObject_wrapper proc
	jmp mProcs[37*8]
D3DKMTWaitForSynchronizationObject_wrapper endp
D3DKMTWaitForVerticalBlankEvent_wrapper proc
	jmp mProcs[38*8]
D3DKMTWaitForVerticalBlankEvent_wrapper endp
DXGID3D10CreateDevice_wrapper proc
	jmp mProcs[39*8]
DXGID3D10CreateDevice_wrapper endp
DXGID3D10CreateLayeredDevice_wrapper proc
	jmp mProcs[40*8]
DXGID3D10CreateLayeredDevice_wrapper endp
DXGID3D10ETWRundown_wrapper proc
	jmp mProcs[41*8]
DXGID3D10ETWRundown_wrapper endp
DXGID3D10GetLayeredDeviceSize_wrapper proc
	jmp mProcs[42*8]
DXGID3D10GetLayeredDeviceSize_wrapper endp
DXGID3D10RegisterLayers_wrapper proc
	jmp mProcs[43*8]
DXGID3D10RegisterLayers_wrapper endp
DXGIDumpJournal_wrapper proc
	jmp mProcs[44*8]
DXGIDumpJournal_wrapper endp
DXGIGetDebugInterface1_wrapper proc
	jmp mProcs[45*8]
DXGIGetDebugInterface1_wrapper endp
DXGIReportAdapterConfiguration_wrapper proc
	jmp mProcs[46*8]
DXGIReportAdapterConfiguration_wrapper endp
DXGIRevertToSxS_wrapper proc
	jmp mProcs[47*8]
DXGIRevertToSxS_wrapper endp
OpenAdapter10_wrapper proc
	jmp mProcs[48*8]
OpenAdapter10_wrapper endp
OpenAdapter10_2_wrapper proc
	jmp mProcs[49*8]
OpenAdapter10_2_wrapper endp
SetAppCompatStringPointer_wrapper proc
	jmp mProcs[50*8]
SetAppCompatStringPointer_wrapper endp
end
