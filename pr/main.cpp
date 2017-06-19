#include "host.h"
#include "process.h"


long tickTime = 1000;
std::vector<std::string> _watchProcess;
bool bWriteFile = false;
bool bPrintScene = true;

void getConfig()
{
	std::string configFileName = ".//config.ini";

	tickTime = GetPrivateProfileInt("main", "tickTime ", 1000, configFileName.c_str());
	bWriteFile = GetPrivateProfileInt("main", "bWriteFile ", 0, configFileName.c_str()) == 1 ? true : false;
	bPrintScene = GetPrivateProfileInt("main", "bPrintScene", 0, configFileName.c_str()) == 1 ? true : false;
	
	char temp[1024];
	GetPrivateProfileString("main", "process", "", temp, 1024, configFileName.c_str());

	std::string processList = temp;
	std::istringstream iss(processList);
	std::string parameter;
	std::getline(iss, parameter, ',');
	while (parameter != "")
	{
		_watchProcess.push_back(parameter);
		parameter = "";
		std::getline(iss, parameter, ',');
	}
	
}

int main()
{
	//read config
	getConfig();	

	//init
	hostManager* phm = new hostManager();
	
	processManager* ppm = new processManager(_watchProcess);
	ppm->printAllWatchProcess();
		
	//tick
	
	long lastTime = GetTickCount();
	while (true)
	{
		long now = GetTickCount();
		if (now - lastTime > tickTime)
		{
			long _tickTime = now - lastTime;

			//
			phm->Tick(_tickTime);
			ppm->Tick(_tickTime);

			//
			if (bPrintScene)
			{
				system("cls");
				phm->printRecordData();
				ppm->printAllWatchProcessData();
			}
			
			//
			if (bWriteFile)
			{
				phm->writeFileRecordData();
				ppm->writeFileAllWatchProcessData();
			}
			

			//
			lastTime = now;
		}
		
		Sleep(1);

		
	}
	return 0;
}