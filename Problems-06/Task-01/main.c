#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "Two files are needed");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 0) {
		err(2, "Error opening source file");
	}

	int fd2 = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd2 < 0) {
		int old_errno = errno;
		close(fd1);
		errno = old_errno;
		err(3, "Error opening destination file");
	}

	char buff[4096];
	ssize_t bytes_read;
	while ( (bytes_read = read(fd1, &buff, sizeof(buff))) > 0 ) {
		write(fd2, buff, bytes_read);
	}

	close(fd1);
	close(fd2);

	exit(0);
}
