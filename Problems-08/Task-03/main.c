#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

int main (int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "Program expects 1 parameter! Usage: %s [STRING]", argv[0]);
	}

	int pfd[2];
	if (pipe(pfd) < 0) {
		err(2, "Error creating pipe");
	}

	pid_t pid = fork();
	if (pid < 0) {
		int old_errno = errno;
		close(pfd[0]);
		close(pfd[1]);
		errno = old_errno;
		err(3, "Error forking process");
	}
	else if (pid == 0) {
		close(pfd[1]);
		dup2(pfd[0], 0);
		if (execlp("wc", "wc", "-c", (char*)NULL) < 0) {
			int old_errno = errno;
			close(pfd[0]);
			errno = old_errno;
			err(4, "Error executing wc -c in child process");
		}
	}
	else {
		close(pfd[0]);

		if (write(pfd[1], argv[1], strlen(argv[1])) <= 0) {
			int old_errno = errno;
			close(pfd[1]);
			errno = old_errno;
			err(5, "Error writing in pipe");
		}

		close(pfd[1]);

		wait(NULL);
		exit(0);
	}
}
