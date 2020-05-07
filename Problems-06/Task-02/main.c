#include <err.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "Only one parameter is needed");
	}

	const int LINES_TO_READ = 10;
	
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 0) {
		err(2, "Error opening file %s", argv[1]);
	}	

	char buff;
	uint8_t lines_read = 0;
	while ( read(fd1, &buff, sizeof(buff)) > 0 ) {
		if (buff == '\n') {
			lines_read++;
		}

		write(1, &buff, sizeof(buff));

		if (lines_read >= LINES_TO_READ) {
			break;
		}
	}

	close(fd1);
	exit(0);
}
