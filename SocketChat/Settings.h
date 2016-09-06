#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <Winsock.h>
#pragma comment( lib, "Ws2_32.lib" ) 


struct CONNECTED_CLIENT
{
	std::string login; 
	SOCKET clientSock; 
};

const int MAX_CLIENTS = 10; 

const int PORT_NUM = 30001; 

static int clientCounter; 

CRITICAL_SECTION crit_section;