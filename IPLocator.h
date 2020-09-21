#ifndef _IP_LOCATOR_H_
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <windows.h>
#include <direct.h>
#include <tlhelp32.h>
#include <time.h>
#include "tchar.h"
#include "psapi.h"
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
#pragma warning(disable : 4996)

using namespace std;
using std::streamsize;
using std::string;
class IPLocator
{
public:
	IPLocator(const string &ipdb_name);
	~IPLocator();
	string getVersion();
	string getIpAddr(const string &ip);
	string getIpRange(const string &ip);

private:
	string getIpAddr(unsigned int ip);
	string getIpRange(unsigned int ip);
	static unsigned int getIpFromString(const string &ip);
	static string getIpString(unsigned int ip);
	static unsigned int bytes2integer(unsigned char *ip, int count);
	void readFromFile(streamsize offset, unsigned char *buf, int len);
	string readStringFromFile(streamsize offset);
	string getAddr(streamsize offset);
	string getAreaAddr(streamsize offset);
	void setIpRange(unsigned int rec_no);

private:
	std::ifstream ipdb;
	unsigned int first_index = 0;
	unsigned int last_index = 0;
	unsigned int index_count = 0;
	unsigned int cur_start_ip = 0;
	unsigned int cur_start_ip_offset = 0;
	unsigned int cur_end_ip = 0;
};
#endif
#define _IP_LOCATOR_H_