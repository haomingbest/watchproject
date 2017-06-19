#include "cpu.h"

CpuRecord::CpuRecord()
{
	init();
}
CpuRecord::~CpuRecord()
{
}

void CpuRecord::init()
{
	GetSystemTimes(&IdelTime, &KernelTime, &UserTime);	
	
}
void CpuRecord::Tick(int TickTime)
{
	FILETIME _IdelTime, _KernelTime, _UserTime;
	ULARGE_INTEGER _uliIdelTime, _uliKernelTime, _uliUserTime;	
	if (GetSystemTimes(&_IdelTime, &_KernelTime, &_UserTime))
	{
		_uliIdelTime = getFIleTime_difference(IdelTime, _IdelTime);
		_uliKernelTime = getFIleTime_difference(KernelTime, _KernelTime);
		_uliUserTime = getFIleTime_difference(UserTime, _UserTime);

		long long d = (_uliUserTime.QuadPart + _uliKernelTime.QuadPart - _uliIdelTime.QuadPart) * 100 / (_uliUserTime.QuadPart + _uliKernelTime.QuadPart);
		mData["host_cpu%"] = d;


		IdelTime = _IdelTime;
		KernelTime = _KernelTime;
		UserTime = _UserTime;
		
	}
	else
	{
		printf("ERROR GetSystemTimes\n");
	}
}

