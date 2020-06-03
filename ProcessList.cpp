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
    //获得进程句柄
    HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);
    if (hProcess != NULL)
    {
        //获得退出代码
        ::GetExitCodeProcess(hProcess, &dwExitCode);
        if (dwExitCode == STILL_ACTIVE)
        {
            //结束进程
            bRet = ::TerminateProcess(hProcess, dwExitCode);
        }
    }
    //关闭进程句柄
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
        printf("\n   进程名字= %s ", pe32.szExeFile);
        printf("\n   进程ID  = %d", pe32.th32ProcessID);
        printf("\n   线程数  = %d \n", pe32.cntThreads);
    } while (Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);
    return (TRUE);
}

int main()
{
    GetProcessList();

    //DWORD dwProcessId;
    //cin >> dwProcessId;
    //TerminateProcessFromId(dwProcessId); //关闭进程
    system("pause");
    return 0;
}
