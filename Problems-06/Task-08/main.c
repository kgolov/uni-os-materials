#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

int main() {
	int fd1 = open("/etc/passwd", O_RDONLY);
	if (fd1 < 0) {
		err(1, "Error opening /etc/passwd");
	}
	int fd2 = open("./passwd", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd2 < 0) {
		int old_errno = errno;
		close(fd1);
		errno = old_errno;
		err(2, "Error opening dest file for writing");
	}

	char buff;
	while ( read(fd1, &buff, 1) ) {
		if (buff == ':') {
			buff = '?';
		}
		if (! write(fd2, &buff, 1) ) {
			int old_errno = errno;
			close(fd1);
			close(fd2);
			errno = old_errno;
			err(3, "Error writing to file");
		}
	}

	close(fd1);
	close(fd2);

	// Try to make permissions the same
	struct stat st;
	if (stat("/etc/passwd", &st) < 0) {
		err(4, "Error stat file /etc/passwd");
	}
	if (chmod("./passwd", st.st_mode) < 0) {
		err(5, "Error chmod file ./passwd");
	}

	exit(0);
}
