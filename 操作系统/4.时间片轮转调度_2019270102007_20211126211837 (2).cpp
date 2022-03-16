#include "thread_hdr.h"
unsigned int time_ticks = 0;
unsigned int time_interval = 0;

void add_ready_thread(thread* ready_thread)
{
	ready_thread->clock_times = 0;
	ready_queue.push_back(ready_thread);

}

void schedule()
{

	if (current_thread != &idle_thread) 
	{
		current_thread->clock_times = 0;
		if (ready_queue.size() != 0) 
		{
			ready_queue.push_back(current_thread);
			current_thread = ready_queue.front();
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
	current_thread = &idle_thread;
	if (ready_queue.size() != 0)schedule();
}

void current_thread_blocked()
{
	current_thread->clock_times = 0;
	blocked_queue.push_back(current_thread);
	current_thread = &idle_thread;
	schedule();
}

void notify()
{
	if (blocked_queue.size() != 0) 
	{
		ready_queue.push_back(blocked_queue.front());
		blocked_queue.pop_front();
	}
}

void notify_all()
{
	while (blocked_queue.size() != 0)notify();
}
void on_clock()
{
	if (current_thread != &idle_thread) 
	{
		current_thread->clock_times += time_interval;
		if (current_thread->clock_times >= time_ticks) 
		{
			schedule();
		}
	}
	else schedule();
		
}

void set_time_ticks(unsigned int ticks)
{
	time_ticks = ticks;
}

void set_time_interval(unsigned int interval)
{
	time_interval = interval;
}