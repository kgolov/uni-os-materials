#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

int N = 2; // How much processes will be executed one after the other

int main (int argc, char* argv[]) {
	if (argc != (N + 2)) {
		errx(1, "Program expects %d parameters! Usage: %s [COMMAND1] [COMMAND2] [FILE]", (N + 1),  argv[0]);
	}
	
	int fd = open(argv[argc - 1], O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		err(2, "Error opening file %s", argv[argc - 1]);
	}

	for (int i = 1; i < argc - 1; i++) {
		pid_t pid = fork();
		if (pid < 0) {
			int old_errno = errno;
			close(fd);
			errno = old_errno;
			err(3, "Error forking process");
		}
		else if (pid == 0) {
			close(fd);

			if (execlp(argv[i], argv[i], (char*)NULL) < 0) {
				int old_errno = errno;
				close(fd);
				errno = old_errno;
				err(4, "Error executing command: %s", argv[i]);
			}
		}

		int status;
		wait(&status);
		if (WEXITSTATUS(status) == 0) {
			if (write(fd, argv[i], strlen(argv[i])) <= 0) {
				int old_errno = errno;
				close(fd);
				errno = old_errno;
				err(5, "Error writing to file %s", argv[argc - 1]);
			}
			if (write(fd, "\n", 1) != 1) {
				int old_errno = errno;
				close(fd);
				errno = old_errno;
				err(5, "Error writing to file %s", argv[argc - 1]);
			}
		}
		else {
			printf("Command %s failed with exit code %d!\n", argv[i], WEXITSTATUS(status));
		}
	}

	close(fd);
	exit(0);
}
