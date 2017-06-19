#include "process_io.h"

process_io_Record::process_io_Record(HANDLE _hProcess)
{
	hProcess = _hProcess;
	init();
}
process_io_Record::~process_io_Record()
{
	
}

void process_io_Record::init()
{	
	if (GetProcessIoCounters(hProcess, &ic))
	{
		
	}
	else
	{
		printf("ERROR process_io_Record\n");
	}
}
void process_io_Record::Tick(int TickTime)
{
	IO_COUNTERS _ic;
	if (GetProcessIoCounters(hProcess, &_ic))
	{
		float secend = TickTime / 1000;
		mData["io_read"] = (_ic.ReadTransferCount - ic.ReadTransferCount) / secend;
		mData["io_write"] = (_ic.WriteTransferCount - ic.WriteTransferCount) / secend;
	
		ic = _ic;
	}
	else
	{
		printf("ERROR process_io_Record\n");
	}
}