#pragma once
#include "SocketBase.h"
#include "GlobalConst.h"

namespace SocketChat
{

	class SocketServer : public SocketBase
	{
	private:
		// pointer tot connected client socket 
		SOCKET clientSocket;
		// struct of info of current server  
		sockaddr_in ServerSAddr;

		//thread for work with accepted clients socket 
		friend DWORD WINAPI InThreadWithClients(LPVOID)throw();
	public:
		//array with coonnected clients sockets 
		static std::vector<SOCKET> vecClients;

		SocketServer();
		~SocketServer();

		// Initialization of struct with info of current server 
		void SocketStructInit(void)throw();

		// Binding 
		int SocketBind(void);

		// Starting of listening 
		int SocketListen(void) const;

		// Creation of thread for work with accepted clients socket 
		SOCKET&  SocketAccept(void);

	};
	//thread for work with accepted clients socket 
	DWORD WINAPI InThreadWithClients(LPVOID)throw();
}