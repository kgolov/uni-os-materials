#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[]) {
	if(argc != 2) {
		errx(1, "Program expects 1 parameter! Usage: %s [STRING]", argv[0]);
	}

	int pipefd[2];
	if (pipe(pipefd) < 0) {
		err(2, "Error opening pipe");
	}

	pid_t pid = fork();
	if (pid < 0) {
		int old_errno = errno;
		close(pipefd[0]);
		close(pipefd[1]);
		errno = old_errno;
		err(3, "Error forking process");
	}
	else if (pid == 0) {
		close(pipefd[1]);
		
		char buf;
		while (read(pipefd[0], &buf, 1) > 0) {
			if(write(1, &buf, 1) <= 0) {
				int old_errno = errno;
				close(pipefd[0]);
				errno = old_errno;
				err(5, "Error writing to stdout");
			}
		}

		close(pipefd[0]);
		exit(0);
	}
	else {
		close(pipefd[0]);
		
		if(write(pipefd[1], argv[1], strlen(argv[1])) < 0) {
			int old_errno = errno;
			close(pipefd[1]);
			errno = old_errno;
			err(4, "Error writing in pipe");
		}

		close(pipefd[1]);

		// Wait for child to finish, then exit
		wait(NULL);
		exit(0);
	}
}
