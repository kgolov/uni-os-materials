#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

// We will do inplace sorting with counting sort
int main(int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "One argument expected");
	}

	int fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		err(2, "Error opening file %s", argv[1]);
	}

	uint32_t countingMap[256];
	for (int i = 0; i < 256; i++) {
		countingMap[i] = 0;
	}
	uint8_t buffer;
	while ( read(fd, &buffer, sizeof(buffer)) > 0 ) {
		countingMap[buffer]++;
	}

	lseek(fd, 0, SEEK_SET);

	// Now, write to file according to histogram
	for (int i = 0; i < 256; i++) {
		uint8_t currentByte = i;
		while (countingMap[currentByte] > 0) {
			if (write(fd, &currentByte, sizeof(currentByte)) <= 0) {
				int old_errno = errno;
				close(fd);
				errno = old_errno;
				err(3, "Error writing to file %s", argv[1]);
			}
			countingMap[currentByte]--;
		}
	}

	close(fd);
	exit(0);
}
