#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "One file expected! Usage: %s [FILE]", argv[0]);
	}

	int pfd[2];
	if (pipe(pfd) < 0) {
		err(2, "Error creating pipe");
	}
		
	pid_t first = fork();
	if (first < 0) {
		int old_errno = errno;
		close(pfd[0]);
		close(pfd[1]);
		errno = old_errno;
		err(3, "Error forking process");
	}
	else if (first == 0) {
		close(pfd[0]);
		dup2(pfd[1], 1);

		if (execlp("cat", "cat", argv[1], (char*)NULL) < 0) {
			int old_errno = errno;
			close(pfd[1]);
			errno = old_errno;
			err(4, "Error executing cat command");
		}
	}

	pid_t second = fork();
	if (second < 0) {
		int old_errno = errno;
		close(pfd[0]);
		close(pfd[1]);
		errno = old_errno;
		err(3, "Error forking process");
	}
	else if (second == 0) {
		close(pfd[1]);
		dup2(pfd[0], 0);
		
		if (execlp("sort", "sort", (char*)NULL) < 0) {
			int old_errno = errno;
			close(pfd[0]);
			errno = old_errno;
			err(4, "Error executing sort command");
		}
	}
	
	wait(NULL);
	exit(0);
}
