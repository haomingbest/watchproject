#ifndef UNICODE
#define UNICODE
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <psapi.h>
#include <windows.h>

#pragma comment(lib, "iphlpapi.lib")
int main()
{
	FILETIME _IdelTime, _KernelTime, _UserTime;
	ULARGE_INTEGER uliIdelTime, uliKernelTime, uliUserTime;
	ULARGE_INTEGER luliIdelTime, luliKernelTime, luliUserTime;
	

	//init
	GetSystemTimes(&_IdelTime, &_KernelTime, &_UserTime);
	
	//loop
	while (1)
	{
		//cpu
		FILETIME IdelTime, KernelTime, UserTime;
		if (GetSystemTimes(&IdelTime, &KernelTime, &UserTime))
		{
			uliKernelTime.HighPart = KernelTime.dwHighDateTime - _KernelTime.dwHighDateTime;
			uliKernelTime.LowPart = KernelTime.dwLowDateTime - _KernelTime.dwLowDateTime;
			uliUserTime.HighPart = UserTime.dwHighDateTime - _UserTime.dwHighDateTime;
			uliUserTime.LowPart = UserTime.dwLowDateTime - _UserTime.dwLowDateTime;
			printf("(cpu :%llu %llu )\n",
				(uliKernelTime.QuadPart) / 1000000,
				(uliUserTime.QuadPart) / 1000000);
			_IdelTime = IdelTime;
			_KernelTime = KernelTime;
			_UserTime = UserTime;
		}

		//mem handelcount...
		PERFORMANCE_INFORMATION pi;
		if (GetPerformanceInfo(&pi, sizeof(pi)))
		{
			LONG64 PhysicalTotal = pi.PhysicalTotal * pi.PageSize;
			printf("mem : %d/%d commit %d/%d\n",
				(pi.PhysicalTotal - pi.PhysicalAvailable)*pi.PageSize / 1024 / 1024,
				pi.PhysicalTotal*pi.PageSize / 1024 / 1024,
				pi.CommitPeak *pi.PageSize / 1024 / 1024,
				pi.CommitTotal*pi.PageSize / 1024 / 1024);
			printf("HandleCount : %d ProcessCount : %d ThreadCount : %d\n", pi.HandleCount, pi.ProcessCount, pi.ThreadCount);
		}

		//io


		//net

		Sleep(1000);


	}


}