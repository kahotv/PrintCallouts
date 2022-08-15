#include "stdafx.h"
#include "callouts.h"

VOID NTAPI
Callouts::bfeStateCallback(
	IN OUT void* context,
	IN FWPM_SERVICE_STATE  newState
)
{
	UNREFERENCED_PARAMETER(context);

	KdPrint((LOG_NAME" bfeStateCallback begin\n"));

	if (newState == FWPM_SERVICE_RUNNING)
	{
		FWPM_SESSION session = { 0};
		session.flags = FWPM_SESSION_FLAG_DYNAMIC;
		//open
		NTSTATUS status = pFwpmEngineOpen0(
				NULL,
				RPC_C_AUTHN_WINNT,
				NULL,
				&session,
				&m_engineHandle
			);
		if (!NT_SUCCESS(status))
		{
			KdPrint((LOG_NAME"bfeStateCallback callouts_init failed, status=%x\n", status));
		}
	}
	KdPrint((LOG_NAME" bfeStateCallback end\n"));
}


bool Callouts::Init(void* pDeviceObject)
{
	NTSTATUS status = 0;
	void* pModule = NULL;
	FWPM_SESSION session = { 0 };
	//KIRQL irql;
	bool succ = false;
	KeInitializeSpinLock(&m_lock);

	do
	{
		//KeAcquireSpinLock(&m_lock, &irql);
		if(m_engineHandle != NULL)
		{
			KdPrint((LOG_NAME"m_engineHandle not NULL\n"));
			break;
		}

		pModule = dynlink_getModuleBase("fwpkclnt.sys");
		if (!pModule)
		{
			KdPrint((LOG_NAME"dynlink_getModuleBase failed\n"));
			break;
		}

		pFwpmBfeStateGet0 = (t_FwpmBfeStateGet0)dynlink_getProcAddress(pModule, "FwpmBfeStateGet0");
		if (!pFwpmBfeStateGet0)
		{
			KdPrint((LOG_NAME"dynlink_getProcAddress failed for FwpmBfeStateGet0\n"));
			return FALSE;
		}

		pFwpmBfeStateSubscribeChanges0 = (t_FwpmBfeStateSubscribeChanges0)dynlink_getProcAddress(pModule, "FwpmBfeStateSubscribeChanges0");
		if (!pFwpmBfeStateSubscribeChanges0)
		{
			KdPrint((LOG_NAME"dynlink_getProcAddress failed for FwpmBfeStateSubscribeChanges0\n"));
			return FALSE;
		}

		pFwpmEngineOpen0 = (t_FwpmEngineOpen0)dynlink_getProcAddress(pModule, "FwpmEngineOpen0");
		if (!pFwpmEngineOpen0)
		{
			KdPrint((LOG_NAME"dynlink_getProcAddress failed for FwpmEngineOpen0\n"));
			break;
		}

		pFwpmSubLayerEnum0 = (t_FwpmSubLayerEnum0)dynlink_getProcAddress(pModule, "FwpmSubLayerEnum0");
		if (!pFwpmSubLayerEnum0)
		{
			KdPrint((LOG_NAME"dynlink_getProcAddress failed for FwpmSubLayerEnum0\n"));
			break;
		}

		pFwpmSubLayerCreateEnumHandle0 = (t_FwpmSubLayerCreateEnumHandle0)dynlink_getProcAddress(pModule, "FwpmSubLayerCreateEnumHandle0");
		if (!pFwpmSubLayerCreateEnumHandle0)
		{
			KdPrint((LOG_NAME"dynlink_getProcAddress failed for FwpmSubLayerCreateEnumHandle0\n"));
			break;
		}

		pFwpmSubLayerDestroyEnumHandle0 = (t_FwpmSubLayerDestroyEnumHandle0)dynlink_getProcAddress(pModule, "FwpmSubLayerDestroyEnumHandle0");
		if (!pFwpmSubLayerDestroyEnumHandle0)
		{
			KdPrint((LOG_NAME"dynlink_getProcAddress failed for FwpmSubLayerDestroyEnumHandle0\n"));
			break;
		}

		pFwpmFreeMemory0 = (t_FwpmFreeMemory0)dynlink_getProcAddress(pModule, "FwpmFreeMemory0");
		if (!pFwpmFreeMemory0)
		{
			KdPrint((LOG_NAME"dynlink_getProcAddress failed for FwpmFreeMemory0\n"));
			break;
		}

		pFwpmEngineClose0 = (t_FwpmEngineClose0)dynlink_getProcAddress(pModule, "FwpmEngineClose0");
		if (!pFwpmEngineClose0)
		{
			KdPrint((LOG_NAME"dynlink_getProcAddress failed for pFwpmEngineClose0\n"));
			break;
		}

		if (pFwpmBfeStateGet0() == FWPM_SERVICE_RUNNING)
		{
			KdPrint((LOG_NAME" is running %p\n", pFwpmEngineOpen0));
			session.flags = FWPM_SESSION_FLAG_DYNAMIC;
			//open
			status = pFwpmEngineOpen0(
				NULL,
				RPC_C_AUTHN_WINNT,
				NULL,
				&session,
				&m_engineHandle
			);
			KdPrint((LOG_NAME" open status: %x\n", status));
		}
		else 
		{
			KdPrint((LOG_NAME" need start\n"));
			status = pFwpmBfeStateSubscribeChanges0(
				pDeviceObject,
				bfeStateCallback,
				NULL,
				&m_bfeStateSubscribeHandle);
		}

		
		if (!NT_SUCCESS(status))
		{
			KdPrint((LOG_NAME"pFwpmEngineOpen0 failed %x\n", status));
			break;
		}

		succ = true;

	} while (false);

	//KeReleaseSpinLock(&m_lock, irql);

	return succ;
}
void Callouts::Uninit()
{
	//KIRQL irql;

	//KeAcquireSpinLock(&m_lock, &irql);
	KdPrint((LOG_NAME"Uninit begin m_engineHandle=%x, m_bfeStateSubscribeHandle=%x\n", m_engineHandle, m_bfeStateSubscribeHandle));

	if (m_engineHandle)
	{
		pFwpmEngineClose0(m_engineHandle);
		m_engineHandle = NULL;
	}
	if (m_bfeStateSubscribeHandle)
	{
		pFwpmBfeStateUnsubscribeChanges0(m_bfeStateSubscribeHandle);
		m_bfeStateSubscribeHandle = NULL;
	}
	//KeReleaseSpinLock(&m_lock, irql);
	KdPrint((LOG_NAME"Uninit end m_engineHandle=%x, m_bfeStateSubscribeHandle=%x\n", m_engineHandle, m_bfeStateSubscribeHandle));
}
eastl::map<UINT16, SublayersInfo> Callouts::GetAll()
{
	//KIRQL irql;
	//eastl::vector<callout_info> v;

	eastl::map<UINT16, SublayersInfo> map;

	HANDLE hEnum = NULL;

	//KeAcquireSpinLock(&m_lock, &irql);



	do
	{
		if (m_engineHandle == NULL)
			break;

		if (pFwpmSubLayerCreateEnumHandle0(m_engineHandle, NULL, &hEnum) != 0)
			break;

		for (UINT32 n = 0;; n++)
		{
			FWPM_SUBLAYER** pSublayers;
			UINT32 nItems = 0;

			if (pFwpmSubLayerEnum0(m_engineHandle, hEnum, 1, &pSublayers, &nItems) != 0)
				break;
			if (nItems < 1)
				break;

			for (UINT32 i = 0; i < nItems; i++)
			{
				auto& item = map[pSublayers[i]->weight];
				
				item.layer_index1 = n;
				item.layer_index2 = i;
				item.weight = pSublayers[i]->weight;
				if(pSublayers[i]->displayData.name != NULL)
					item.name = pSublayers[i]->displayData.name;
				if(pSublayers[i]->displayData.description != NULL)
					item.desc = pSublayers[i]->displayData.description;
				

				/*DbgPrint(LOG_NAME"getSublayer[%d][%d] %d, %ws, %ws\n",
					n,
					i,
					pSublayers[i]->weight,
					pSublayers[i]->displayData.name,
					pSublayers[i]->displayData.description);*/
			}

			pFwpmFreeMemory0((void**)&pSublayers);
		}
		pFwpmSubLayerDestroyEnumHandle0(m_engineHandle, hEnum);
	} while (false);

	//KeReleaseSpinLock(&m_lock, irql);

	return map;
 }
