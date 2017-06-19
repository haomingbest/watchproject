#pragma once
#include "process_cpu.h"
#include "process_memory.h"
#include "process_io.h"
#include <vector>
#include <list>
#include <map>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>

class processManager
{
public:
	processManager();
	processManager(std::vector<std::string>& vWatchName);
	virtual ~processManager();
	

	bool getWatchProcess(std::vector<std::string>& vWatchName);
	

	void printAllWatchProcess();
	void printAllWatchProcessData();

	void writeFileAllWatchProcessData();
	void Tick(int TickTime);

	bool bWriteToFile;
private:
	struct watchProcessData
	{
		DWORD pid;
		std::string name;
		HANDLE openHandle;
		FILE* outFileHandl;
		std::string outFileName;
		FILE* outFileHandle;
		std::vector<RecordBase*> vRecord;
	};
	void initWatchProcess(watchProcessData& wpd);
	void removeProcess(watchProcessData&);
	void writeFileWatchProcessData(watchProcessData&);

	std::map<DWORD,watchProcessData> vWatchProcess;
	std::vector<std::string> vWatchName;
	std::string nowTimeString;
};