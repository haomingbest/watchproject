// Need to link with Iphlpapi.lib and Ws2_32.lib
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

/* Note: could also use malloc() and free() */

int main()
{

	// Declare and initialize variables
	PMIB_TCPTABLE_OWNER_PID  pTcpTable;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	char szLocalAddr[128];
	char szRemoteAddr[128];

	struct in_addr IpAddr;

	int i;

	pTcpTable = (MIB_TCPTABLE_OWNER_PID  *)MALLOC(sizeof (MIB_TCPTABLE_OWNER_PID));
	if (pTcpTable == NULL) {
		printf("Error allocating memory\n");
		return 1;
	}

	dwSize = sizeof (MIB_TCPTABLE_OWNER_PID);
	// Make an initial call to GetTcpTable to
	// get the necessary size into the dwSize variable
	if ((dwRetVal = GetExtendedTcpTable(pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_CONNECTIONS, 0)) ==
		ERROR_INSUFFICIENT_BUFFER) {
		FREE(pTcpTable);
		pTcpTable = (MIB_TCPTABLE_OWNER_PID  *)MALLOC(dwSize);
		if (pTcpTable == NULL) {
			printf("Error allocating memory\n");
			return 1;
		}
	}
	// Make a second call to GetTcpTable to get
	// the actual data we require
	if ((dwRetVal = GetExtendedTcpTable(pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_CONNECTIONS, 0)) == NO_ERROR) {
		printf("Number of entries: %d", (int)pTcpTable->dwNumEntries);
		for (i = 0; i < (int)pTcpTable->dwNumEntries; i++) {
			IpAddr.S_un.S_addr = (u_long)pTcpTable->table[i].dwLocalAddr;
			strcpy_s(szLocalAddr, sizeof (szLocalAddr), inet_ntoa(IpAddr));
			IpAddr.S_un.S_addr = (u_long)pTcpTable->table[i].dwRemoteAddr;
			strcpy_s(szRemoteAddr, sizeof (szRemoteAddr), inet_ntoa(IpAddr));

			printf("\nTCP[%4d] pid %4d   State: %3ld - ", i, pTcpTable->table[i].dwOwningPid,
				pTcpTable->table[i].dwState);
			switch (pTcpTable->table[i].dwState) {
			case MIB_TCP_STATE_CLOSED:
				printf("CLOSED");
				break;
			case MIB_TCP_STATE_LISTEN:
				printf("LISTEN");
				break;
			case MIB_TCP_STATE_SYN_SENT:
				printf("SYN-SENT");
				break;
			case MIB_TCP_STATE_SYN_RCVD:
				printf("SYN-RECEIVED");
				break;
			case MIB_TCP_STATE_ESTAB:
				printf("ESTABLISHED");
				break;
			case MIB_TCP_STATE_FIN_WAIT1:
				printf("FIN-WAIT-1");
				break;
			case MIB_TCP_STATE_FIN_WAIT2:
				printf("FIN-WAIT-2");
				break;
			case MIB_TCP_STATE_CLOSE_WAIT:
				printf("CLOSE-WAIT");
				break;
			case MIB_TCP_STATE_CLOSING:
				printf("CLOSING");
				break;
			case MIB_TCP_STATE_LAST_ACK:
				printf("LAST-ACK");
				break;
			case MIB_TCP_STATE_TIME_WAIT:
				printf("TIME-WAIT");
				break;
			case MIB_TCP_STATE_DELETE_TCB:
				printf("DELETE-TCB");
				break;
			default:
				printf("UNKNOWN dwState value");
				break;
			}
			printf("\tLocal  %s", szLocalAddr);
			printf(" : %5d", ntohs((u_short)pTcpTable->table[i].dwLocalPort));
			printf("\tRemote  %s", szRemoteAddr);
			printf(" : %5d", ntohs((u_short)pTcpTable->table[i].dwRemotePort));
		}
	}
	else {
		printf("\tGetTcpTable failed with %d\n", dwRetVal);
		FREE(pTcpTable);
		return 1;
	}

	if (pTcpTable != NULL) {
		FREE(pTcpTable);
		pTcpTable = NULL;
	}
	while (1)
	{
		Sleep(1);
	}
	return 0;
}
