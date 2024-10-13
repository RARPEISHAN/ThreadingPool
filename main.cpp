#include "ThreadingPool.h"
#include "NetworkTask.h"
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32")

bool test_network_task(threadpool::ThreadPool& _threads_Pool);

int main(void)
{
	threadpool::ThreadPool threadspool{ 10 };
	test_network_task(threadspool);
	threadspool.shutdown();

	return 0;
}

bool 
test_network_task(threadpool::ThreadPool& _threads_Pool)
{
	/*Initialize winsock2 library.*/
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData))
		return 0;

	/*create a socket for listening.*/
	SOCKET listening_sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listening_sock == INVALID_SOCKET)
		return 0;

	/*bind to a local ip address and port.*/
	SOCKADDR_IN local_addr;
	int net_buf;
	if (InetPtonW(AF_INET, L"192.168.43.94", &net_buf) != 1)
		return -1;
	local_addr.sin_port = htons(28002);
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.S_un.S_addr = net_buf;

	if (::bind(
		listening_sock,
		reinterpret_cast<const sockaddr*>(&local_addr),
		sizeof(local_addr)) == SOCKET_ERROR)
		return -1;

	if (::listen(listening_sock, SOMAXCONN))
		return -1;

	while (true) {
		// set a client info and accept a connection.
		SOCKADDR_IN client_addr;
		int addr_len = sizeof client_addr;
		SOCKET work_sock = ::accept(
			listening_sock,
			reinterpret_cast<sockaddr*>(&client_addr),
			&addr_len);
		if (work_sock == INVALID_SOCKET)
			continue;

		auto net_task = std::make_shared<NetworkTask>(client_addr, work_sock);

		_threads_Pool.add_task(net_task);
		
	}

	::closesocket(listening_sock);
	::WSACleanup();
}