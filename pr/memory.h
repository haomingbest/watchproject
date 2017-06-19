#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <psapi.h>
#include "recordBase.h"
class MemoryRecord :public RecordBase
{
public:
	MemoryRecord();
	virtual ~MemoryRecord();

	virtual void init();
	virtual void Tick(int TickTime);

private:

	PERFORMANCE_INFORMATION pi;
};