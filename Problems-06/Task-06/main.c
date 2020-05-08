#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		errx(1, "Need at least one parameter!");
	}

	for (int i = 1; i < argc; i++) {
		int fd = open(argv[i], O_RDONLY);
		if (fd < 0) {
			warn("Error opening file %s", argv[i]);
			continue;
		}
		
		char buff[4096];
		ssize_t bytes_read;
		while ( (bytes_read = read(fd, &buff, sizeof(buff))) > 0 ) {
			if (write(1, &buff, bytes_read) != bytes_read) {
				int old_errno = errno;
				close(fd);
				errno = old_errno;
				warn("Error writing to stdout");
				continue;
			}
		}
		close(fd);
	}
	
	exit(0);
}
