#pragma once
#include "SocketClient.h"
#include "SocketServer.h"

namespace SocketChat
{
	enum Option { server = 1, client = 2 };

	// Class for entering the programm of Socket 
	class Menu
	{
	public:
		Menu();
		~Menu();

		// To show the name of PC
		static void GetNameOfPC(void);

		// To enter the menu 
		void StartMenu(void)const;

	};
}
