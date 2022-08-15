//
// 	NetFilterSDK 
// 	Copyright (C) Vitaly Sidorov
//	All rights reserved.
//
//	This file is a part of the NetFilter SDK.
//	The code and information is provided "as-is" without
//	warranty of any kind, either expressed or implied.
//


#ifndef _DYNLINK_H
#define _DYNLINK_H

PVOID
dynlink_getProcAddress(
    PVOID ModuleBase,
    PCHAR pFunctionName
    );

PVOID
dynlink_getModuleBase(
    PCHAR  pModuleName
    );


#endif

