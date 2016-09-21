#pragma once
#include "Settings.h"

namespace SocketChat
{

	class SocketBase
	{
	protected:
		// Structure for initializing information of main starting socket 
		WSAData WSStartData;
		// Pointer to main socket 
		SOCKET currSocket;
	public:
		// buffer for temporary messages 
		static std::string strBuff;

		SocketBase();
		virtual ~SocketBase();

		// Initialization of main socket's information structure  
		int SocketInit(void);

		// Creation of socket 
		int SocketStart(int);

		// Closing of socket 
		void SocketClose(int)const throw();
	};
}