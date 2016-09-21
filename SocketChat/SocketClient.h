#pragma once

#include "SocketBase.h"
#include "GlobalConst.h"

namespace SocketChat
{

	class SocketClient : public SocketBase
	{
	private:
		// name of current client 
		std::string strName;
		// struct of info of current client 
		sockaddr_in ConnectSAddr;
		// thread for using function of receiving messages 
		friend DWORD WINAPI InThreadWithServer(LPVOID)throw();
	public:
		SocketClient();
		~SocketClient();

		// function for current client name setting 
		void SetName(std::string&)throw();

		// function for current client name getting 
		const std::string& GetName(void)const throw();

		// Initialization of struct with info of current client 
		bool ServerInit(std::string &);

		// Connection 
		int ConnectToServer(void);

		// Creation of thread for using function of receiving messages 
		void CreateThreadInServerLoop(void);
	};

	// thread for using function of receiving messages 
	DWORD WINAPI InThreadWithServer(LPVOID)throw();
}