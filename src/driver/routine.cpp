#include "stdafx.h"
#include "routine.h"


NTSTATUS CreateCompleteRoutine(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	UNREFERENCED_PARAMETER(pDeviceObject);

	NTSTATUS status = STATUS_SUCCESS;

	KdPrint(("Create...\r\n"));

	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;
	// 设置 Irp 请求已经处理完成，不要再继续传递
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}

NTSTATUS CloseCompleteRoutine(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	UNREFERENCED_PARAMETER(pDeviceObject);


	NTSTATUS status = STATUS_SUCCESS;

	KdPrint(("Close...\r\n"));
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;
	// 设置 Irp 请求已经处理完成，不要再继续传递
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}

NTSTATUS ReadCompleteRoutine(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	UNREFERENCED_PARAMETER(pDeviceObject);


	NTSTATUS status = STATUS_SUCCESS;

	KdPrint(("Read...\r\n"));

	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;
	// 设置 Irp 请求已经处理完成，不要再继续传递
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}

NTSTATUS WriteCompleteRoutine(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	UNREFERENCED_PARAMETER(pDeviceObject);


	NTSTATUS status = STATUS_SUCCESS;

	KdPrint(("Write...\r\n"));

	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;
	// 设置 Irp 请求已经处理完成，不要再继续传递
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}

//NTSTATUS DeviceControlCompleteRoutine_GETCALLOUTS(void* pBuffer, ULONG inBufLen, ULONG outBufLen, ULONG* outLen)
//{
//	auto list = Callouts::GetAll();
//	DbgPrint("callouts num: %lld", list.size());
//
//
//}

NTSTATUS DeviceControlCompleteRoutine(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	UNREFERENCED_PARAMETER(pDeviceObject);

	NTSTATUS			status = STATUS_SUCCESS;
	PIO_STACK_LOCATION	pIrpsp = IoGetCurrentIrpStackLocation(pIrp);
	ULONG				uOutLen = 0;


	PVOID pBuffer = pIrp->AssociatedIrp.SystemBuffer;
	//ULONG inBufLen = pIrpsp->Parameters.DeviceIoControl.InputBufferLength;
	//ULONG ouBufLen = pIrpsp->Parameters.DeviceIoControl.OutputBufferLength;

	//KdPrint(("DeviceControlCompleteRoutine inBufLen: %d,outBufLen: %d, pBuffer: %p \n", inBufLen, ouBufLen, pBuffer));

	//DbgBreakPoint();

	do
	{
		if (pBuffer == NULL)
		{
			status = STATUS_INVALID_BUFFER_SIZE;
			break;
		}

		switch (pIrpsp->Parameters.DeviceIoControl.IoControlCode)
		{
			case CWK_DVC_GET_CALLOUTS:
			{
				//status = DeviceControlCompleteRoutine_GETCALLOUTS(pBuffer, inBufLen, ouBufLen, &uOutLen);
				break;
			}
			default:
			{
				status = STATUS_INVALID_PARAMETER;
			}
		}
	} while (FALSE);

	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = uOutLen;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}
