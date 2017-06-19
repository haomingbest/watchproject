#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <conio.h>
// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

void PrintProcessNameAndID(DWORD processID)
{
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

	// Get a handle to the process.

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION
		| PROCESS_VM_READ,
		FALSE, processID);

	
	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		// Get the process name.

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
			&cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName,
				sizeof(szProcessName) / sizeof(TCHAR));
		}
		else
		{
			CloseHandle(hProcess);
			return;
		}
		// Print the process name and identifier.

		_tprintf(TEXT("%s  (PID: %u) "), szProcessName, processID);

		// Get mem info
		PROCESS_MEMORY_COUNTERS pmc;
		if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
		{
			printf("(mem : %d %d )", pmc.PagefileUsage / 1024 / 1024, pmc.PeakPagefileUsage / 1024 / 1024);
		}
		else
		{
			printf("(mem : %d %d )", -1, -1);
		}
		//Get IO
		IO_COUNTERS ic;
		if(GetProcessIoCounters(hProcess, &ic))
		{
			printf("(io : %llu %llu )", ic.ReadTransferCount,ic.WriteTransferCount);
		}
		else
		{
			printf("(io : %d %d )", -1, -1);
		}
		//Get cpu
		FILETIME createTime; 
		FILETIME exitTime;
		FILETIME KernelTime;
		FILETIME UserTime;
		ULARGE_INTEGER  uliKernelTime, uliUserTime;
		if (GetProcessTimes(hProcess, &createTime, &exitTime, &KernelTime, &UserTime))
		{
			uliKernelTime.HighPart = KernelTime.dwHighDateTime;
			uliKernelTime.LowPart = KernelTime.dwLowDateTime;
			uliUserTime.HighPart = UserTime.dwHighDateTime;
			uliUserTime.LowPart = UserTime.dwLowDateTime;
			printf("(cpu :%llu %llu )", uliKernelTime.QuadPart, uliUserTime.QuadPart);
		}
		else
		{
			printf("(cpu : %d %d ) ", -1, -1);
		}


		printf("\n");
	}
		
	

	// Release the handle to the process.

	
	CloseHandle(hProcess);
}

int main(void)
{
	// Get the list of process identifiers.

	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return 1;
	}


	// Calculate how many process identifiers were returned.

	cProcesses = cbNeeded / sizeof(DWORD);

	// Print the name and process identifier for each process.

	for (i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] != 0)
		{
			PrintProcessNameAndID(aProcesses[i]);
		}
	}

	while (!_kbhit())
	{
		Sleep(1);
	}
	return 0;
}
