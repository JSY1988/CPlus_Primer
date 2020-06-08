#include "IPLocator.cpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <windows.h>
#include <sstream>
#include <iostream>
#include <string>
#include <direct.h>
#include <tlhelp32.h>
#include <time.h>
#include "tchar.h"
#include "stdio.h"
#include "psapi.h"
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define STATUS_FALIED 0xFFFF
unsigned long serverIP;
long MaxThread = 200;
long ThreadCount = 0;
long *AA = &ThreadCount;
using namespace std;
extern string getTime()
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
    return tmp;
}
extern BOOL TerminateProcessFromId(DWORD dwId)
{
    BOOL bRet = FALSE;
    DWORD dwExitCode = 0;
    //��ý��̾��
    HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);
    if (hProcess != NULL)
    {
        //����˳�����
        ::GetExitCodeProcess(hProcess, &dwExitCode);
        if (dwExitCode == STILL_ACTIVE)
        {
            //��������
            bRet = ::TerminateProcess(hProcess, dwExitCode);
        }
    }
    //�رս��̾��
    CloseHandle(hProcess);
    return bRet;
}
extern DWORD WINAPI ScanPort(LPVOID lpParam)
{
    short Port = *(short *)lpParam;
    InterlockedIncrement(AA);
    //������ʽ�׽���
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        cout << "��ʼ��ʧ��" << endl;
        return 0;
    }
    else
    {
        //����������ַ
        sockaddr_in severAddr;
        severAddr.sin_family = AF_INET;
        severAddr.sin_port = htons(Port);
        severAddr.sin_addr.S_un.S_addr = serverIP;
        //�жϴ˻����Ƿ��
        connect(sock, (sockaddr *)&severAddr, sizeof(severAddr));
        struct fd_set write;
        FD_ZERO(&write);
        FD_SET(sock, &write);
        //��ʼ����ʱʱ��
        struct timeval timeout;
        timeout.tv_sec = 100 / 1000;
        timeout.tv_usec = 0;
        if (select(0, NULL, &write, NULL, &timeout) > 0)
        {
            cout << Port << ",";
        };
        closesocket(sock);
    }

    InterlockedDecrement(AA);
    return 0;
}
extern int Tcpview()
{
    const int MAXPATH = 255;
    char buffer[MAXPATH];
    getcwd(buffer, MAXPATH);
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
                    string time = getTime();
                    cout << "ʱ��    : " << time << endl;
                    cout << "���� PID: " << pTcpTable->table[i].dwOwningPid << endl;
                    printf("��������: %s\n", Buffer);
                    printf("����  IP: %s\n", szLocalAddr);
                    printf("���ض˿�: %d\n", ntohs((u_short)pTcpTable->table[i].dwLocalPort));
                    printf("Զ��  IP: %s\n", szRemoteAddr);
                    printf("Զ�̶˿�: %d\n", ntohs((u_short)pTcpTable->table[i].dwRemotePort));
                    cout << "Զ�̵�ַ: " << ipl.getIpAddr(szRemoteAddr) << std::endl;
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
extern BOOL GetProcessList()
{
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32))
    {
        CloseHandle(hProcessSnap);
        return (FALSE);
    }
    do
    {
        printf("��������= %s \n", pe32.szExeFile);
        printf("���� PID= %d \n", pe32.th32ProcessID);
        printf("�߳���  = %d \n", pe32.cntThreads);
        printf("\n");
    } while (Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);
    return (TRUE);
}
