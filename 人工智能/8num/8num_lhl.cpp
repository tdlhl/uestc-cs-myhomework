#include<iomanip>
#include<time.h> 
#include<windows.h>
#include<stdlib.h>
#include<cmath>
#include<cstring>
#include<iostream>

using namespace std;

typedef struct node
{
	int h;
	int g;
	int f;
	char position;
	char state[10];
	char last_state[10];
}node;

char tar_hang[9];
char tar_lie[9];
char target[10] = { 0 };
const char ref_h[9] = { 0,0,0,1,1,1,2,2,2 };
const char ref_l[9] = { 0,1,2,0,1,2,0,1,2 };
const char can_move[9][4] = { {1,3,-1,-1},{0,2,4,-1},{1,5,-1,-1},{0,4,6,-1},{1,3,5,7},{2,4,8,-1},{3,7,-1,-1},{4,6,8,-1},{5,7,-1,-1} };

node all[1814400];
int n, step, num, hash_table[20000000];
int search(const char st[])
{
	int t1 = atoi(st) / 100;
	int t2 = t1 + 10000000;
	if (hash_table[t1] == 0)
		return 0;
	if (strcmp(all[hash_table[t1]].state, st) == 0)
		return (t1);
	else
		if (hash_table[t2] != 0)
			return (t2);
		else
			return 0;
}

template <typename T>
void Swap(T& a, T& b)
{
	T t;
	t = a;
	a = b;
	b = t;
}

void swap_heap(int x, int temp)
{
	Swap(hash_table[search(all[x].state)], hash_table[search(all[temp].state)]);
	Swap(all[x].h, all[temp].h);
	Swap(all[x].g, all[temp].g);
	Swap(all[x].f, all[temp].f);
	Swap(all[x].position, all[temp].position);
	char t[10] = { 0 };
	strcpy(t, all[x].state);
	strcpy(all[x].state, all[temp].state);
	strcpy(all[temp].state, t);
	strcpy(t, all[x].last_state);
	strcpy(all[x].last_state, all[temp].last_state);
	strcpy(all[temp].last_state, t);
}

int h1(const char st[])
{
	int result= 0;
	for (int i = 0; i < int(strlen(st)); i++)
	{
		if (st[i] != target[i])
		{
			result++;
		}
	}
	return result;
}

int h2(const char st[],float gamma)
{
	int result = 0;
	for (int i = 0; i < int(strlen(st)); i++)
	{
		result = result + abs(tar_hang[st[i] - 48] - ref_h[i]) + abs(tar_lie[st[i] - 48] - ref_l[i]);
	}
	result=result*gamma;
	return result;
}

int h3(const char st[],float lamda)
{
	int result = 0,temp=0;
	for (int i = 0; i < int(strlen(st)); i++)
	{
		temp = abs(tar_hang[st[i] - 48] - ref_h[i]) + abs(tar_lie[st[i] - 48] - ref_l[i]);
		temp=temp*lamda;
		result+=temp;
	}
	return result;
}

void insert(int x)
{
	if (x / 2 == 0)
		return;
	if (all[x].f < all[x / 2].f)
	{
		swap_heap(x, x / 2);
		insert(x / 2);
	}
}

void update(int x)
{
	if (x * 2 > n)
		return;
	int temp;
	if ((x * 2 + 1 > n) || (all[x * 2].f < all[x * 2 + 1].f))
		temp = x * 2;
	else
		temp = x * 2 + 1;
	swap_heap(x, temp);
	update(temp);
}

int is_solvable (const char st[])
{
	int result= 0;
	for (int i = 0; i < int(strlen(st)); i++)
	{
		for (int j = 0; j < i; j++)
		{
			if ((st[j] > st[i])&&(st[i] != '0'))
			{
				result++;
			}
		}
	}
		
	return (result % 2);
}

