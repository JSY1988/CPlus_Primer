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

//ɨ��˿ڵ��߳�
DWORD WINAPI ScanPort(LPVOID lpParam)
{
    short Port = *(short *)lpParam;
    InterlockedIncrement(AA);
    //������ʽ�׽���
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        cout << "�����׽���ʧ�ܣ�" << endl;
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

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "������Ŀ������IP��ַ" << endl;
    }
    //������socket��İ�
    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
    {
        cout << "WSAStartup falied!" << GetLastError() << endl;
        ExitProcess(STATUS_FALIED);
    }
    serverIP = inet_addr(argv[1]);
    cout << "���ж˿��ѿ��ţ�" << endl;
    for (int i = 1; i < 1024; i++)
    {
        //������������߳����ȴ�
        while (ThreadCount >= MaxThread)
            Sleep(10);
        //�����̣߳�ɨ��˿�
        DWORD ThreadID;
        CreateThread(NULL, 0, ScanPort, (LPVOID) new short(i), 0, &ThreadID);
    }
    //����û�н������̣߳��ȴ�
    while (ThreadCount > 0)
        Sleep(50);

    WSACleanup();
    return 0;
}