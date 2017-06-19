#pragma once

#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
#include "recordBase.h"

class DiskRecord : public RecordBase
{

public:
	DiskRecord();
	virtual ~DiskRecord();

	virtual void init();
	virtual void Tick(int TickTime);

private:

	//disk
	//std::wstring  wszDrive;
	HANDLE hDiskDevice;
	DISK_PERFORMANCE dp;
	BOOL GetDevicePerformance(LPSTR wszPath, DISK_PERFORMANCE &dp, HANDLE &hDevice);
};