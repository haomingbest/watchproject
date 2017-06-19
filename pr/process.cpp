#include "process.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <algorithm>

processManager::processManager()
{
}

processManager::processManager(std::vector<std::string>& _vWatchName)
{
	vWatchName = _vWatchName;
	getWatchProcess(vWatchName);

}
processManager::~processManager()
{
	for (auto iter = vWatchProcess.begin(); iter != vWatchProcess.end(); iter++)
	{
		removeProcess(iter->second);
	}

	vWatchProcess.clear();
}


void processManager::initWatchProcess(watchProcessData& wpd)
{
	RecordBase* rp;

	rp = new process_io_Record(wpd.openHandle);
	wpd.vRecord.push_back(rp);

	rp = new process_cpu_Record(wpd.openHandle);
	wpd.vRecord.push_back(rp);

	rp = new process_memory_Record(wpd.openHandle);
	wpd.vRecord.push_back(rp);

	
}


void processManager::removeProcess(watchProcessData& wpd)
{
	if (wpd.openHandle)
	{
		CloseHandle(wpd.openHandle);
	}
	
	if (wpd.outFileHandle)
	{
		fclose(wpd.outFileHandle);
	}

	
	for (auto piter = wpd.vRecord.begin(); piter != wpd.vRecord.end(); piter++)
	{
		delete (*piter);
	}
	wpd.vRecord.clear();
}

bool processManager::getWatchProcess(std::vector<std::string>& vWatchName)
{
	// Get the list of process identifiers.

	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return false;
	}
	cProcesses = cbNeeded / sizeof(DWORD);

	std::vector<DWORD> _process(aProcesses, aProcesses + cProcesses);
	
	//清除不存在的进程
	if (!vWatchProcess.empty())
	{
		for (auto iter = vWatchProcess.begin(); iter != vWatchProcess.end(); )
		{
			auto fiter = find(_process.begin(), _process.end(), iter->second.pid);
			if (fiter == _process.end())
			{
				removeProcess(iter->second);
				iter = vWatchProcess.erase(iter);
			}
			else
			{				
				iter++;
			}
		}
	}
	// Calculate how many process identifiers were returned.

	

	// Print the name and process identifier for each process.

	for (i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] != 0)
		{
			DWORD processID = aProcesses[i];
					
			if (vWatchProcess.find(processID) != vWatchProcess.end())
			{
				continue;
			}

			CHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION
				| PROCESS_VM_READ,
				FALSE, processID);
			
			if (NULL != hProcess)
			{
				

				//GetModuleFileNameEx(hProcess, NULL, szProcessName, MAX_PATH);
				DWORD _size = MAX_PATH;
				QueryFullProcessImageName(hProcess, 0, szProcessName, &_size);
				std::string _name = szProcessName;
				std::size_t found = _name.find_last_of("/\\");
				_name = _name.substr(found + 1);
				if (find(vWatchName.begin(), vWatchName.end(), _name) == vWatchName.end())
				{
					//不是要找的进程
					CloseHandle(hProcess);
					continue;
				}
				else
				{
					
					watchProcessData* pWpd = new watchProcessData();					
					pWpd->name = _name;
					pWpd->pid = processID;
					pWpd->openHandle = hProcess;
					pWpd->outFileName = "";
					pWpd->outFileHandle = NULL;
					initWatchProcess(*pWpd);
					vWatchProcess[processID] = *pWpd;
					
				}

				//HMODULE hMod;
				//DWORD _cbNeeded;
				//if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
				//	&_cbNeeded))
				//{
				//	GetModuleBaseName(hProcess, hMod, szProcessName,
				//		sizeof(szProcessName) / sizeof(CHAR));
				//	
				//		
				//	
				//	
				//}
				//else
				//{
				//	//没法找到名字不管
				//	CloseHandle(hProcess);
				//	continue;
				//}
				
			}
			else
			{
				//打不开进程不管
				continue;
			}
			Sleep(1);
		}
	}
	return true;
}

void processManager::Tick(int TickTime)
{
	//getnow
	char timeStr[256];
	SYSTEMTIME now;
	GetLocalTime(&now);
	sprintf_s(timeStr, "%d:%d:%d", 
		now.wHour, now.wMinute, now.wSecond);
	nowTimeString = timeStr;

	getWatchProcess(vWatchName);
	//
	for (auto iter = vWatchProcess.begin(); iter != vWatchProcess.end(); iter++)
	{
		for (auto piter = iter->second.vRecord.begin(); piter != iter->second.vRecord.end(); piter++)
		{			
			(*piter)->Tick(TickTime);			
		}
	}
}

void processManager::printAllWatchProcess()
{
	for (auto iter = vWatchProcess.begin(); iter != vWatchProcess.end(); iter++)
	{
		printf("%d %s \n", iter->second.pid, iter->second.name.c_str());
	}
}

void processManager::printAllWatchProcessData()
{

	printf("\n\nProcess performance date:");
	for (auto iter = vWatchProcess.begin(); iter != vWatchProcess.end(); iter++)
	{
		printf("\n%d %s ", iter->second.pid, iter->second.name.c_str());
		for (auto piter = iter->second.vRecord.begin(); piter != iter->second.vRecord.end(); piter++)
		{
			(*piter)->printNameAndData();
		}
	}
}

void processManager::writeFileAllWatchProcessData()
{
	for (auto iter = vWatchProcess.begin(); iter != vWatchProcess.end(); iter++)
	{
		writeFileWatchProcessData(iter->second);
	}
}

void processManager::writeFileWatchProcessData(watchProcessData& _wpd)
{
	if (_wpd.outFileName == "")
	{
		char _fileName[256];
		SYSTEMTIME now;
		GetLocalTime(&now);
		sprintf_s(_fileName, "%s-%d(%d-%d-%d %d-%d-%d).txt", _wpd.name.c_str(), _wpd.pid,
			now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond);
		_wpd.outFileName = _fileName;

		//第一次打印
		FILE* fp;

		//errno_t err;
		//if ((err = fopen_s(&fp, _wpd.outFileName.c_str(), "w+")) == 0)
		if( (fp = fopen(_wpd.outFileName.c_str(), "w+")) != NULL)
		{
			_wpd.outFileHandle = fp;
			
			std::string outStr = "time\t";
			for (auto piter = _wpd.vRecord.begin(); piter != _wpd.vRecord.end(); piter++)
			{
				outStr += (*piter)->getAllName("\t");
			}
			fprintf(_wpd.outFileHandle, "%s\n", outStr.c_str());	
			fflush(_wpd.outFileHandle);
		}
		else
		{
			printf("打开文件失败 %s\n", _wpd.outFileName.c_str());
		}
	}
	else
	{
		//打印数据
		if (_wpd.outFileHandle)
		{
			std::string outStr = nowTimeString + "\t";;
			for (auto piter = _wpd.vRecord.begin(); piter != _wpd.vRecord.end(); piter++)
			{
				outStr += (*piter)->getAllData("\t");
			}
			fprintf(_wpd.outFileHandle, "%s\n", outStr.c_str());	
			fflush(_wpd.outFileHandle);
		}
		
	}
}