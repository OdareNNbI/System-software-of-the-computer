#include<iostream>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>

#define MAX_COUNT_PROCESS 5

using namespace std;


const char * NAMES[MAX_COUNT_PROCESS] = {
	"FIRST_PROCESS",
	"SECOND_PROCESS",
	"THIRD_PROCESS",
	"FOURTH_PROCESS",
	"FIFTH_PROCESS"
};

const char * NAMES_CLOSE[MAX_COUNT_PROCESS] = {
	"FIRST_PROCESSC",
	"SECOND_PROCESSC",
	"THIRD_PROCESSC",
	"FOURTH_PROCESSC",
	"FIFTH_PROCESSC"
};


int main()
{
	char str1[255];
	sprintf_s(str1, "People.exe");
	char action;
	STARTUPINFO *si = NULL;
	PROCESS_INFORMATION *pi = NULL;
	HANDLE *hOpens = NULL;
	HANDLE *hCloses = NULL;

	int processQuantity = 0;
	int currentNum = 0;

	printf("CHOOSE THE ACTION: ");
	while (1)
	{


		if (_kbhit())
		{
			cin >> action;
			printf("CHOOSE THE ACTION: ");

			if (action == '+' && processQuantity < MAX_COUNT_PROCESS)
			{
				processQuantity++;

				si = (STARTUPINFO*)realloc(si, processQuantity * sizeof(STARTUPINFO));
				ZeroMemory(&si[processQuantity - 1], sizeof(si[processQuantity - 1]));
				si[processQuantity - 1].cb = sizeof(si[processQuantity - 1]);

				pi = (PROCESS_INFORMATION*)realloc(pi, processQuantity * sizeof(PROCESS_INFORMATION));
				ZeroMemory(&pi[processQuantity - 1], sizeof(pi[processQuantity - 1]));

				hOpens = (HANDLE*)realloc(hOpens, processQuantity * sizeof(HANDLE));
				hOpens[processQuantity - 1] = CreateEvent(NULL, TRUE, FALSE, NAMES[processQuantity - 1]);
				hCloses = (HANDLE*)realloc(hCloses, processQuantity * sizeof(HANDLE));
				hCloses[processQuantity - 1] = CreateEvent(NULL, TRUE, FALSE, NAMES_CLOSE[processQuantity - 1]);


				char strWin[256];
				sprintf_s(strWin, "People.exe %s %s", NAMES[processQuantity - 1], NAMES_CLOSE[processQuantity - 1]);
				if (!CreateProcess(NULL,
					strWin,
					NULL,								 // Process handle not inheritable.
					NULL,								 // Thread handle not inheritable.
					FALSE,								 // Set handle inheritance to FALSE.
					CREATE_NEW_CONSOLE,
					NULL,								 // Use parent's environment block.
					NULL,								 // Use parent's starting directory.
					&si[processQuantity - 1],								 // Pointer to STARTUPINFO structure.
					&pi[processQuantity - 1])
					)
				{
					cout << "CreateProcess failed" << endl << GetLastError();
					return -1;
				}
			}
			else if (action == '-' && processQuantity > 0)
			{
				SetEvent(hCloses[processQuantity - 1]);
				WaitForSingleObject(pi[processQuantity - 1].hProcess, INFINITY);
				CloseHandle(pi[processQuantity - 1].hProcess);
				CloseHandle(hOpens[processQuantity - 1]);
				CloseHandle(hCloses[processQuantity - 1]);
				if (currentNum == processQuantity - 1) currentNum = 0;
				processQuantity--;

				pi = (PROCESS_INFORMATION*)realloc(pi, processQuantity * sizeof(PROCESS_INFORMATION));
				hOpens = (HANDLE*)realloc(hOpens, processQuantity * sizeof(HANDLE));
				si = (STARTUPINFO*)realloc(si, processQuantity * sizeof(STARTUPINFO));
				hCloses = (HANDLE*)realloc(hCloses, processQuantity * sizeof(HANDLE));

				cout << "Process " << processQuantity << " was  closed" << endl;
			}
			else if (action == 'q')
			{
				for (int i = 0; i < processQuantity; i++)
				{
					SetEvent(hCloses[i]);
					WaitForSingleObject(pi[i].hProcess, INFINITY);
					CloseHandle(pi[i].hProcess);
					CloseHandle(hOpens[i]);
					CloseHandle(hCloses[i]);

					cout << "Process " << i << " was  closed" << endl;
				}

				free(pi);
				free(si);
				return 0;
			}
		}

		if (processQuantity > 0 && WaitForSingleObject(hOpens[currentNum], 0) == WAIT_TIMEOUT)
		{
			if (currentNum >= processQuantity - 1) currentNum = 0;
			else  currentNum++;
			SetEvent(hOpens[currentNum]);
		}
	}

	system("pause");
}