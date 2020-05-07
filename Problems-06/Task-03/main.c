#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "Only one argument is needed");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 0) {
		err(2, "Error opening file %s", argv[1]);
	}

	char buff;
	uint32_t lines = 0;
	uint32_t words = 0;
	uint32_t chars = 0;
	while ( read(fd1, &buff, sizeof(buff)) > 0) {
		if (buff == '\n') {
			lines++;
			words++;
		} 
		else if (buff == ' ') {
			words++;
		}
		chars++;
	}

	printf(" %d %d %d %s\n", lines, words, chars, argv[1]);

	close(fd1);
	exit(0);
}
