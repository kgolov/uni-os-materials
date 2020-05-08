#include <err.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main (int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "Two arguments expected");
	}

	uint8_t mode = 0;
	if (strcmp(argv[1], "--min") == 0) {
		mode = 1;
	}
	else if (strcmp(argv[1], "--max") == 0) {
		mode = 2;
	}
	else if (strcmp(argv[1], "--print") == 0) {
		mode = 3;
	}
	else {
		errx(2, "First argument unknown!");
	}

	uint16_t buff;
	struct stat st;
	if (stat(argv[2], &st) < 0) {
		err(3, "Error stat file %s", argv[2]);
	}
	
	if (st.st_size == 0) {
		exit(0);
	}
	else if (st.st_size % sizeof(buff) != 0) {
		errx(4, "File is not aligned");
	}

	int fd = open(argv[2], O_RDONLY);
	if (fd < 0) {
		err(5, "Error opening file %s for reading", argv[2]);
	}

	uint16_t max = 0x0000;
	uint16_t min = 0xFFFF;
	while ( read(fd, &buff, sizeof(buff)) > 0 ) {
		if (mode == 1 && buff < min) {
			min = buff;
		}
		else if (mode == 2 && buff > max) {
			max = buff;
		}
		else if (mode == 3) {
			printf("%d\n", buff);
		}
	}

	if (mode == 1) {
		printf("%d\n", min);
	}
	else if (mode == 2) {
		printf("%d\n", max);
	}

	close(fd);
	exit(0);
}
