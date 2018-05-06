#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ncurses.h>
#include <curses.h>
#include <time.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

/* Структура, которую использует функция sigaction() */
struct sigaction printID, closeProcess;
int flagForPrint = 0;
int flagForClose = 1;
#define MAX_COUNT_PROCESS 5 

const char * NAMES[MAX_COUNT_PROCESS] = {
	"FIRST_PROCESS",
	"SECOND_PROCESS",
	"THIRD_PROCESS",
	"FOURTH_PROCESS",
	"FIFTH_PROCESS"
};

/* Функции обработчики сигналов */
void handlerForPrint(int sign) {
	refresh();
	flagForPrint = 1;
}

void handlerForClose(int sign) {
	refresh();
	flagForClose = 1;
}

int main() {
	char key;
	int fl = 1, number = -1;

	pid_t arrayOfPid[MAX_COUNT_PROCESS];
	char cmdArgs[256];
	int exitCode, currentNumber = 0;
	
	printID.sa_handler = handlerForPrint;
	sigaction(SIGUSR1, &printID, NULL);

	closeProcess.sa_handler = handlerForClose;
	sigaction(SIGUSR2, &closeProcess, NULL);


	fl = 1;
	number = 0;

	initscr();
	curs_set(0);
	clear();
	noecho();
	refresh();

	while (fl) {
		halfdelay(1);
		key = (char)getchar();
		if (key == '+') {
			if (number < MAX_COUNT_PROCESS) {
				arrayOfPid[number] = fork();
				number++;
				/* Если процесс создан успешно */
				if ( arrayOfPid[number - 1] == 0) {
					sprintf(cmdArgs, "%s", NAMES[number-1]);
					if (execlp ("./child", cmdArgs, NULL) == -1) {
						printw("Didn't execute child process\n");
						refresh();
					}
				}
			}
		}

		if (key == '-') {
			if (number <= 0) { fl = 0; break; }
			kill(arrayOfPid[number - 1], SIGUSR2);
			kill(getpid(), SIGABRT);
			waitpid(arrayOfPid[number - 1], &exitCode, 0);
			printw("Process number %d closed\n", number - 1);
			refresh();
			number--;
			
			if(currentNumber >= number) {
				currentNumber = 0;
				flagForClose = 1;
			}

		}

		if (key == 'q') {
			if (number <= 0) { fl =0; break;}

			while(number > 0) {
				kill(arrayOfPid[number - 1], SIGUSR2);
				waitpid(arrayOfPid[number - 1], &exitCode, 0);
				printw("Process number %d closed\n", number - 1);
				kill(arrayOfPid[number - 1], SIGTERM);
				refresh();
				number--;
			}
		}
	
		if (flagForClose) {
			sleep(1);
			flagForClose = 0;
			if (currentNumber >= number - 1) currentNumber = 0;
			else currentNumber++;

			kill(arrayOfPid[currentNumber], SIGUSR1);
		}
	}

	clear();
	endwin();

	return 0;
}