#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdint.h>

int comparator(const void* int1, const void* int2) {
	return ((*(uint8_t*)int1) - (*(uint8_t*)int2));
}

int main (int argc, char** argv) {
	if (argc != 2) {
		errx(1, "Program expects one parameter! Usage: %s [FILE]", argv[0]);
	}
	
	struct stat st;
	if (stat(argv[1], &st) < 0) {
		err(2, "Error stat file %s!", argv[1]);
	}

	uint8_t* buff = malloc(st.st_size);
	if (buff == NULL) {
		err(3, "Error allocating %ld bytes of memory!", st.st_size);
	}

	int fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		int old_errno = errno;
		free(buff);
		errno = old_errno;
		err(4, "Error opening file %s", argv[1]);
	}

	if (read(fd, buff, st.st_size) < 0) {
		int old_errno = errno;
		free(buff);
		close(fd);
		errno = old_errno;
		err(5, "Error reading from file %s", argv[1]);
	}

	qsort(buff, st.st_size, sizeof(uint8_t), comparator);

	if (lseek(fd, 0, SEEK_SET) < 0) {
		int old_errno = errno;
		free(buff);
		close(fd);
		errno = old_errno;
		err(6, "Error seeking in file %s", argv[1]);
	}	

	if (write(fd, buff, st.st_size) < 0) {
		int old_errno = errno;
		free(buff);
		close(fd);
		errno = old_errno;
		err(7, "Error writing to file %s", argv[1]);
	}

	free(buff);
	close(fd);
	
	exit(0);
}
