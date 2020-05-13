#pragma once

/********************************************************************
	created:	2016/01/21
	created:	21:1:2016   15:27
	file base:	XDisplayDevice
	file ext:	h
	author:		zhangweifang

	purpose:	��ʾ�豸���API
*********************************************************************/
#ifndef _X_DISPLAY_DEVICE_H_
#define _X_DISPLAY_DEVICE_H_
//////////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include <string>
using namespace std;
//////////////////////////////////////////////////////////////////////////
// ��ȡ��ǰ����ʹ�õ�Monitor
BOOL
XDD_GetActiveAttachedMonitor(
	OUT DISPLAY_DEVICE &ddMonitor                // ���ddMonitor��Ϣ
);

// ����DeviceID�õ�LEN0028�Լ�{4d36e96e-e325-11ce-bfc1-08002be10318}\0001
// DeviceID:MONITOR\LEN0028\{4d36e96e-e325-11ce-bfc1-08002be10318}\0001
BOOL
XDD_GetModelDriverFromDeviceID(
	IN  LPCWSTR lpDeviceID,                      // DeviceID
	OUT wstring &strModel,                       // ����ͺţ�����LEN0028
	OUT wstring &strDriver                       // ���������Ϣ������{4d36e96e-e325-11ce-bfc1-08002be10318}\0001
);

// ����Model�ж�EDID�����Ƿ���ȷ
BOOL
XDD_IsCorrectEDID(
	IN  const BYTE *pEDIDBuf,                    // EDID���ݻ�����
	IN  DWORD dwcbBufSize,                       // �����ֽڴ�С
	IN  LPCWSTR lpModel                          // �ͺ�
);

// ����Model��Driver��Ϣȡ��EDID����
BOOL
XDD_GetDeviceEDID(
	IN  LPCWSTR lpModel,                         // �ͺ�
	IN  LPCWSTR lpDriver,                        // Driver
	OUT BYTE *pDataBuf,                          // ���EDID���ݻ�����
	IN DWORD dwcbBufSize,                        // ����������ֽڴ�С������С��256
	OUT DWORD *pdwGetBytes = NULL                // ʵ�ʻ���ֽ���
);

// ��ȡ��ǰMonitor������ߴ磬��λCM
BOOL
XDD_GetActiveMonitorPhysicalSize(
	OUT DWORD &dwWidth,                          // �����ȣ���λCM
	OUT DWORD &dwHeight,                         // ����߶ȣ���λCM
	CHAR *tempPath //����ļ�·��

);



//////////////////////////////////////////////////////////////////////////
#endif