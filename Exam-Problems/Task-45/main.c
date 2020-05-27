#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

// ALGORITHM
//        cut -d ':' -f 7 /etc/passwd | sort | uniq -c | sort -n

int main() {
	int pipeA[2];
	if (pipe(pipeA) < 0) {
		err(1, "Error creating pipe");
	}

	pid_t first = fork();
	if (first < 0) {
		int old_errno = errno; 
		close(pipeA[0]);	close(pipeA[1]);
		errno = old_errno;
		err(2, "Error forking process");
	}
	else if (first == 0) {
		close(pipeA[0]);

		if (dup2(pipeA[1], 1) < 0) {
			int old_errno = errno;
			close(pipeA[1]);
			errno = old_errno;
			err(4, "Error executing dup2");
		}
		
		if (execlp("cut", "cut", "-d:", "-f7", "/etc/passwd", (char*)NULL) < 0) {
			int old_errno = errno;
			close(pipeA[1]);
			errno = old_errno;
			err(3, "Error executing command: cut");
		}
	}

	int pipeB[2];
	if (pipe(pipeB) < 0) {
		int old_errno = errno;
		close(pipeA[0]);	close(pipeA[1]);
		errno = old_errno;
		err(1, "Error creating pipe");
	}

	pid_t second = fork();
	if (second < 0) {
		int old_errno = errno;
		close(pipeA[0]);	close(pipeA[1]);
		close(pipeB[0]);	close(pipeB[1]);
		errno = old_errno;
		err(2, "Error forking process");
	}
	else if (second == 0) {
		close(pipeA[1]);
		close(pipeB[0]);
		
		if(dup2(pipeA[0], 0) < 0 || dup2(pipeB[1], 1) < 0) {
			int old_errno = errno;
			close(pipeA[0]);	close(pipeB[1]);
			errno = old_errno;
			err(4, "Error executing dup2");
		}

		if(execlp("sort", "sort", (char*)NULL) < 0) {
			int old_errno = errno;
			close(pipeA[0]);	close(pipeB[1]);
			errno = old_errno;
			err(3, "Error executing command: sort");
		}
	}

	int pipeC[2];
	if (pipe(pipeC) < 0) {
		int old_errno = errno;
		close(pipeA[0]);	close(pipeA[1]);
		close(pipeB[0]);	close(pipeB[1]);
		errno = old_errno;
		err(1, "Error creating pipe");
	}

	pid_t third = fork();
	if (third < 0) {
		int old_errno = errno;
		close(pipeA[0]);	close(pipeA[1]);
		close(pipeB[0]);	close(pipeB[1]);
		close(pipeC[0]);	close(pipeC[1]);
		errno = old_errno;
		err(2, "Error forking process");
	}
	else if (third == 0) {
		close(pipeA[0]);	close(pipeA[1]);
		close(pipeB[1]);
		close(pipeC[0]);
		
		if(dup2(pipeB[0], 0) < 0 || dup2(pipeC[1], 1) < 0) {
			int old_errno = errno;
			close(pipeB[0]);	close(pipeC[1]);
			errno = old_errno;
			err(4, "Error executing dup2");
		}

		if(execlp("uniq", "uniq", "-c", (char*)NULL) < 0) {
			int old_errno = errno;
			close(pipeB[0]);	close(pipeC[1]);
			errno = old_errno;
			err(3, "Error executing command: uniq");
		}
	}

	close(pipeA[0]);	close(pipeA[1]);
	close(pipeB[0]);	close(pipeB[1]);
	close(pipeC[1]);

	if(dup2(pipeC[0], 0) < 0) {
		int old_errno = errno;
		close(pipeC[0]);
		errno = old_errno;
		err(4, "Error executing dup2");
	}

	if (execlp("sort", "sort", "-n", (char*)NULL) < 0) {
		int old_errno = errno;
		close(pipeC[0]);
		errno = old_errno;
		err(3, "Error executing command: sort");
	}
}	
