#define UNICODE 1
#define _UNICODE 1

/* The code of interest is in the subroutine GetDriveGeometry. The
code in main shows how to interpret the results of the call. */

#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
#include <conio.h>
#define wszDrive L"\\\\.\\PhysicalDrive0"


HANDLE hDevice = INVALID_HANDLE_VALUE;  // handle to the drive to be examined 
BOOL GetDevicePerformance(LPWSTR wszPath, DISK_PERFORMANCE &dp)
{
	
	BOOL bResult = FALSE;                 // results flag
	DWORD junk = 0;                     // discard results

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		hDevice = CreateFileW(wszPath,          // drive to open
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
int wmain(int argc, wchar_t *argv[])
{
	BOOL bResult = FALSE;      // generic results flag	

	DISK_PERFORMANCE dp,_dp;
	bResult = GetDevicePerformance(wszDrive, _dp);

	
	if (bResult)
	{
		while (1)
		{
			bResult = GetDevicePerformance(wszDrive, dp);
			if (bResult)
			{				
				wprintf(L"BytesRead = %lld\t", (dp.BytesRead.QuadPart - _dp.BytesRead.QuadPart)/1024);
				wprintf(L"BytesWritten = %lld\t", (dp.BytesWritten.QuadPart - _dp.BytesWritten.QuadPart)/1024);				
				wprintf(L"IdleTime = %lld\t", (dp.IdleTime.QuadPart - _dp.IdleTime.QuadPart ) / 10000);
				wprintf(L"QueryTime = %lld\n", (dp.QueryTime.QuadPart - _dp.QueryTime.QuadPart)/10000);

				_dp = dp;
			}
			else
			{
				wprintf(L"GetDrivePerformance failed. Error %ld.\n", GetLastError());
			}
			Sleep(1000);
			
		}		
	}
	else
	{
		wprintf(L"GetDrivePerformance failed. Error %ld.\n", GetLastError());
	}

	
	return ((int)bResult);
}
