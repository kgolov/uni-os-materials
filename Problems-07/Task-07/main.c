#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (int argc, char* argv[]) {
	if (argc != 4) {
		errx(1, "Three parameters expected! Usage: %s [COMMAND1] [COMMAND2] [COMMAND3]", argv[0]);
	}

	for (int i = 1; i <= 3; i++) {
		pid_t pid = fork();
		
		if (pid < 0) {
			err(2, "Error forking process");
		}
		else if (pid == 0) {
			// Child process, exec..
			if (execlp(argv[i], argv[i], (char*)NULL) < 0) {
				err(3, "Error during exec");
			}
		} 
		else {
			// Parent process, wait for child to finish
			int status;
			wait(&status);

			if (WIFEXITED(status)) {
				printf("Command \"%s\" finished. PID was %d, exit status is %d.\n", 
						argv[i], pid, WEXITSTATUS(status));
			}
		}
	}

	exit(0);
}
