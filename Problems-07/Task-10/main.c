#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main (int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "Program expects 2 arguments! Usage: %s [COMMAND1] [COMMAND2]", argv[0]);
	}
	
	pid_t first = fork();
	if (first < 0) {
		err(2, "Error forking process");
	}
	else if (first == 0) {
		if(execlp(argv[1], argv[1], (char*)NULL) < 0) {
			err(3, "Error during exec command: %s", argv[1]);
		}
	}

	int status;
	wait(&status);
	
	if(WEXITSTATUS(status) == 0) {
		printf("Command %s executed successfully.\n", argv[1]);

		pid_t second = fork();
		if (second < 0) {
			err(2, "Error forking process");
		}
		else if (second == 0) {
			if(execlp(argv[2], argv[2], (char*)NULL) < 0) {
				err(3, "Error during exec command: %s", argv[2]);
			}
		}

		wait(&status);
		if(WEXITSTATUS(status) == 0) {
			printf("Command %s executed successfully.\n", argv[2]);
		}
		else {
			printf("Command %s failed.\n", argv[2]);
		}
	}
	else {
		errx(-1, "Command %s failed. Exiting.", argv[1]);
	}

	exit(0);
}
