#include <winsock2.h>
#include <ws2tcpip.h>

#include <iphlpapi.h>

#include <stdio.h>
#include <stdlib.h>

#pragma comment (lib, "Iphlpapi.lib") 
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
/* Note: could also use malloc() and free() */


int GetTargetAdapter()
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	UINT i;

	/* variables used to print DHCP time info */
	struct tm newtime;
	char buffer[32];
	errno_t error;

	ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof (IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
		return 1;
	}
	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		//�豸��ֻһ�� ��Ҫɸѡ
		//���·�����������ڴ�
	
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return 1;
		}

		//�ȴ�ӡ����ȫ����
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

				pAdapter = pAdapter->Next;
				printf("\n");
			}
		}
		else {
			printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);

		}

		//ѡһ��
		int _index;
		printf("Enter the interface index you would like to: ");
		scanf("%d", &_index);


		if (pAdapterInfo)
			FREE(pAdapterInfo);

		return _index;
	}
	else
	{
		//ֻ��һ���豸 ������
		printf("GetAdapter Name: %s\n", pAdapterInfo->Description);
		int _index = pAdapterInfo->Index;
		if (pAdapterInfo)
			FREE(pAdapterInfo);
		return _index;
	}
}

int main()
{
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;
	MIB_IFROW IfRow, _IfRow;	

	int adapterIndex = GetTargetAdapter();
	if (adapterIndex != -1)
	{
		_IfRow.dwIndex = adapterIndex;
		if ((dwRetVal = GetIfEntry(&_IfRow)) == NO_ERROR) {
			
		}
		else
		{
			printf("GetIfEntry failed for index %d with error: %ld\n",
				adapterIndex, dwRetVal);
		}
	}

	while (true)
	{
		//net 
		if (adapterIndex != -1)
		{
			IfRow.dwIndex = adapterIndex;
			if ((dwRetVal = GetIfEntry(&IfRow)) == NO_ERROR) {				
				
				printf("���� : %10d kbps  ", (IfRow.dwOutOctets - _IfRow.dwOutOctets) * 8 / 1024);
				printf("���� : %10d kbps  \n", (IfRow.dwInOctets - _IfRow.dwInOctets) * 8 / 1024);
				//printf("dwInUcastPkts   : %d\n", IfRow.dwInUcastPkts);
				//printf("dwOutUcastPkts   : %d\n", IfRow.dwOutUcastPkts);

				_IfRow = IfRow;
			}
			else
			{
				printf("GetIfEntry failed for index %d with error: %ld\n",
					adapterIndex, dwRetVal);
			}
		}

		Sleep(1000);
	}

	exit(0);
}
