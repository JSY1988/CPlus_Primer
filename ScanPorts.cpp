#include <iostream>
using namespace std;
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define STATUS_FALIED 0xFFFF
unsigned long serverIP;
long MaxThread = 200;
long ThreadCount = 0;
long *AA = &ThreadCount;

//扫描端口的线程
DWORD WINAPI ScanPort(LPVOID lpParam)
{
    short Port = *(short *)lpParam;
    InterlockedIncrement(AA);
    //创建流式套接字
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        cout << "创建套接字失败！" << endl;
        return 0;
    }
    else
    {
        //填充服务器地址
        sockaddr_in severAddr;
        severAddr.sin_family = AF_INET;
        severAddr.sin_port = htons(Port);
        severAddr.sin_addr.S_un.S_addr = serverIP;
        //判断此机器是否打开
        connect(sock, (sockaddr *)&severAddr, sizeof(severAddr));
        struct fd_set write;
        FD_ZERO(&write);
        FD_SET(sock, &write);
        //初始化超时时间
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

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "请输入目的主机IP地址" << endl;
    }
    //建立与socket库的绑定
    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
    {
        cout << "WSAStartup falied!" << GetLastError() << endl;
        ExitProcess(STATUS_FALIED);
    }
    serverIP = inet_addr(argv[1]);
    cout << "下列端口已开放：" << endl;
    for (int i = 1; i < 1024; i++)
    {
        //超过最大允许线程数等待
        while (ThreadCount >= MaxThread)
            Sleep(10);
        //创建线程，扫描端口
        DWORD ThreadID;
        CreateThread(NULL, 0, ScanPort, (LPVOID) new short(i), 0, &ThreadID);
    }
    //还有没有结束的线程，等待
    while (ThreadCount > 0)
        Sleep(50);

    WSACleanup();
    return 0;
}