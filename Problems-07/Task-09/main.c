#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main (int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "One parameter expected");
	}

	int fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		err(2, "Error opening file %s for writing", argv[1]);
	}

	pid_t first = fork();
	if (first < 0) {
		int old_errno = errno;
		close(fd);
		errno = old_errno;
		err(3, "Error forking");
	}
	else if (first == 0) {
		if (write(fd, "foo", 4) != 4) {
			int old_errno = errno;
			close(fd);
			errno = old_errno;
			err(4, "Error writing to file %s", argv[1]);
		}

		close(fd);
		exit(0);
	} 
	

	wait(NULL);
	pid_t second = fork();
	if (second < 0) {
		int old_errno = errno;
		close(fd);
		errno = old_errno;
		err(3, "Error forking");
	}
	else if (second == 0) {
		if (write(fd, "bar", 4) != 4) {
			int old_errno = errno;
			close(fd);
			errno = old_errno;
			err(4, "Error writing to file %s", argv[1]);
		}
	}

	close(fd);
	exit(0);
}
