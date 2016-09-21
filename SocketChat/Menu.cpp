#include "Menu.h"
using namespace SocketChat; 

Menu::Menu(){}


Menu::~Menu(){}

void Menu::GetNameOfPC()
{
	const int MAX_PASS = 20;
	TCHAR strNamePC[MAX_PASS];
	DWORD iSizePC = MAX_PASS;

	GetComputerName(strNamePC, &iSizePC);
	strNamePC[iSizePC] = '\0';
	std::wcout << strNamePC << std::endl;
}

void Menu::StartMenu() const
{
	std::cout << "Press 1 - to server. 2 - to client!" << std::endl;
	int option;
	std::cin >> option;

	/*SERVER*/
	if (option == server)
	{
		SocketServer sockServer;
		sockServer.SocketInit();
		sockServer.SocketStart(server);
		sockServer.SocketStructInit();
		sockServer.SocketBind();
		sockServer.SocketListen();
		sockServer.SocketAccept();
	}
	/*CLIENT*/
	else if (option == client)
	{
		SocketClient sockClient;
		sockClient.SocketInit();
		sockClient.SocketStart(client);
		std::cout << "Enter Servers IP: " << std::endl;
		std::string strSerIP;
		while (strSerIP.size() == 0)
		{
			std::getline(std::cin, strSerIP);
		}

		std::cout << "Enter Your Nickname: " << std::endl;
		std::string strName;
		while (strName.size() == 0)
		{
			std::getline(std::cin, strName);
		}
		sockClient.SetName(strName);
		sockClient.ServerInit(strSerIP);
		sockClient.ConnectToServer();
		sockClient.CreateThreadInServerLoop();
		sockClient.SocketClose(2);
	}
}
