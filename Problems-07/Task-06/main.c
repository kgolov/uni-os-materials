#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "One parameter expected! Usage: %s [COMMAND]", argv[0]);
	}

	pid_t pid = fork();
	if (pid < 0) {
		err(2, "Error executing fork");
	}
	else if (pid == 0) {
		// Child process
		if (execlp(argv[1], argv[1], (char*)NULL) < 0) {
			err(3, "Error during exec");
		}
	}
	else {
		// Parent process
		int status;
		wait(&status);
		
		if(WEXITSTATUS(status) == 0) {
			printf("Command \"%s\" executed successfully.\n", argv[1]);
		}
		else {
			printf("Command \"%s\" did not executre successfully, exit code: %d\n", argv[1], WEXITSTATUS(status));
		}
	}

	exit(0);
}
