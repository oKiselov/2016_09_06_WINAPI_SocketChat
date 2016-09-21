#include "SocketClient.h"
using namespace SocketChat;

SocketClient::SocketClient(){}


SocketClient::~SocketClient(){}

// function for current client name setting 
void SocketClient::SetName(std::string&strName)
{
	this->strName = strName; 
}

// function for current client name getting 
const std::string& SocketClient::GetName() const
{
	return this->strName; 
}

// Initialization of struct with info of current client 
bool SocketClient::ServerInit(std::string&ServerIP)
{
	bool bRet = TRUE; 
	ServerIP = "127.0.0.1"; 
	this->ConnectSAddr.sin_addr.s_addr = INADDR_NONE;
	if (INADDR_NONE == this->ConnectSAddr.sin_addr.s_addr)
	{
		struct hostent *hGetByName = ::gethostbyname(ServerIP.c_str());

		if (!hGetByName)
		{
			throw std::exception("Error gethostbyname: " ); 
		}
		this->ConnectSAddr.sin_addr = * reinterpret_cast<in_addr*> (hGetByName->h_addr_list[0]);
	}
	this->ConnectSAddr.sin_family = AF_INET;
	this->ConnectSAddr.sin_port = ::htons(PORT_NUM);
	
	std::cout << "Initialized" << std::endl; 

	return bRet; 
}

// Connection 
int SocketClient::ConnectToServer()
{
	int iConnect = connect(this->currSocket, reinterpret_cast<sockaddr*>(&this->ConnectSAddr), sizeof(this->ConnectSAddr)); 
	if (iConnect==SOCKET_ERROR)
	{
		throw std::exception("Error connection: "); 
	}
	std::cout << "Connected" << std::endl; 
	return iConnect; 
}

// Creation of thread for using function of receiving messages 
void SocketClient::CreateThreadInServerLoop()
{
	HANDLE hThread = CreateThread(NULL, NULL, InThreadWithServer, (LPVOID)this->currSocket, NULL,NULL);
	if (!hThread)
	{
		throw std::exception("Error creation of thread: "); 
	}
	int SizeOfSendedStr; 
	
	// Creation of first message with name 
	std::string strTemp("\r\n"); 
	strTemp += this->strName;
	strTemp += ": "; 
	strTemp += this->strBuff; 
	
	do
	{
		/*****************/
		/*SEND*/
		/*****************/
		SizeOfSendedStr = send(this->currSocket, &strTemp[0], strTemp.size(), 0);
		if (SizeOfSendedStr != SOCKET_ERROR && SizeOfSendedStr == strTemp.size())
		{
			// Creation of next messages with name 
			this->strBuff.resize(0); 
			std::getline(std::cin, this->strBuff);
			strTemp="\r\n";
			strTemp += this->strName;
			strTemp += ": ";
			strTemp += this->strBuff;
		}
	} while ((SizeOfSendedStr != SOCKET_ERROR) || (this->strBuff == "exit"));
}

DWORD WINAPI SocketChat::InThreadWithServer(LPVOID currSocket)
{
	SOCKET SocketInThread = reinterpret_cast<SOCKET>(currSocket);

	int SizeOfReceivedStr; 
	do
	{
		/*****************/
		/*RECEIVE*/
		/*****************/

		std::string strBuff(1024, '\0');
		SizeOfReceivedStr = recv(SocketInThread, &strBuff[0], strBuff.size(), 0);	
		if (SizeOfReceivedStr && SizeOfReceivedStr != SOCKET_ERROR)
		{
			strBuff.erase(strBuff.find('\0'), strBuff.size() - 1);
			std::cout << strBuff << std::endl;
		}
	} while (SizeOfReceivedStr && SizeOfReceivedStr != SOCKET_ERROR);

	return 0;
}

