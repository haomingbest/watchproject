
#include "network.h"
#include "disk.h"
#include "cpu.h"
#include "memory.h"
#include <vector>
#include "recordBase.h"
class hostManager
{
public:
	hostManager();
	~hostManager();

	void initHostRecord();
	void Tick(int TickTime);
	void printRecordData();
	void writeFileRecordData();
	bool bWriteToFile;
	
private:
	std::vector<RecordBase*> vRecord;
	std::string outFileName;
	FILE* outFileHandle = NULL;
	std::string nowTimeString;
};