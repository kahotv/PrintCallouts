#pragma once

#define LOG_NAME				"[callouts]"

//�������ơ���������
#define DRIVER_NAME				L"PrintCallouts"



// ȡcallouts�б�
#define  CWK_DVC_GET_CALLOUTS \
	(ULONG)CTL_CODE( \
	FILE_DEVICE_UNKNOWN, \
	0x911,METHOD_BUFFERED, \
	FILE_READ_DATA)



typedef struct
{
	uint16_t index;
	wchar_t name[1024];
	wchar_t desc[1024];
}callout_info;