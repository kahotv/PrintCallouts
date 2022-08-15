#pragma once
/*
* ¥¶¿Ìcallouts
*/

typedef FWPM_SERVICE_STATE(NTAPI* t_FwpmBfeStateGet0)(void);
typedef NTSTATUS(NTAPI* t_FwpmBfeStateSubscribeChanges0)(
	void* deviceObject,
	FWPM_SERVICE_STATE_CHANGE_CALLBACK0 callback,
	void* context,
	HANDLE* changeHandle
	);
typedef NTSTATUS(NTAPI* t_FwpmBfeStateUnsubscribeChanges0)(
	HANDLE changeHandle
	);
typedef NTSTATUS(NTAPI* t_FwpmEngineOpen0)(
	const wchar_t* serverName,
	UINT32 authnService,
	SEC_WINNT_AUTH_IDENTITY_W* authIdentity,
	const FWPM_SESSION0* session,
	HANDLE* engineHandle
	);
typedef NTSTATUS(NTAPI* t_FwpmSubLayerCreateEnumHandle0)(
	HANDLE engineHandle,
	const FWPM_SUBLAYER_ENUM_TEMPLATE0* enumTemplate,
	HANDLE* enumHandle
	);
typedef NTSTATUS(NTAPI* t_FwpmSubLayerEnum0)(
	HANDLE engineHandle,
	HANDLE enumHandle,
	UINT32 numEntriesRequested,
	FWPM_SUBLAYER0*** entries,
	UINT32* numEntriesReturned
	);
typedef NTSTATUS(NTAPI* t_FwpmSubLayerDestroyEnumHandle0)(
	HANDLE engineHandle,
	HANDLE enumHandle
	);
typedef void (NTAPI* t_FwpmFreeMemory0)(void** p);

typedef NTSTATUS(NTAPI* t_FwpmEngineClose0)(HANDLE engineHandle);

typedef struct 
{
	UINT32 layer_index1;
	UINT32 layer_index2;
	UINT16 weight;
	eastl::wstring name;
	eastl::wstring desc;
}SublayersInfo;

class Callouts
{
	
public:
	static bool Init(void* pDeviceObject);
	static void Uninit();
	static eastl::map<UINT16, SublayersInfo> GetAll();
private:
	static VOID NTAPI bfeStateCallback(IN OUT void* context, IN FWPM_SERVICE_STATE  newState);
private:
	static HANDLE m_engineHandle;
	static HANDLE m_bfeStateSubscribeHandle;
	static KSPIN_LOCK m_lock;

	static t_FwpmBfeStateGet0				pFwpmBfeStateGet0;
	static t_FwpmBfeStateSubscribeChanges0	pFwpmBfeStateSubscribeChanges0;
	static t_FwpmBfeStateUnsubscribeChanges0 pFwpmBfeStateUnsubscribeChanges0;
	static t_FwpmEngineOpen0				pFwpmEngineOpen0;
	static t_FwpmSubLayerEnum0				pFwpmSubLayerEnum0;
	static t_FwpmSubLayerCreateEnumHandle0	pFwpmSubLayerCreateEnumHandle0;
	static t_FwpmSubLayerDestroyEnumHandle0	pFwpmSubLayerDestroyEnumHandle0;
	static t_FwpmFreeMemory0				pFwpmFreeMemory0;
	static t_FwpmEngineClose0				pFwpmEngineClose0;
};
__declspec(selectany) HANDLE							Callouts::m_engineHandle = NULL;
__declspec(selectany) HANDLE							Callouts::m_bfeStateSubscribeHandle = NULL;
__declspec(selectany) KSPIN_LOCK						Callouts::m_lock;

__declspec(selectany) t_FwpmBfeStateGet0				Callouts::pFwpmBfeStateGet0 = NULL;
__declspec(selectany) t_FwpmBfeStateSubscribeChanges0	Callouts::pFwpmBfeStateSubscribeChanges0 = NULL;
__declspec(selectany) t_FwpmBfeStateUnsubscribeChanges0	Callouts::pFwpmBfeStateUnsubscribeChanges0 = NULL;
__declspec(selectany) t_FwpmEngineOpen0					Callouts::pFwpmEngineOpen0 = NULL;
__declspec(selectany) t_FwpmSubLayerEnum0				Callouts::pFwpmSubLayerEnum0 = NULL;
__declspec(selectany) t_FwpmSubLayerCreateEnumHandle0	Callouts::pFwpmSubLayerCreateEnumHandle0 = NULL;
__declspec(selectany) t_FwpmSubLayerDestroyEnumHandle0	Callouts::pFwpmSubLayerDestroyEnumHandle0 = NULL;
__declspec(selectany) t_FwpmFreeMemory0					Callouts::pFwpmFreeMemory0 = NULL;
__declspec(selectany) t_FwpmEngineClose0				Callouts::pFwpmEngineClose0 = NULL;

