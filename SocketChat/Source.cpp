#include "SocketClient.h"
#include "SocketServer.h"
#include "Menu.h"

std::vector<SOCKET> SocketChat::SocketServer::vecClients;
std::string SocketChat::SocketBase::strBuff = " has just entered this chat!";

int main()
{
	try{
		SocketChat::Menu menu; 
		menu.GetNameOfPC(); 
		menu.StartMenu(); 
		}
	catch (const std::exception &ex)
	{
		std::cout << ex.what() << WSAGetLastError() << std::endl; 
	}
	return 0;
}