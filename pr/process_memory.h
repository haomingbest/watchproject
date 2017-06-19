#pragma once
#include "recordBase.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
class process_memory_Record :public RecordBase
{
public:
	process_memory_Record(HANDLE _hProcess);
	virtual ~process_memory_Record();

	virtual void init();
	virtual void Tick(int TickTime);

private:
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;
	DWORD HandleCount;
};