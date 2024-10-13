//------------------------------------------------------
//   ThreadingPool.h
//		12/07/2023.     c17,c++20
//------------------------------------------------------
#pragma once
#ifndef _THREADING_POOL_H__
#define _THREADING_POOL_H__

//------------------------------------------------------
// header.h
//------------------------------------------------------
#include "ITask.h"
#include<semaphore>
#include<thread>
#include<mutex>
#include<memory>
#include<list>

//------------------------------------------------------
namespace threadpool
{
	class ThreadPool
	{
	public:
		ThreadPool(long long thread_count = 10ll);
		~ThreadPool();

		void shutdown(void);

		void add_task(std::shared_ptr<ITask> task);
		void add_task(std::list<std::shared_ptr<ITask>> tasks);
	private:
		std::counting_semaphore<>			_semaphore;
		std::list<std::shared_ptr<ITask>>	_task_queue;
		std::mutex							_access_mx;

		long long							_max_threads{ 10ll };
		long long							_current_threads{ 0ll };
		long long							_alive_threads{ 0ll };
		bool								_is_exit{ false };
	};
}

//------------------------------------------------------
#endif // !_THREADING_POOL_H__
