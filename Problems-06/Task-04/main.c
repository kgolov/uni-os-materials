#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main (int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "Expected two arguments");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 0) {
		err(2, "Error opening file %s", argv[1]);
	}

	int fd2 = open(argv[2], O_RDONLY);
	if (fd2 < 0) {
		int old_errno = errno;
		close(fd1);
		errno = old_errno;
		err(2, "Error opening file %s", argv[2]);
	}

	struct stat st1;
	if ( stat(argv[1], &st1) < 0 ) {
		int old_errno = errno;
		close(fd1);
		close(fd2);
		errno = old_errno;
		err(3, "Error stat file %s", argv[1]);
	}

	char* temp;
	if (st1.st_size != 0) {
		temp = malloc(st1.st_size);
		if (!temp) {
			int old_errno = errno;
			close(fd1);
			close(fd2);
			errno = old_errno;
			err(4, "Error allocating memory");
		}

		ssize_t bytes_read = read(fd1, temp, st1.st_size);
		if (bytes_read != st1.st_size) {
			int old_errno = errno;
			close(fd1);
			close(fd2);
			free(temp);
			errno = old_errno;
			err(5, "Error reading from file %s", argv[1]);
		}
	}

	close(fd1);
	fd1 = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd1 < 0) {
		int old_errno = errno;
		close(fd2);
		free(temp);
		errno = old_errno;
		err(6, "Error opening file %s for writing", argv[1]);
	}

	ssize_t bytes_read;
	char buff[4096];
	while ( (bytes_read = read(fd2, &buff, sizeof(buff))) > 0) {
		if (write(fd1, &buff, bytes_read) != bytes_read) {
			int old_errno = errno;
			close(fd1);
			close(fd2);
			free(temp);
			errno = old_errno;
			err(7, "Error writing to file %s", argv[1]);
		}
	}

	close(fd1);
	close(fd2);
	fd2 = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd2 < 0) {
		int old_errno = errno;
		free(temp);
		errno = old_errno;
		err(6, "Error opening file %s for writing", argv[2]);
	}

	if (st1.st_size != 0) {
		if (write(fd2, temp, st1.st_size) != st1.st_size) {
			int old_errno = errno;
			free(temp);
			close(fd2);
			errno = old_errno;
			err(7, "Error writing to file %s", argv[2]);
		}
	}
	
	free(temp);
	close(fd2);
	exit(0);
}
