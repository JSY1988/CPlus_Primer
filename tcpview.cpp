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
		cout << "tcpview [��ѡ] [��ѡ]<�����ǰ�����ĳ���>\n";
		cout << "tcpview -l <��ʾ����>\n";
		cout << "tcpview -c ����ID <�رս���>\n";
		cout << "tcpview -p IP <���IP���Ŷ˿�>\n"
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
		TerminateProcessFromId(dwProcessId); //�رս���
		system("TrayRefresh.exe");
	}
	else if (strcmp(argv[1], "-p") == 0)
	{
		WSADATA WSAData;
		if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
		{
			cout << "��ʼ��ʧ��" << GetLastError() << endl;
			ExitProcess(STATUS_FALIED);
		}
		serverIP = inet_addr(argv[2]);
		cout << argv[2] << "�ѿ��Ŷ˿�:" << endl;
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
