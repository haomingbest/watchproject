#include "network.h"


#pragma comment (lib, "Iphlpapi.lib") 
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

NetWorkRecord::NetWorkRecord()
{
	init();
}

NetWorkRecord::~NetWorkRecord()
{

}

int NetWorkRecord::GetTargetAdapter()
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	UINT i;

	/* variables used to print DHCP time info */	

	ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof (IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
		return 1;
	}
	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		//设备不只一个 需要筛选
		//重新分配接受数据内存

		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return 1;
		}

		//先打印出来全部的
		if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
			pAdapter = pAdapterInfo;
			while (pAdapter) {
				printf("Index: %d", pAdapter->Index);
				printf("\tAdapter Desc: %s", pAdapter->Description);
				printf("\tAdapter Addr: ");
				for (i = 0; i < pAdapter->AddressLength; i++) {
					if (i == (pAdapter->AddressLength - 1))
						printf("%.2X\n", (int)pAdapter->Address[i]);
					else
						printf("%.2X-", (int)pAdapter->Address[i]);
				}
				printf("\tIP Address: \t%s\n",
					pAdapter->IpAddressList.IpAddress.String);

				pAdapter = pAdapter->Next;
				printf("\n");
			}
		}
		else {
			printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);

		}

		//选一个
		int _index;
		printf("Enter the interface index you would like to: ");
		scanf("%d", &_index);


		if (pAdapterInfo)
			FREE(pAdapterInfo);

		return _index;
	}
	else
	{
		//只有一个设备 就他了
		printf("GetAdapter Name: %s\n", pAdapterInfo->Description);
		int _index = pAdapterInfo->Index;
		if (pAdapterInfo)
			FREE(pAdapterInfo);
		return _index;
	}
}

void NetWorkRecord::init()
{
	DWORD dwRetVal = 0;
	adapterIndex = GetTargetAdapter();
	if (adapterIndex != -1)
	{
		IfRow.dwIndex = adapterIndex;
		if ((dwRetVal = GetIfEntry(&IfRow)) == NO_ERROR) {

		}
		else
		{
			printf("GetIfEntry failed for index %d with error: %ld\n",
				adapterIndex, dwRetVal);
		}
	}
}

void NetWorkRecord::Tick(int TickTime)
{
	DWORD dwRetVal = 0;
	MIB_IFROW _IfRow;
	if (adapterIndex != -1)
	{
		_IfRow.dwIndex = adapterIndex;
		if ((dwRetVal = GetIfEntry(&_IfRow)) == NO_ERROR)
		{
			float secend = TickTime / 1000;
			mData["发送(KB/s)"] = (_IfRow.dwOutOctets - IfRow.dwOutOctets)  / 1024 / secend;
			mData["接收(KB/s)"] = (_IfRow.dwInOctets - IfRow.dwInOctets) / 1024 / secend;
			mData["发送错误"] = (_IfRow.dwOutErrors - IfRow.dwOutErrors);
			mData["接收错误"] = (_IfRow.dwInErrors - IfRow.dwInErrors);
			mData["发送丢弃"] = (_IfRow.dwOutDiscards - IfRow.dwOutDiscards);
			mData["接收丢弃"] = (_IfRow.dwInDiscards - IfRow.dwInDiscards);
			mData["发送单播/s"] = (_IfRow.dwOutUcastPkts - IfRow.dwOutUcastPkts) / secend;
			mData["接收单播/s"] = (_IfRow.dwInUcastPkts - IfRow.dwInUcastPkts) / secend;
			//mData["发送队列长度"] = _IfRow.dwOutQLen;
			IfRow = _IfRow;
		}
		else
		{
			printf("GetIfEntry failed for index %d with error: %ld\n",
				adapterIndex, dwRetVal);
		}
	}
}