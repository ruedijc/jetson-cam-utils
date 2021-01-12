#ifndef XIAPIWRAPPER_H
#define XIAPIWRAPPER_H

#include <xiCoreGlobal.h>
#include <xiApi.h>

struct SxCameraInfo;
class CxHsiCameraParams;
class CxValue;

uint XICORE_API xiApi_GetNumberDevices();

bool XICORE_API xiApi_GetDeviceInfo(uint uiDevIdx, SxCameraInfo &info);

bool XICORE_API xiApi_SetDeviceInfoString(uint uiDevIdx, const char* sParam, QString &sValue);

bool XICORE_API xiApi_GetSerialNumber(uint uiDevIdx, QString &sSerialNumber);

XICORE_HANDLE XICORE_API xiApi_OpenDevice(uint uiDevIdx);

bool XICORE_API xiApi_CloseDevice(XICORE_HANDLE hDevice);

bool XICORE_API xiApi_StartAcquisition(XICORE_HANDLE hDevice);

bool XICORE_API xiApi_StopAcquisition(XICORE_HANDLE hDevice);

bool XICORE_API xiApi_GetImage(XICORE_HANDLE hDevice, uint uiTimeout, XI_IMG *aXiImg, XI_RETURN *piRetVal = NULL, bool bNotifyFailed = true);

bool XICORE_API xiApi_SetParamInt(XICORE_HANDLE hDevice, const char *szParam, const int iVal, XI_RETURN *piRetVal = NULL);

bool XICORE_API xiApi_SetParamFloat(XICORE_HANDLE hDevice, const char *szParam, const float fVal, XI_RETURN *piRetVal = NULL);

bool XICORE_API xiApi_SetParamString(XICORE_HANDLE hDevice, const char *szParam, char *szVal, uint uiValSize, XI_RETURN *piRetVal = NULL);

bool XICORE_API xiApi_GetParamInt(XICORE_HANDLE hDevice, const char *szParam, int &iVal, XI_RETURN *piRetVal = NULL);

bool XICORE_API xiApi_GetParamFloat(XICORE_HANDLE hDevice, const char *szParam, float &fVal, XI_RETURN *piRetVal = NULL);

bool XICORE_API xiApi_GetParamString(XICORE_HANDLE hDevice, const char *szParam, QByteArray &sVal, XI_RETURN *piRetVal = NULL);

bool XICORE_API xiApi_GetParamUnknownType(XICORE_HANDLE hDevice, const char *szParam, CxValue *pRet, XI_RETURN *piRetVal = NULL);

bool XICORE_API xiApi_ReadHsiParams(XICORE_HANDLE hDevice, const SxCameraInfo &info, CxHsiCameraParams *pHsiParams, const QString &sFallbackXmlFolder);

bool XICORE_API xiApiHsi_IsCameraHsi(XICORE_HANDLE hDevice);

#endif // XIAPIWRAPPER_H
