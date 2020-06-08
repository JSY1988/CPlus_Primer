#include "Net.cpp"
#include "system.cpp"
using namespace std;
int main(int argc, char *argv[])
{

	if (argc < 2)
	{
		Tcpview();
	}
	else if (strcmp(argv[1], "-h") == 0)
	{
		cout << "tcpview [可选] [可选]<输出当前联网的程序>\n";
		cout << "tcpview -l <显示进程>\n";
		cout << "tcpview -c 进程ID <关闭进程>\n";
		cout << "tcpview -p IP <输出IP开放端口>\n"
			 << endl;
	}
	else if (strcmp(argv[1], "-l") == 0)
	{
		GetProcessList();
	}
	else if (strcmp(argv[1], "-c") == 0)
	{
		DWORD dwProcessId;
		dwProcessId = atoi(argv[2]);
		TerminateProcessFromId(dwProcessId); //关闭进程
		system("TrayRefresh.exe");
	}
	else if (strcmp(argv[1], "-p") == 0)
	{
		WSADATA WSAData;
		if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
		{
			cout << "初始化失败" << GetLastError() << endl;
			ExitProcess(STATUS_FALIED);
		}
		serverIP = inet_addr(argv[2]);
		cout << argv[2] << "已开放端口:" << endl;
		for (int i = 1; i < 1024; i++)
		{

			while (ThreadCount >= MaxThread)
				Sleep(10);

			DWORD ThreadID;
			CreateThread(NULL, 0, ScanPort, (LPVOID) new short(i), 0, &ThreadID);
		}
		while (ThreadCount > 0)
			Sleep(50);
		WSACleanup();
	}

	return 0;
}
