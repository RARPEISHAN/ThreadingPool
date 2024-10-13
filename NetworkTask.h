//------------------------------------------------------
//   NetworkTask.h
//		12/07/2023.     c17,c++20
//------------------------------------------------------
#pragma once

#ifndef _NETWORK_TASK_H__
#define _NETWORK_TASK_H__
//------------------------------------------------------
// header.h
//------------------------------------------------------
#include "ITask.h"
#include <WinSock2.h>

class NetworkTask : public threadpool::ITask 
{
public:
	NetworkTask(SOCKADDR_IN& client_addr, SOCKET& work_socket);
	virtual bool run(void) override;
private:
	SOCKADDR_IN			_client_addr;
	SOCKET				_work_socket;
};
//------------------------------------------------------
#endif // !_NETWORK_TASK_H__
