#include <wdm.h>

VOID DriverUnLoad(PDRIVER_OBJECT pDrvObject)
{
	UNREFERENCED_PARAMETER(pDrvObject);
	DbgPrint("DriverUnLoad");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDrvObject, PUNICODE_STRING pRegPath)
{
	UNREFERENCED_PARAMETER(pDrvObject);
	UNREFERENCED_PARAMETER(pRegPath);
	DbgPrint("DriverEntry");

	pDrvObject->DriverUnload = DriverUnLoad;

	//return STATUS_NO_MEMORY;
	return STATUS_UNSUCCESSFUL
	//return STATUS_SUCCESS;
}