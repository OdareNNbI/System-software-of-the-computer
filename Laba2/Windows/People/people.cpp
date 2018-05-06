
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

#include <iostream>
#include <fstream>
#include<time.h>

using namespace std;

int main(int argc, char** argv)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hOpen = OpenEvent(EVENT_ALL_ACCESS, FALSE, argv[1]);
	HANDLE hClose = OpenEvent(EVENT_ALL_ACCESS, FALSE, argv[2]);

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	cout << "Input count of your money";
	char strCurMoney[256];
	cin >> strCurMoney;

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

	char strWin[256];
	sprintf_s(strWin, "CoffeMachine.exe  %s %s", strCurMoney, str);
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

	}
	DWORD exitCode;
	GetExitCodeProcess(pi.hProcess, &exitCode);
	if (exitCode == 0) cout << "\r\n\nOperation was successfully completed!\r\n\n";
	if (exitCode == 1) cout << "\r\n\nNot found coffee\r\n\n";
	if (exitCode == 2) cout << "\r\n\nYou don't have nedded money\r\n\n";
	if (exitCode == 3) cout << "\r\n\nNot open file\r\n\n";

	while (true)
	{

		if (WaitForSingleObject(hOpen, 0) == WAIT_OBJECT_0)
		{
			int i = 0;
			while (argv[1][i] != '\0')
			{
				if (WaitForSingleObject(hClose, 0) == WAIT_OBJECT_0)
				{
					ResetEvent(hOpen);

					CloseHandle(hClose);
					CloseHandle(hOpen);
					CloseHandle(pi.hProcess);
					return 0;
				}
				cout << argv[1][i];
				i++;
				Sleep(50);
			}
			cout << endl;
			ResetEvent(hOpen);
		}

		if (WaitForSingleObject(hClose, 0) == WAIT_OBJECT_0)
		{
			CloseHandle(hClose);
			CloseHandle(hOpen);
			CloseHandle(pi.hProcess);
			return 0;
		}
	}
}
