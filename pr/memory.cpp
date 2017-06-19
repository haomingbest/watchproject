#include "memory.h"
#ifndef UNICODE
#define UNICODE
#endif

MemoryRecord::MemoryRecord()
{
}
MemoryRecord::~MemoryRecord()
{
}

void MemoryRecord::init()
{
}
void MemoryRecord::Tick(int TickTime)
{
	if (GetPerformanceInfo(&pi, sizeof(pi)))
	{		
		/*printf("mem : %d/%d commit %d/%d\n",
			(pi.PhysicalTotal - pi.PhysicalAvailable)*pi.PageSize / 1024 / 1024,
			pi.PhysicalTotal*pi.PageSize / 1024 / 1024,
			pi.CommitPeak *pi.PageSize / 1024 / 1024,
			pi.CommitTotal*pi.PageSize / 1024 / 1024);

		printf("HandleCount : %d ProcessCount : %d ThreadCount : %d\n", pi.HandleCount, pi.ProcessCount, pi.ThreadCount);*/
	
		mData["Physical Mem(KB)"] = (pi.PhysicalTotal - pi.PhysicalAvailable) / 1024 * pi.PageSize / 1024;
		mData["Commit Mem(KB)"] = pi.CommitTotal / 1024  * pi.PageSize / 1024;
		mData["HandleCount"] = pi.HandleCount;
		mData["ProcessCount"] = pi.ProcessCount;
		mData["ThreadCount"] = pi.ThreadCount;
	}
	else
	{
		printf("ERROR GetPerformanceInfo\n");
	}
}