#include <err.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main (int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "One argument expected");
	}

	struct stat st;
	if (stat(argv[1], &st) < 0) {
		err(2, "Error stat file %s", argv[1]);
	}
	if (st.st_size == 0) {
		exit(0);
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		err(3, "Error opening file %s for reading", argv[1]);
	}

	uint8_t buff;
	uint8_t max = 0x00;
	while ( read(fd, &buff, sizeof(buff)) > 0 ) {
		if (buff > max) {
			max = buff;
		}	
	}
	
	printf("%d\n", max);

	close(fd);
	exit(0);
}
