#include <windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <stdio.h>
#include <tchar.h>
using namespace std;
BOOL TerminateProcessFromId(DWORD dwId)
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
BOOL GetProcessList()
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
        printf("\n   ��������= %s ", pe32.szExeFile);
        printf("\n   ����ID  = %d", pe32.th32ProcessID);
        printf("\n   �߳���  = %d \n", pe32.cntThreads);
    } while (Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);
    return (TRUE);
}

int main()
{
    GetProcessList();

    //DWORD dwProcessId;
    //cin >> dwProcessId;
    //TerminateProcessFromId(dwProcessId); //�رս���
    system("pause");
    return 0;
}
