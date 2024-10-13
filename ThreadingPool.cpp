//------------------------------------------------------
//   ThreadingPool.cpp
//		12/07/2023.     c17,c++20
//------------------------------------------------------

//------------------------------------------------------
// header.h
//------------------------------------------------------
#include"ThreadingPool.h"
	
//------------------------------------------------------
threadpool::ThreadPool::ThreadPool(long long thread_count)
	:_semaphore{ 0 }, _max_threads{ thread_count },
	_current_threads{ thread_count }
{
	for (int i = 0; i < _max_threads; i++)
	{
		std::jthread t([this](void)->void 
		{
			while (true)
			{
				_semaphore.acquire(); //will block.
				++_alive_threads;
				if (_is_exit)
				{
					--_current_threads;
					--_alive_threads;
					break;
				} 

				while (_task_queue.size() > 0)
				{
					std::unique_lock lck{ _access_mx };
					if (_task_queue.size() == 0)
						break;
					auto task = _task_queue.front();
					_task_queue.pop_front();
					lck.unlock();
					task->run();

					if (_is_exit)
					{
						--_current_threads;
						--_alive_threads;
						break;
					}
				}
				--_alive_threads;
			}
		});
		t.detach();
	}
	_current_threads = thread_count;//? can't understand
}
//------------------------------------------------------

threadpool::ThreadPool::~ThreadPool()
{
}
//------------------------------------------------------
void 
threadpool::ThreadPool::shutdown(void)
{
	while (_current_threads > 0)
	{
		_is_exit = true;
		_semaphore.release(_current_threads - _alive_threads);
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(200ms);
	}

	return;
}
//------------------------------------------------------
void 
threadpool::ThreadPool::add_task(std::shared_ptr<ITask> task)
{
	if (!task)
		return;
	std::unique_lock lck(_access_mx);
	_task_queue.push_back(task);
	lck.unlock();
	if (_alive_threads < _current_threads)
		_semaphore.release(1);
}
//------------------------------------------------------
void 
threadpool::ThreadPool::add_task(std::list<std::shared_ptr<ITask>> tasks)
{
	if (tasks.size() == 0u)
		return;
	std::unique_lock lck(_access_mx);
	_task_queue.insert(_task_queue.end(), tasks.begin(), tasks.end());
	lck.unlock();

	if (auto diff = _current_threads - _alive_threads)
		// if(diff > 0ll )
		_semaphore.release(diff >= _task_queue.size() ? _task_queue.size() : diff);
}
//------------------------------------------------------


