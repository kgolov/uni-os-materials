#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "Program expects 2 commands! Usage: %s [COMMAND1] [COMMAND2]", argv[0]);
	}
	
	pid_t first = fork();
	if (first < 0) {
		err(2, "Error forking process");
	}
	else if (first == 0) {
		if(execlp(argv[1], argv[1], (char*)NULL) < 0) {
			err(3, "Error exec command: %s", argv[1]);
		}
	}

	pid_t second = fork();
	if (second < 0) {
		err(2, "Error forking process");
	}
	else if (second == 0) {
		if(execlp(argv[2], argv[2], (char*)NULL) < 0) {
			err(3, "Error exec command: %s", argv[2]);
		}
	}

	int status;
	pid_t finished = wait(&status);

	if (WEXITSTATUS(status) == 0) {
		printf("Command which finished first was with PID %d.\n", finished);

		wait(NULL);
		exit(0);
	}

	finished = wait(&status);
	if (WEXITSTATUS(status) == 0) {
		printf("First command failed. Command which finished successfully was with PID %d.\n", finished);
	}
	else {
		errx(-1, "No command finished successfully.");
	}

	exit(0);
}
