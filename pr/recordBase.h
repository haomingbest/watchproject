#pragma once

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <Windows.h>

class RecordBase
{
public:
	RecordBase(){};
	virtual ~RecordBase(){ mData.clear(); };

	virtual void Tick(int TickTime) = 0;
	virtual void printNameAndData()
	{
		for (auto iter = mData.begin(); iter != mData.end(); iter++)
		{
			//std::wcout<<iter->first<<" = "<<iter->second<<std::endl;
			printf("%s = %lld ", iter->first.c_str(), iter->second);
		}
	}
	virtual long long getData(std::string name)
	{
		if (mData.find(name) != mData.end())
		{
			return mData[name];
		}
		else
		{
			return 0;
		}
	}

	virtual std::string getAllName(std::string inStr)
	{
		std::string outStr;		
		for (auto iter = mData.begin(); iter != mData.end(); iter++)
		{			
			outStr += iter->first;
			outStr += inStr;
		}		
		return outStr;
	}

	virtual std::string getAllData(std::string inStr)
	{
		std::string outStr;
		std::stringstream ss;
		for (auto iter = mData.begin(); iter != mData.end(); iter++)
		{
			ss << iter->second;
			ss << inStr;
		}
		//ss >> outStr;
		outStr = ss.str();
		return outStr;
	}

	virtual long long operator[](std::string name)
	{
		if (mData.find(name) != mData.end())
		{
			return mData[name];
		}
		else
		{
			return 0;
		}
	}
protected:

	virtual void init() = 0;
	std::map<std::string, long long> mData;
};

ULARGE_INTEGER getFIleTime_difference(FILETIME& f1, FILETIME& f2);
