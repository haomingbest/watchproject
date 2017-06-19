#pragma once
#include "recordBase.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
class process_cpu_Record :public RecordBase
{
public:
	process_cpu_Record(HANDLE _hProcess);
	virtual ~process_cpu_Record();

	virtual void init();
	virtual void Tick(int TickTime);

private:

	HANDLE hProcess;

	FILETIME createTime;
	FILETIME exitTime;
	FILETIME KernelTime;
	FILETIME UserTime;
};