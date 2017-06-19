#include "process_memory.h"

process_memory_Record::process_memory_Record(HANDLE _hProcess)
{
	hProcess = _hProcess;
	init();
}
process_memory_Record::~process_memory_Record()
{}

void process_memory_Record::init()
{
	// Get mem info
	
	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		
	}
	else
	{
		printf("ERROR process_memory_Record GetProcessMemoryInfo\n");
	}

	if (GetProcessHandleCount(hProcess, &HandleCount))
	{
	}
	else
	{
		printf("ERROR process_memory_Record GetProcessHandleCount\n");
	}
	
}
void process_memory_Record::Tick(int TickTime)
{	
	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		mData["������(KB)"] = pmc.WorkingSetSize/1024;
		mData["�ύ��С(KB)"] = pmc.PagefileUsage / 1024;
		//mData["ר�ù�����(KB)"] = pmc.QuotaNonPagedPoolUsage / 1024;
		
	}
	else
	{
		printf("ERROR process_memory_Record GetProcessMemoryInfo\n");
	}

	if (GetProcessHandleCount(hProcess, &HandleCount))
	{
		mData["HandleCount"] = HandleCount;
	}
	else
	{
		printf("ERROR process_memory_Record GetProcessHandleCount\n");
	}
}