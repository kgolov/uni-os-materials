#include <err.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

// No error checking!
int main(int argc, char* argv[]) {
	if (argc != 5) {
		errx(1, "Program expects 4 parameters");
	}

	int fd1 = open(argv[1], O_RDONLY);
	int fd2 = open(argv[2], O_RDONLY);
	int fd3 = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	int fd4 = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);

	struct data {
		uint16_t offset;
		uint8_t length;
		uint8_t reserved;
	};
	struct data index;

	struct stat st;
	stat(argv[2], &st);
	if (st.st_size % sizeof(index) != 0) {
		errx(2, "Index file is not aligned properly");
	}

	uint16_t last_offset = 0x0000;
	while ( read(fd2, &index, sizeof(index)) > 0 ) {
		lseek(fd1, index.offset, SEEK_SET);
		char ch;
		// Check first character
		read(fd1, &ch, 1);
		if (ch <= 'A' || ch >= 'Z') {
			// Error, continue
			continue;
		}
		
		write(fd3, &ch, 1);
		ssize_t bytes_read = 1;
		while (bytes_read < index.length) {
			bytes_read += read(fd1, &ch, 1);
			write(fd3, &ch, 1);
		}

		struct data towrite = { last_offset, index.length, 0x00 };
		write(fd4, &towrite, sizeof(towrite));
		last_offset += index.length;
	}

	close(fd1);
	close(fd2);
	close(fd3);
	close(fd4);	

	exit(0);
}
