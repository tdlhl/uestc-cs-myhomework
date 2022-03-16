#include "thread_hdr.h"

void add_ready_thread(thread* ready_thread)
{
	// 相应的代码实现
	ready_queue.push_back(ready_thread);
}

void schedule()
{
	if (current_thread != &idle_thread) 
	{
		if (ready_queue.size() != 0) 
		{
			ready_queue.push_back(current_thread);
			thread* temp;
			temp = ready_queue.front();
			current_thread=temp;
			ready_queue.pop_front();
		}
	}
	else 
	{
		if (ready_queue.size() != 0) 
		{
			current_thread = ready_queue.front();
			ready_queue.pop_front();
		}
	}
}

void current_thread_finished()
{
	// 实现的代码
	current_thread=&idle_thread;
	if (ready_queue.size() !=0)schedule();
}

void current_thread_blocked()
{
	// 相应的代码实现
	blocked_queue.push_back(current_thread);
	current_thread=&idle_thread;
	if (ready_queue.size() !=0)
	{
		schedule();
	}
}

void notify()
{
	// 相应的代码实现
	thread* temp;
	temp=blocked_queue.front();
	blocked_queue.pop_front();
	ready_queue.push_back(temp);
}

void notify_all()
{
	// 相应的代码实现
	thread* temp;
	while(blocked_queue.size()!=0)
	{
		temp=blocked_queue.front();
		blocked_queue.pop_front();
		ready_queue.push_back(temp);
	}
}