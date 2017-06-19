#include "host.h"

#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "iphlpapi.lib")

//#define wszDrive L"\\\\.\\PhysicalDrive0"
DiskRecord::DiskRecord()
{
	init();
}

DiskRecord::~DiskRecord()
{
}

BOOL DiskRecord::GetDevicePerformance(LPSTR wszPath, DISK_PERFORMANCE &dp, HANDLE &hDevice)
{

	BOOL bResult = FALSE;                 // results flag
	DWORD junk = 0;                     // discard results

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		hDevice = CreateFile(wszPath,          // drive to open
			0,                // no access to the drive
			FILE_SHARE_READ | // share mode
			FILE_SHARE_WRITE,
			NULL,             // default security attributes
			OPEN_EXISTING,    // disposition
			0,                // file attributes
			NULL);            // do not copy file attributes

		if (hDevice == INVALID_HANDLE_VALUE)    // cannot open the drive
		{
			return (FALSE);
		}
	}

	bResult = DeviceIoControl(hDevice,                       // device to be queried
		IOCTL_DISK_PERFORMANCE, // operation to perform		
		NULL,
		0,                       // no input buffer
		&dp, sizeof(dp),            // output buffer
		&junk,                         // # bytes returned
		(LPOVERLAPPED)NULL);          // synchronous I/O

	//CloseHandle(hDevice);

	return (bResult);
}


void DiskRecord::init()
{
	BOOL bResult = FALSE;
	//wszDrive = L"\\\\.\\PhysicalDrive0";
	//disk
	hDiskDevice = INVALID_HANDLE_VALUE;
	bResult = GetDevicePerformance("\\\\.\\PhysicalDrive0", dp, hDiskDevice);
	if (!bResult || hDiskDevice == INVALID_HANDLE_VALUE)
	{
		printf("ERROR disk init \n");
		hDiskDevice = INVALID_HANDLE_VALUE;
	}
}

void DiskRecord::Tick(int TickTime)
{
	BOOL bResult = FALSE;

	//disk
	if (hDiskDevice != INVALID_HANDLE_VALUE)
	{
		DISK_PERFORMANCE _dp;
		bResult = GetDevicePerformance("\\\\.\\PhysicalDrive0", _dp, hDiskDevice);
		if (bResult)
		{			
			float secend = TickTime / 1000;
			mData["读取速度 KB/s"] = (_dp.BytesRead.QuadPart - dp.BytesRead.QuadPart) / 1024 / secend;
			mData["写入速度 KB/s"] = (_dp.BytesWritten.QuadPart - dp.BytesWritten.QuadPart) / 1024 / secend;
			long long idle = (_dp.IdleTime.QuadPart - dp.IdleTime.QuadPart) / ((_dp.QueryTime.QuadPart - dp.QueryTime.QuadPart) / 100);
			mData["active%"] = idle >= 100 ? 0 : (100 - idle);

			dp = _dp;
		}
		else
		{
			printf("ERROR GetDevicePerformance\n");
		}
	}
	
}