void check(const char st[])//检查初始状态能否到达目标状态，不能则输出"无解"
{
	if (is_solvable (all[1].state) != is_solvable (target))
	{
		cout << "can't be solved'" << endl;
		exit(0);
	}
}

void print_path(const node t)
{
	if (int(strlen(t.last_state)) != 0)
	{
		print_path(all[hash_table[search(t.last_state)]]);
	}
	else
		cout << "原图：" << "f(n)=" << t.g + t.h << endl;
	if (step != 0)
		cout << "step" << setw(2) << step <<" : " << "f(n)=" << t.g + t.h << endl;
	int i;
	for (i = 0; i < 9; i++)
	{
		cout << setw(4) << t.state[i];
		if ((i + 1) % 3 == 0)
			cout << endl;
	}
	step++;
}


int main()
{

	(void)freopen("data.txt", "r", stdin);
	(void)freopen("result.txt", "w", stdout);

	char st[10];
	int i,x;
	float gamma=1.30,lamda=2.1;
	node t;
	n = 1;
	
	double Time=0;    
	LARGE_INTEGER nFreq;  
	LARGE_INTEGER nBeginTime;  
	LARGE_INTEGER nEndTime;  
	QueryPerformanceFrequency(&nFreq);  
	QueryPerformanceCounter(&nBeginTime);//开始计时 

	cin >> all[1].state;
	cin >> target;
	for (i = 0; i<int(strlen(target)); i++)
	{
		if ((target[i] >= '0') && (target[i] <= '9'))
		{
			tar_hang[target[i] - '0'] = i / 3;
			tar_lie[target[i] - '0'] = i % 3;
		}
	}

	check(all[1].state);
	//all[1].h = h2(all[1].state,gamma);
	all[1].h = h3(all[1].state,lamda);
	all[1].g = 0;
	all[1].f = all[1].g + all[1].h;
	hash_table[atoi(all[1].state) / 100] = 1;
	for (i = 0; i < 10; i++)
	{
		all[1].last_state[i] = 0;
	}
	for (i = 0; i < int(strlen(all[1].state)); i++)
	{
		if (all[1].state[i] == '0')
		{
			all[1].position = i;
			break;
		}
	}
		
	while (1)
	{
		t = all[1];
		if (strcmp(t.state, target) == 0)
		{
			print_path(t);
			cout << "成功！ 共计";
			cout << t.f;
			cout << "步";
			cout << endl;
			break;
		}
		num++;
		all[1].f = 2147483647;
		update(1);
		for (i = 0; i < 4; i++)
		{
			if (can_move[t.position][i] == -1)
			{
				break;
			}
			strcpy(st, t.state);
			Swap(st[can_move[t.position][i]], st[t.position]);
			int t3 = search(st);
			if (t3 == 0)
			{
				
				n++;
				strcpy(all[n].state, st);
				strcpy(all[n].last_state, t.state);
				//all[n].h = h2(all[n].state,gamma);
				all[n].h = h3(all[n].state,lamda);
				all[n].g = t.g + 1;
				all[n].f = all[n].g + all[n].h;
				all[n].position = can_move[t.position][i];
				int t1 = atoi(st) / 100;
				int t2 = t1 + 10000000;
				if (hash_table[t1] != 0)
					hash_table[t2] = n;
				else
					hash_table[t1] = n;
				insert(n);
			}
			else
			{
				x = hash_table[t3];
				if ((t.g + 1) < all[x].g)
				{
					strcpy(all[x].last_state, t.state);
					all[x].g = t.g + 1;
					all[x].f = all[x].g + all[x].h;
					insert(x);
				}
			}
		}
	}
	
	QueryPerformanceCounter(&nEndTime);//停止计时  
	Time=(double)(nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart;
	cout<<"运行时间："<<Time*1000<<"ms"<<endl;
	cout<<"估价函数: "<<"h3 "<<"  lamda="<<lamda<<endl;
	return 0;
}
