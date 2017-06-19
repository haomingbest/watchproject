#include "host.h"

hostManager::hostManager()
{
	initHostRecord();
	outFileName = "";
}
hostManager::~hostManager()
{
	for (auto iter = vRecord.begin(); iter != vRecord.end(); iter++)
	{
		delete (*iter);
	}
	vRecord.clear();
}

void hostManager::initHostRecord()
{
	if (!vRecord.empty())
	{
		vRecord.clear();
	}

	RecordBase* pR;

	pR = new DiskRecord();
	vRecord.push_back(pR);

	pR = new NetWorkRecord();
	vRecord.push_back(pR);

	pR = new CpuRecord();
	vRecord.push_back(pR);

	pR = new MemoryRecord();
	vRecord.push_back(pR);
	
}
void hostManager::Tick(int TickTime)
{
	//
	//getnow
	char timeStr[256];
	SYSTEMTIME now;
	GetLocalTime(&now);
	sprintf_s(timeStr, "%d:%d:%d",
		now.wHour, now.wMinute, now.wSecond);
	nowTimeString = timeStr;

	//
	for (auto iter = vRecord.begin(); iter != vRecord.end(); iter++)
	{
		(*iter)->Tick(TickTime);
	}
}

void hostManager::printRecordData()
{
	printf("Host performance date:");
	for (auto iter = vRecord.begin(); iter != vRecord.end(); iter++)
	{
		printf("\n");
		(*iter)->printNameAndData();
	}
}

void hostManager::writeFileRecordData()
{
	if (outFileName == "")
	{
		char _fileName[256];
		SYSTEMTIME now;
		GetLocalTime(&now);
		sprintf_s(_fileName, "host(%d-%d-%d %d-%d-%d).txt", 
			now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond);
		outFileName = _fileName;

		//第一次打印
		FILE* fp;

		//errno_t err;
		//if ((err = fopen_s(&fp, _wpd.outFileName.c_str(), "w+")) == 0)
		if ((fp = fopen(outFileName.c_str(), "w+")) != NULL)
		{
			outFileHandle = fp;

			std::string outStr = "time\t";
			for (auto piter = vRecord.begin(); piter != vRecord.end(); piter++)
			{
				outStr += (*piter)->getAllName("\t");
			}
			fprintf(outFileHandle, "%s\n", outStr.c_str());
			fflush(outFileHandle);
		}
		else
		{
			printf("打开文件失败 %s\n", outFileName.c_str());
		}
	}
	else
	{
		//打印数据
		if (outFileHandle)
		{
			std::string outStr = nowTimeString + "\t";;
			for (auto piter = vRecord.begin(); piter != vRecord.end(); piter++)
			{
				outStr += (*piter)->getAllData("\t");
			}
			fprintf(outFileHandle, "%s\n", outStr.c_str());
			fflush(outFileHandle);
		}

	}
}