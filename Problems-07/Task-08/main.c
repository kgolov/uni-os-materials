#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "One parameter expected! Usage: %s [FILE]", argv[0]);
	}

	int fd = open(argv[1], O_CREAT | O_TRUNC | O_RDWR, S_IWUSR | S_IRUSR);
	if (fd < 0) {
		err(2, "Error opening / creating file %s", argv[1]);
	}

	pid_t pid = fork();
	if (pid < 0) {
		err(3, "Error during fork");
	}
	else if (pid == 0) {
		if (write(fd, "foobar", 7) <= 0) {
			int old_errno = errno;
			close(fd);
			errno = old_errno;
			err(4, "Error writing to file %s", argv[1]);
		}
		close(fd);
		exit(0);
	}
	
	// In parent
	wait(NULL);
	
	if (lseek(fd, 0, SEEK_SET) < 0) {
		int old_errno = errno;
		close(fd);
		errno = old_errno;
		err(5, "Error seeking in file %s", argv[1]);
	}

	char buff[2];
	ssize_t bytes_read;
	while ((bytes_read = read(fd, &buff, sizeof(buff))) > 0) {
		if (write(1, &buff, bytes_read) <= 0) {
			int old_errno = errno;
			close(fd);
			errno = old_errno;
			err(6, "Error writing to stdout");
		}
		if (write(1, " ", 2) <= 0) {
			int old_errno = errno;
			close(fd);
			errno = old_errno;
			err(6, "Error writing to stdout");
		}
	}
	
	close(fd);
	exit(0);
}
