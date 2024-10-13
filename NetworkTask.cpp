//------------------------------------------------------
//   NetworkTask.cpp
//		12/07/2023.     c17,c++20
//------------------------------------------------------
#include "NetworkTask.h"
#include <ws2tcpip.h>
#include <iostream>
#include <string>

//------------------------------------------------------
NetworkTask::NetworkTask(
	SOCKADDR_IN& client_addr, 
	SOCKET& work_socket)
	: _client_addr{client_addr},
	_work_socket{work_socket}
{
}

//------------------------------------------------------
bool 
NetworkTask::run(void)
{
	if (_work_socket == INVALID_SOCKET)
		return false;
	// printed corresponding ip address and ports of client.  
	wchar_t buf_addr[46] = L"";
	::InetNtopW(AF_INET, (void*)&_client_addr.sin_addr, buf_addr, 46);
	wprintf(L"client address: %s, port:%hu\n", buf_addr, _client_addr.sin_port);

	std::wstring text{ L"Welcome to Server!" };
	::send(
		_work_socket,
		reinterpret_cast<const char*>(text.c_str()),
		text.length() * 2 + 2, 0);

	char bur[50];
	if (::recv(_work_socket, bur, 50, 0))
		std::wcout << reinterpret_cast<const wchar_t*>(bur) << std::endl;

	::closesocket(_work_socket);
	
	return true;
}
//------------------------------------------------------