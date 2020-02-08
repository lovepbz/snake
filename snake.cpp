#include<Windows.h>

#include<conio.h>
#include<thread>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define CH ' '
#define SNAKE 'S'
#define TARGET 'X'
#define UP 's'
#define DOWN 'w'
#define LEFT 'a'
#define RIGHT 'd'

typedef struct my_queue{
	int* arr;
	int head;
	int tail;
	int size;
}que;
que* queue_create(int size)
{
	que* res = (que*)malloc(sizeof(que));
	res->size = size;
	res->head = 0;
	res->tail = 0;
	res->arr = (int*)malloc(sizeof(int) * size);
	return res;
}
void queue_push(que* q, int x)
{
	q->arr[q->head] = x;
	q->head = (q->head + 1) % q->size;
}
int queue_peek(que* q)
{
	return q->arr[(q->head + q->size - 1) % q->size];
}
int queue_tail(que* q)
{
	return q->arr[q->tail];
}
int queue_pop(que* q)
{
	int res = q->arr[q->tail];
	q->tail = (q->tail + 1) % q->size;
	return res;
}
bool queue_full(que* q)
{
	return ((q->head + 1) % q->size == q->tail);
}

char to = 'w';
int count = 1;
bool lock = false;
bool stop = 0;
void input();
bool pro(char** arr, int m, int n, que* q1,que* q2);
void print(char** arr, int m, int n);
int main()
{
	int times = 100;
	int m = 15;
	int n = 20;
	int size = m * n;
	char** arr = (char**)malloc(sizeof(char*)*m);
	for (int i = 0; i < m; ++i)
	{
		arr[i] = (char*)malloc(sizeof(char) * n);
		for (int j = 0; j < n; ++j)
		{
			arr[i][j] = CH;
		}
	}
	arr[0][0] = TARGET;
	arr[0][1] = SNAKE;
	que* q1 = queue_create(size);
	que* q2 = queue_create(size);
	queue_push(q1, 0);
	queue_push(q2, 1);
	//CreateThread(NULL,0, (LPTHREAD_START_ROUTINE)input,NULL,0, NULL);
	std::thread t1(input);
	t1.detach();
	srand(time(NULL));
	double start = clock();
	double end;
	
	bool flag = true;
	for (;;)
	{
		/*
		for (;;)
		{
			end = clock();
			if (end - start >= time)
			{
				start = end;
				break;
			}
		}
		*/
		Sleep(times);//nice
		lock = true;
		if (to == ' ')continue;
		flag=pro(arr, m, n, q1, q2);
		if (count == size)break;
		if (!flag)break;
		system("cls");
		print(arr, m, n);
		//printf("\n");
	}
	printf("\n");
	//if (t1.joinable())t1.join();
	stop = 1;
	if (flag)
	{
		printf("you win\n");
	}
	else
	{
		printf("you lose\n");
	}
	system("pause");
	
	return 0;
}

void input()
{
	char temp;
	while (!stop)
	{
		temp = _getch();
		if (temp == UP&&to==DOWN||temp==DOWN&&to==UP||temp==LEFT&&to==RIGHT||temp==RIGHT&&to==LEFT)
		{
			continue;
		}
		else if(lock)
		{
			to = temp;
			lock = false;
		}
		
	}
}
bool pro(char** arr, int m, int n, que* q1, que* q2)
{
	
	char to1 = to;
	char temp = to1;
	if (temp == UP)
	{
		queue_push(q1, (queue_peek(q1) + 1) % m);
		queue_push(q2, queue_peek(q2));
	}
	else if (temp == DOWN)
	{
		queue_push(q1, (queue_peek(q1) - 1+m) % m);
		queue_push(q2, queue_peek(q2));
	}
	else if (temp == LEFT)
	{
		queue_push(q1, queue_peek(q1));
		queue_push(q2, (queue_peek(q2) - 1+n) % n);
	}
	else //if (temp == RIGHT)
	{
		queue_push(q1, queue_peek(q1));
		queue_push(q2, (queue_peek(q2) + 1) % n);
	}
	temp = arr[queue_peek(q1)][queue_peek(q2)];
	if (temp== SNAKE)
	{
		return false;
	}

	if (temp == TARGET)
	{
		arr[queue_peek(q1)][queue_peek(q2)] = SNAKE;
		int size = m * n;
		++count;
		for (;;)
		{
			int t = rand() % size;
			int x = t / n;
			int y = t % n;
			if (arr[x][y] != SNAKE)
			{
				arr[x][y] = TARGET;
				break;
			}
		}
		

	}
	else
	{
		arr[queue_peek(q1)][queue_peek(q2)] = SNAKE;
		arr[queue_pop(q1)][queue_pop(q2)] = CH;
	}
	return true;
}
void print(char** arr, int m, int n)
{
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			printf("%c", arr[i][j]);
		}
		printf("\n");
	}
}