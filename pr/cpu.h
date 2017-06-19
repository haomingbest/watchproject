#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <psapi.h>
#include "recordBase.h"

class CpuRecord :public RecordBase
{
public:
	CpuRecord();
	virtual ~CpuRecord();

	virtual void init();
	virtual void Tick(int TickTime);

private:
	FILETIME IdelTime, KernelTime, UserTime;	
	
};