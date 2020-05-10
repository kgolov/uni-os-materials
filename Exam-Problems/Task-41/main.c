#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

int comparator(const void* p1, const void* p2) {
	uint16_t num1 = *(const uint16_t*)p1;
	uint16_t num2 = *(const uint16_t*)p2;

	return num1 - num2;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "Wrong parameters! Usage: %s [input.bin] [output.bin]", argv[0]);
	}
	
	struct stat st;
	if (stat(argv[1], &st) < 0) {
		err(2, "Error stat file %s", argv[1]);
	}
	
	if (st.st_size / sizeof(uint16_t) >= UINT16_MAX) {
		errx(3, "File is too big");
	}
	else if (st.st_size % sizeof(uint16_t) != 0) {
		errx(3, "File is not aligned");
	}
	
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 0) {
		err(4, "Error opening file %s", argv[1]);
	}

	int fd2 = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, st.st_mode);
	if (fd2 < 0) {
		int old_errno = errno;
		close(fd1);
		errno = old_errno;
		err(4, "Error opening file %s", argv[2]);
	}

	uint16_t* buffer = malloc(st.st_size);
	if (!buffer) {
		int old_errno = errno;
		close(fd1);
		close(fd2);
		errno = old_errno;
		err(5, "Error allocating memory");
	}

	if (read(fd1, buffer, st.st_size) < 0) {
		int old_errno = errno;
		close(fd1);
		close(fd2);
		free(buffer);
		errno = old_errno;
		err(6, "Error read from file %s", argv[1]);
	}

	close(fd1);
	qsort(buffer, st.st_size / sizeof(uint16_t), sizeof(uint16_t), comparator);

	if (write(fd2, buffer, st.st_size) < 0) {
		int old_errno = errno;
		close(fd2);
		free(buffer);
		errno = old_errno;
		err(7, "Error write to file %s", argv[2]);
	}

	close(fd1);
	close(fd2);
	free(buffer);
	exit(0);
}
