#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ncurses.h>
#include <curses.h>
#include <time.h>


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <signal.h>
#include <csignal>

struct sigaction printID, closeProcess;
int flagForPrint = 0;
int flagForClose = 1;
#define MAX_COUNT_OF_PROCESS 5

/* Функции обработчики сигналов */
void handlerForPrint(int sign) {
	//printw("Alright\n");
	//refresh();
	flagForPrint = 1;
}

void handlerForClose(int sign) {
	//printw("deleteHandler\n");
	//refresh();
	flagForClose = 1;
}


int main(int argc, char ** argv) {
	setlocale(LC_ALL, "Russian");
	int checker, choice, money, i;
	FILE* fileForInf;
	char strForCreate[260];
	char tempForChoice[2] = { 0 };
	char tempForMoney[238] = { 0 };

	initscr();

	char uniqueStr[256];
	printID.sa_handler = handlerForPrint;
	sigaction(SIGUSR1, &printID, NULL);

	closeProcess.sa_handler = handlerForClose;
	sigaction(SIGUSR2, &closeProcess, NULL);

	sprintf(uniqueStr, "%s", argv[0]);
	pid_t pid = fork();

	switch (pid)
	{
	case -1: printw("Process didn't create");
		return 1;
	case 0:
	{
		char strCurMoney[5] = { '1','2','3','4','\0' };
		printw("%s %s\n", "Count of your money", strCurMoney);

		char str[2];

		str[0] = 'R';
		str[1] = '\0';
		printw("%s %s\n", "Coffee name", str);



		refresh();
		if (execl("./coffemachine", strCurMoney, str, NULL) == -1)
			printf("Didn't execute child process");
	}
	break;

	default:
	{
		flagForClose = 0;

		refresh();
		while (!flagForClose)
		{
			usleep(100000);
			if (flagForPrint)
			{
				for (i = 0; i < strlen(uniqueStr); i++)
				{
					if (flagForClose)
					{
						flagForPrint = 0;
						kill(getppid(), SIGUSR2);
						return 0;
					}
					printw("%c", uniqueStr[i]);
					refresh();
					usleep(100000);
				}
				printw("\n");
				refresh();
				flagForPrint = 0;
				kill(getppid(), SIGUSR2);
			}
		}
		return 0;
	}
	}
	endwin();

	return 0;
}
