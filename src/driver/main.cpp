#include "stdafx.h"
NTSTATUS CreateDevice(PDRIVER_OBJECT pDriverObject, PDEVICE_OBJECT* ppDeviceObject)
{
	NTSTATUS status;
	PDEVICE_OBJECT pDeviceObject;
	UNICODE_STRING usDeviceName;
	UNICODE_STRING usSymbolicName;

	RtlInitUnicodeString(&usDeviceName, L"\\Device\\" DRIVER_NAME);

	status = IoCreateDevice(
		pDriverObject,
		0,
		&usDeviceName,
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		TRUE,
		&pDeviceObject);
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	pDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

	RtlInitUnicodeString(&usSymbolicName, L"\\??\\" DRIVER_NAME);

	status = IoCreateSymbolicLink(&usSymbolicName, &usDeviceName);
	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(pDeviceObject);
		return status;
	}

	*ppDeviceObject = pDeviceObject;

	return STATUS_SUCCESS;
}

VOID DriverUnLoad(PDRIVER_OBJECT pDrvObject)
{
	UNREFERENCED_PARAMETER(pDrvObject);

	//DbgBreakPoint();

	UNICODE_STRING usSymbolicName;

	// 删除符号链接和设备对象
	//Callouts::PrintAll();
	Callouts::Uninit();

	if (NULL != pDrvObject->DeviceObject)
	{
		RtlInitUnicodeString(&usSymbolicName, L"\\??\\" DRIVER_NAME);
		IoDeleteSymbolicLink(&usSymbolicName);
		IoDeleteDevice(pDrvObject->DeviceObject);
		KdPrint(("Unload driver success..\r\n"));
	}

	// 释放使用中的链表所有内存
	//ClearListNodePool(&g_ListHead, &g_Lock);
	// 释放链表池子里的所有对象
	//ClearListNodePool(&g_pool_pn, &g_pool_pn_lock);
	DbgPrint("DriverUnLoad");

}

NTSTATUS SysMain(PDRIVER_OBJECT pDrvObject, PUNICODE_STRING pRegPath) {
	UNREFERENCED_PARAMETER(pDrvObject);
	UNREFERENCED_PARAMETER(pRegPath);

#if (NTDDI_VERSION >= NTDDI_WIN8)
	ExInitializeDriverRuntime(DrvRtPoolNxOptIn);
#endif

	// NOTE OF PATH:
	// SEPARATOR = '/' !!!
	// BASE PATH = %SystemRoot% !!!

	DbgPrint("SysMain begin");

	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT pDeviceObject = NULL;
	//创建设备
	status = CreateDevice(pDrvObject, &pDeviceObject);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("CreateDevice(%p) Fail, status: %x", pDrvObject, status);
		return status;
		//return STATUS_UNSUCCESSFUL;
	}
	if (!Callouts::Init(pDeviceObject))
	{
		DbgPrint("Callouts::Init() Fail");
		DriverUnLoad(pDrvObject);
		return STATUS_UNSUCCESSFUL;
	}

	// 处理不同的 IRP 请求
	pDrvObject->MajorFunction[IRP_MJ_CREATE] = CreateCompleteRoutine;
	pDrvObject->MajorFunction[IRP_MJ_CLOSE] = CloseCompleteRoutine;
	pDrvObject->MajorFunction[IRP_MJ_READ] = ReadCompleteRoutine;
	pDrvObject->MajorFunction[IRP_MJ_WRITE] = WriteCompleteRoutine;
	pDrvObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeviceControlCompleteRoutine;

	pDrvObject->DriverUnload = DriverUnLoad;

	auto map = Callouts::GetAll();
	
	for (const auto& item : map)
	{
		DbgPrint(LOG_NAME"Sublayer index: %3d-%-3d, weight: %-5d, name: %-30ws, desc: %ws\n",
			item.second.layer_index1,
			item.second.layer_index2,
			item.second.weight,
			item.second.name.c_str(),
			item.second.desc.c_str());
	}
	DriverUnLoad(pDrvObject);
	DbgPrint("SysMain end");
	//return STATUS_SUCCESS;
	return STATUS_UNSUCCESSFUL;
	//return status;
}
