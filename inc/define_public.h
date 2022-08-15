#pragma once

#define LOG_NAME				"[callouts]"

//驱动名称、服务名称
#define DRIVER_NAME				L"PrintCallouts"



// 取callouts列表
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