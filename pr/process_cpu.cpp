#include "process_cpu.h"

process_cpu_Record::process_cpu_Record(HANDLE _hProcess)
{
	hProcess = _hProcess;
	init();
}
process_cpu_Record::~process_cpu_Record()
{}

void process_cpu_Record::init()
{
	if (GetProcessTimes(hProcess, &createTime, &exitTime, &KernelTime, &UserTime))
	{
	}
	else
	{
		printf("ERROR process_cpu_Record\n");
	}
}
void process_cpu_Record::Tick(int TickTime)
{
	FILETIME _createTime;
	FILETIME _exitTime;
	FILETIME _KernelTime;
	FILETIME _UserTime;
	if (GetProcessTimes(hProcess, &_createTime, &_exitTime, &_KernelTime, &_UserTime))
	{
		ULARGE_INTEGER  uliKernelTime, uliUserTime;
		uliKernelTime = getFIleTime_difference(KernelTime, _KernelTime);
		uliUserTime = getFIleTime_difference(UserTime, _UserTime);
		float secend = TickTime / 1000;
		mData["cpu%"] = (uliUserTime.QuadPart + uliKernelTime.QuadPart) / 100000 / secend;//

		UserTime = _UserTime;
		KernelTime = _KernelTime;

	}
	else
	{
		printf("ERROR process_cpu_Record\n");
	}
}