#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include "recordBase.h"

class NetWorkRecord :public RecordBase
{
public:
	NetWorkRecord();
	virtual ~NetWorkRecord();

	virtual void init();
	virtual void Tick(int TickTime);

private:

	int GetTargetAdapter();
	int adapterIndex;
	MIB_IFROW IfRow;
};