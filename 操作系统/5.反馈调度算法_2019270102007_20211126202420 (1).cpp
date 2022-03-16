#include "thread_hdr.h"

unsigned int time_interval;
unsigned int first_time_ticks;
unsigned int second_time_ticks;

void add_ready_thread(thread* ready_thread)
{
	ready_thread->max_clock_times=first_time_ticks;
	first_ready_queue.push_back(ready_thread);
}

void schedule()
{
	if (current_thread != &idle_thread) 
	{
		if (first_ready_queue.size() != 0) 
		{
			current_thread->clock_times=0;
			current_thread->max_clock_times=second_time_ticks;
			second_ready_queue.push_back(current_thread);
			thread* temp;
			temp = first_ready_queue.front();
			current_thread=temp;
			first_ready_queue.pop_front();
			current_thread->clock_times=0;
		}
		else if(second_ready_queue.size() != 0)
		{
			current_thread->clock_times=0;
			current_thread->max_clock_times=second_time_ticks;
			second_ready_queue.push_back(current_thread);
			thread* temp;
			temp = second_ready_queue.front();
			current_thread=temp;
			second_ready_queue.pop_front();
			current_thread->clock_times=0;
		}
		else
		{
			current_thread->clock_times=0;
			current_thread->max_clock_times=second_time_ticks;
		}
	}
	else 
	{
		current_thread->clock_times=0;
		if (first_ready_queue.size() != 0) 
		{
			current_thread = first_ready_queue.front();
			first_ready_queue.pop_front();
			current_thread->clock_times=0;
		}
		else if(second_ready_queue.size() != 0)
		{
			thread* temp;
			temp = second_ready_queue.front();
			current_thread=temp;
			second_ready_queue.pop_front();
			current_thread->clock_times=0;
		}
		else
		{
			current_thread->clock_times=0;
			current_thread->max_clock_times=second_time_ticks;
		}
	}
}

void current_thread_finished()
{
    current_thread=&idle_thread;
	current_thread->clock_times=0;
	schedule();
}

void current_thread_blocked()
{
    current_thread->clock_times=0;
	blocked_queue.push_back(current_thread);
	current_thread=&idle_thread;
	current_thread->clock_times=0;
	schedule();
}

void notify()
{
    thread* temp;
	temp=blocked_queue.front();
	blocked_queue.pop_front();
	temp->clock_times=first_time_ticks;
	first_ready_queue.push_back(temp);
}

void notify_all()
{
    thread* temp;
	while(blocked_queue.size()!=0)
	{
		temp=blocked_queue.front();
		blocked_queue.pop_front();
		temp->clock_times=first_time_ticks;
		first_ready_queue.push_back(temp);
	}
}

void on_clock()
{
    (current_thread->clock_times)+=time_interval;
    
    if((current_thread->max_clock_times)==second_time_ticks&&first_ready_queue.size()!=0)
    {
    	current_thread->clock_times=0;
		schedule();
	}
    
	if((current_thread->clock_times)>=(current_thread->max_clock_times))
	{
		current_thread->clock_times=0;
		schedule();
	}
}

void set_first_time_ticks(unsigned int ticks)
{
    first_time_ticks=ticks;
}

void set_second_time_ticks(unsigned int ticks)
{
    second_time_ticks=ticks;
}

void set_time_interval(unsigned int interval)
{
    time_interval=interval;
}