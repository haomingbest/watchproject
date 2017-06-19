#pragma once
#include "recordBase.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
class process_io_Record :public RecordBase
{
public:
	process_io_Record(HANDLE _hProcess);
	virtual ~process_io_Record();

	virtual void init();
	virtual void Tick(int TickTime);

private:
	HANDLE hProcess;
	IO_COUNTERS ic;
};