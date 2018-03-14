#ifdef __linux__
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#elif _WIN32 
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include<time.h>

using namespace std;

void ShowTime();

int main()
{
#ifdef  _WIN32
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
#endif //  _WIN32 | _WIN64
	cout << "Input count of your money";
	char strCurMoney[256];
	cin>> strCurMoney;

	fstream file;
	file.open("Availability.txt", ios::out | ios::in);

	char str[256];
	while (!file.eof())
	{
		file >> str;
		cout << str << " ";
		file >> str;
		cout << str << " ";
		file >> str;
		cout << str << endl;
	}

	file.close();
	cout << "Input coffee name" << endl;
	cin >> str;

#ifdef _WIN32 
	char strWin[256];
	sprintf_s(strWin, "child.exe  %s %s", strCurMoney,str);
	if (!CreateProcess(NULL,
		strWin,
		NULL,								 // Process handle not inheritable.
		NULL,								 // Thread handle not inheritable.
		FALSE,								 // Set handle inheritance to FALSE.
		CREATE_NEW_CONSOLE,
		NULL,								 // Use parent's environment block.
		NULL,								 // Use parent's starting directory.
		&si,								 // Pointer to STARTUPINFO structure.
		&pi)
		)
	{
		cout << "CreateProcess failed" << endl << GetLastError();
		return -1;
	}

	while (WaitForSingleObject(pi.hProcess, 50))
	{
		ShowTime();
	}

	DWORD exitCode;
	GetExitCodeProcess(pi.hProcess, &exitCode);
	if (exitCode == 0) cout << "\r\n\nOperation was successfully completed!\r\n\n";
	if (exitCode == 1) cout << "\r\n\nNot found coffee\r\n\n";
	if (exitCode == 2) cout << "\r\n\nYou don't have nedded money\r\n\n";
	if (exitCode == 3) cout << "\r\n\nNot open file\r\n\n";
	system("pause");

	CloseHandle(pi.hThread);
#elif __linux__
	pid_t pid = fork();

	int st;

	if (pid == -1)
		cout << "Error\n";
	else if (pid == 0)
	{
		if (execlp("./child","", strCurMoney,str, NULL) == -1)
			cout << "Error\n";
	}
	else
	{
		while (1)
		{
			ShowTime();
			if (waitpid(pid, &st, WNOHANG) > 0) break;


			napms(1000);
		}
			if (WIFEXITED(st))
			{
				if ((int)WEXITSTATUS(st) == 0) cout << "\r\n\nOperation was successfully completed!\r\n\n";
				if ((int)WEXITSTATUS(st) == 1) cout << "\r\n\nNot found coffee\r\n\n";
				if ((int)WEXITSTATUS(st) == 2) cout << "\r\n\nYou don't have nedded money\r\n\n";
				if ((int)WEXITSTATUS(st) == 3) cout << "\r\n\nNot open file\r\n\n";
			}
		
	}

	cin.get();
#endif

	return 0;
}



void ShowTime()
{
	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	cout << "Current local time and date: "<<asctime(timeinfo);
}
