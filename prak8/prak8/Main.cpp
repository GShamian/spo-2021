#include <windows.h>
#include <stdio.h>
#include <iostream> 

CRITICAL_SECTION cs;
HANDLE hMutex;
HANDLE hEvent1, hEvent2;

int a[5];
HANDLE hThr;
unsigned long uThrID;

using namespace std;

void ThreadCrit(void* pParams)
{
	int i, num = 0;
	while (1)
	{
		EnterCriticalSection(&cs);
		for (i = 0; i < 5; i++) a[i] = num;
		num++;
		LeaveCriticalSection(&cs);
	}
}

void ThreadMutex(void* pParams)
{
	int i, num = 0;
	while (1)
	{
		WaitForSingleObject(hMutex, INFINITE);
		for (i = 0; i < 5; i++) a[i] = num;
		num++;
		ReleaseMutex(hMutex);
	}
}

void ThreadEvent(void* pParams)
{
	int i, num = 0;
	while (1)
	{
		WaitForSingleObject(hEvent2, INFINITE);
		for (i = 0; i < 5; i++) a[i] = num;
		num++;
		SetEvent(hEvent1);
	}
}

int main(void)
{
	setlocale(LC_ALL, "Russian");

	cout << "Выберите вариант:\n1-Критические секции\n2-Мьютексы\n3-События\n";

	int v;
	cin >> v;

	switch (v)
	{
	case 1:
		InitializeCriticalSection(&cs);
		hThr = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadCrit, NULL, 0, &uThrID);

		while (1)
		{
			EnterCriticalSection(&cs);
			printf("%d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4]);
			LeaveCriticalSection(&cs);
		}

		break;
	
	case 2:
		hMutex = CreateMutex(NULL, FALSE, NULL);
		hThr = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadMutex, NULL, 0, &uThrID);

		while (1)
		{
			WaitForSingleObject(hMutex, INFINITE);
			printf("%d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4]);
			ReleaseMutex(hMutex);
		}

		break;
	
	case 3:
		hEvent1 = CreateEvent(NULL, FALSE, TRUE, NULL);
		hEvent2 = CreateEvent(NULL, FALSE, FALSE, NULL);
		hThr = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadEvent , NULL, 0, &uThrID);

		while (1)
		{
			WaitForSingleObject(hEvent1, INFINITE);
			printf("%d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4]);
			SetEvent(hEvent2);
		}

		break;
	
	default:
		cout << "Неправильный вариант";
		break;
	
	}
	
	return 0;
}