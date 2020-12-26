#include "IPLocator.cpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include <time.h>
#include "tchar.h"
#include "psapi.h"
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
using namespace std;
extern string GetTime()
{
    time_t Timep;
    time(&Timep);
    char Tmp[64];
    strftime(Tmp, sizeof(Tmp), "%Y-%m-%d %H:%M:%S", localtime(&Timep));
    return Tmp;
}
extern int main()
{
    IPLocator ipl("QQWry.dat");
    PMIB_TCPTABLE_OWNER_PID pTcpTable;
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;
    char szLocalAddr[128];
    char szRemoteAddr[128];
    struct in_addr IpAddr;
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
        Sleep(1500);
        for (int i = 0; i < (int)pTcpTable->dwNumEntries; i++)
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
                    string time = GetTime();
                    printf("时间    : %s\n", time);
                    printf("进程 PID: %d\n", pTcpTable->table[i].dwOwningPid);
                    printf("进程名称: %s\n", Buffer);
                    printf("本地  IP: %s\n", szLocalAddr);
                    printf("本地端口: %d\n", ntohs((u_short)pTcpTable->table[i].dwLocalPort));
                    printf("远程  IP: %s\n", szRemoteAddr);
                    printf("远程端口: %d\n", ntohs((u_short)pTcpTable->table[i].dwRemotePort));
                    cout << "远程地址: " << ipl.getIpAddr(szRemoteAddr) << std::endl;
                    printf("\n");
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
    return 0;
}
