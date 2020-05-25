#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <err.h>
#include <sys/wait.h>
#include <stdio.h>

int main (int argc, char* argv[]) {
	if (argc < 2) {
		errx(1, "Program expects at least one command! Usage: %s [COMMAND1] ... ([COMMAND N])", argv[0]);
	}

	int successfulCount = 0;
	for (int i = 1; i < argc; i++) {
		pid_t pid = fork();
		if (pid < 0) {
			err(2, "Error forking process");
		}
		else if (pid == 0) {
			if (execlp(argv[i], argv[i], (char*)NULL) < 0) {
				err(3, "Error during exec command: %s", argv[i]);
			}
		}
		
		int status;
		wait(&status);
		if (WEXITSTATUS(status) == 0) {
			successfulCount++;
		}
	}

	printf("\nPrograms which finished successfully: %d\nPrograms which failed: %d\n", successfulCount, (argc - 1 - successfulCount));
	exit(0);
}
