#include "IPLocator.cpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <windows.h>
#include "tchar.h"
#include "stdio.h"
#include "psapi.h"
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
#include <sstream>
#include <iostream>
#include <string>
using namespace std;
using std::cerr;
using std::cout;
using std::endl;
using std::ios;
int main()
{
	IPLocator ipl("QQWry.dat");
	PMIB_TCPTABLE_OWNER_PID pTcpTable;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;
	char szLocalAddr[128];
	char szRemoteAddr[128];
	struct in_addr IpAddr;
	int i = 0;
	pTcpTable = (MIB_TCPTABLE_OWNER_PID *)MALLOC(sizeof(MIB_TCPTABLE_OWNER_PID));
	if (pTcpTable == NULL)
	{
		return 1;
	}
	dwSize = sizeof(MIB_TCPTABLE_OWNER_PID);
	if ((dwRetVal = GetExtendedTcpTable(pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0)) == ERROR_INSUFFICIENT_BUFFER)
	{
		FREE(pTcpTable);
		pTcpTable = (MIB_TCPTABLE_OWNER_PID *)MALLOC(dwSize);
		if (pTcpTable == NULL)
		{
			return 1;
		}
	}
	if ((dwRetVal = GetExtendedTcpTable(pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0)) == NO_ERROR)
	{

		for (i = 0; i < (int)pTcpTable->dwNumEntries; i++)
		{
			HANDLE Handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pTcpTable->table[i].dwOwningPid);
			if (Handle)
			{
				TCHAR Buffer[MAX_PATH];
				if (GetModuleFileNameEx(Handle, 0, Buffer, MAX_PATH))
				{
					IpAddr.S_un.S_addr = (u_long)pTcpTable->table[i].dwLocalAddr;
					strcpy_s(szLocalAddr, sizeof(szLocalAddr), inet_ntoa(IpAddr));
					IpAddr.S_un.S_addr = (u_long)pTcpTable->table[i].dwRemoteAddr;
					strcpy_s(szRemoteAddr, sizeof(szRemoteAddr), inet_ntoa(IpAddr));
					printf("\tProgram    name:%s\n", Buffer);
					printf("\tLocal        IP: %s\n", szLocalAddr);
					printf("\tLocal      port: %d\n", ntohs((u_short)pTcpTable->table[i].dwLocalPort));
					printf("\tRemotely     IP: %s\n", szRemoteAddr);
					printf("\tRemote     port: %d\n", ntohs((u_short)pTcpTable->table[i].dwRemotePort));
					string strRemAddr(szRemoteAddr);
					cout << "\tRemote address: " << ipl.getIpAddr(strRemAddr) << endl;
					printf("\t\n");
				}
			}
		}
	}
	else
	{
		FREE(pTcpTable);
		return 1;
	}
	if (pTcpTable != NULL)
	{
		FREE(pTcpTable);
		pTcpTable = NULL;
	}
	system("pause");
	return 0;
}
