#pragma once

/********************************************************************
	created:	2016/01/21
	created:	21:1:2016   15:27
	file base:	XDisplayDevice
	file ext:	h
	author:		zhangweifang

	purpose:	显示设备相关API
*********************************************************************/
#ifndef _X_DISPLAY_DEVICE_H_
#define _X_DISPLAY_DEVICE_H_
//////////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include <string>
using namespace std;
//////////////////////////////////////////////////////////////////////////
// 获取当前正在使用的Monitor
BOOL
XDD_GetActiveAttachedMonitor(
	OUT DISPLAY_DEVICE &ddMonitor                // 输出ddMonitor信息
);

// 解析DeviceID得到LEN0028以及{4d36e96e-e325-11ce-bfc1-08002be10318}\0001
// DeviceID:MONITOR\LEN0028\{4d36e96e-e325-11ce-bfc1-08002be10318}\0001
BOOL
XDD_GetModelDriverFromDeviceID(
	IN  LPCWSTR lpDeviceID,                      // DeviceID
	OUT wstring &strModel,                       // 输出型号，比如LEN0028
	OUT wstring &strDriver                       // 输出驱动信息，比如{4d36e96e-e325-11ce-bfc1-08002be10318}\0001
);

// 根据Model判断EDID数据是否正确
BOOL
XDD_IsCorrectEDID(
	IN  const BYTE *pEDIDBuf,                    // EDID数据缓冲区
	IN  DWORD dwcbBufSize,                       // 数据字节大小
	IN  LPCWSTR lpModel                          // 型号
);

// 根据Model及Driver信息取得EDID数据
BOOL
XDD_GetDeviceEDID(
	IN  LPCWSTR lpModel,                         // 型号
	IN  LPCWSTR lpDriver,                        // Driver
	OUT BYTE *pDataBuf,                          // 输出EDID数据缓冲区
	IN DWORD dwcbBufSize,                        // 输出缓冲区字节大小，不可小于256
	OUT DWORD *pdwGetBytes = NULL                // 实际获得字节数
);

// 获取当前Monitor的物理尺寸，单位CM
BOOL
XDD_GetActiveMonitorPhysicalSize(
	OUT DWORD &dwWidth,                          // 输出宽度，单位CM
	OUT DWORD &dwHeight,                         // 输出高度，单位CM
	CHAR *tempPath //输出文件路径

);



//////////////////////////////////////////////////////////////////////////
#endif