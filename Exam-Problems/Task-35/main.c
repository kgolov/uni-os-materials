#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

int main (int argc, char* argv[]) {
	if (argc != 4) {
		errx(1, "Three arguments expected");
	}

	struct stat st1;
	struct stat st2;
	if (stat(argv[1], &st1) < 0) {
		err(2, "Error stat file %s", argv[1]);
	}
	if (stat(argv[2], &st2) < 0) {
		err(2, "Error stat file %s", argv[2]);
	}

	if (st1.st_size != st2.st_size) {
		errx(3, "Files not the same length!");
	}

	int file1 = open(argv[1], O_RDONLY);
	if (file1 < 0) {
		err(4, "Error open file %s", argv[1]);
	}
	int file2 = open(argv[2], O_RDONLY);
	if (file2 < 0) {
		int old_errno = errno;
		close(file1);
		errno = old_errno;
		err(4, "Error open file %s", argv[2]);
	}
	int patch = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (patch < 0) {
		int old_errno = errno;
		close(file1);
		close(file2);
		errno = old_errno;
		err(4, "Error open file %s", argv[3]);
	}

	struct data {
		uint16_t offset;
		uint8_t buff1;
		uint8_t buff2;
	};
	struct data dt = { 0, 0, 0 };

	ssize_t bytes_read;
	while ( (bytes_read = read(file1, &dt.buff1, sizeof(dt.buff1)) > 0) && (read(file2, &dt.buff2, sizeof(dt.buff2)) > 0) ) {
		if (dt.buff1 != dt.buff2) {
			// Save in patch file
			if (write(patch, &dt, sizeof(dt)) != sizeof(dt)) {
				int old_errno = errno;
				close(file1);
				close(file2);
				close(patch);
				errno = old_errno;
				err(5, "Error writing to file %s", argv[3]);
			}
		}

		dt.offset += bytes_read;
	}

	close(file1);
	close(file2);
	close(patch);
	exit(0);
}
