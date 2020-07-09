#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char** argv) {
	if (argc != 4) {
		errx(1, "Program expects three parameters! Usage: %s [PATCH] [FILE1] [FILE2]", argv[0]);
	}

	// Try to stat patch file and f1
	struct stat patch;
	struct stat file1;
	if (stat(argv[1], &patch) < 0) {
		err(2, "Error stat file %s", argv[1]);
	}
	if (stat(argv[2], &file1) < 0) {
		err(2, "Error stat file %s", argv[2]);
	}

	struct data {
		uint16_t offset;
		uint8_t old_byte;
		uint8_t new_byte;
	};

	struct data patchdata;
	if (patch.st_size % sizeof(patchdata) != 0) {
		errx(3, "%s is not aligned properly!", argv[1]);
	} 

	// Show warning if f1 is a size, greater than the maximum offset of uint16_t
	if (file1.st_size > 0xFFFF) {
		warnx("%s is too big! Possible loss of data.", argv[2]);
	}

	// Open file descriptors
	int pfd = open(argv[1], O_RDONLY);
	if (pfd < 0) {
		err(4, "Error opening file %s", argv[1]);
	}
	int fd1 = open(argv[2], O_RDONLY);
	if (fd1 < 0) {
		int old_errno = errno;
		close(pfd);
		errno = old_errno;
		err(4, "Error opening file %s", argv[2]);
	}
	int fd2 = open(argv[3], O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd2 < 0) {
		int old_errno = errno;
		close(pfd);
		close(fd1);
		errno = old_errno;
		err(4, "Error opening file %s", argv[3]);
	}

	uint8_t buf;
	ssize_t bytes_read;
	while ( ( bytes_read = read(fd1, &buf, sizeof(buf)) ) > 0 ) { 
		if (write(fd2, &buf, bytes_read) < 0) {
			int old_errno = errno;
	       		close(pfd);
			close(fd1);
			close(fd2);
			errno = old_errno;		
			err(5, "Error writing to file %s", argv[3]);
		}
	}

	close(fd1);

	while ( ( bytes_read = read(pfd, &patchdata, sizeof(patchdata)) ) > 0 ) {
		if (patchdata.offset > file1.st_size || (lseek(fd2, patchdata.offset, SEEK_SET) < 0)) {
			int old_errno = errno;
			close(pfd);
			close(fd2);
			errno = old_errno;
			errx(6, "Invalid offset!");
		}
		
		// Else, we have seeked and we can now check the byte at this position
		if (read(fd2, &buf, sizeof(buf)) < 0) {
			int old_errno = errno;
			close(pfd);
			close(fd2);
			errno = old_errno;
			err(5, "Error reading from file %s", argv[3]);
	       	}	       

		if (buf != patchdata.old_byte) {
			int old_errno = errno;
			close(pfd);
			close(fd2);
			errno = old_errno;
			errx(7, "Patch data does not match");
		}

		if (lseek(fd2, patchdata.offset, SEEK_SET) < 0) {
			int old_errno = errno;
			close(pfd);
			close(fd2);
			errno = old_errno;
			errx(8, "Error seeking in file %s", argv[3]);
		}

		if (write(fd2, &patchdata.new_byte, sizeof(patchdata.new_byte)) < 0) {
			int old_errno = errno;
			close(pfd);
			close(fd2);
			errno = old_errno;
			err(5, "Error writing to file %s", argv[3]);
		}
	}

	close(pfd);
	close(fd2);
	exit(0);
}
