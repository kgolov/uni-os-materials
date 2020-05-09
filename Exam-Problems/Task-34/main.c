#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

// Complete, with error checking and comments
int main(int argc, char* argv[]) {
	if (argc != 5) {
		errx(1, "Program expects 4 parameters");
	}

	// Open all of the arguments
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 0) {
		err(2, "Error opening file %s for reading", argv[1]);
	}
	int fd2 = open(argv[2], O_RDONLY);
	if (fd2 < 0) {
		int old_errno = errno;
		close(fd1);
		errno = old_errno;
		err(2, "Error opening file %s for reading", argv[2]);
	}
	int fd3 = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd3 < 0) {
		int old_errno = errno;
		close(fd1);
		close(fd2);
		errno = old_errno;
		err(3, "Error opening file %s for writing", argv[3]);
	}
	int fd4 = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd4 < 0) {
		int old_errno = errno;
		close(fd1);
		close(fd2);
		close(fd3);
		errno = old_errno;
		err(3, "Error opening file %s for writing", argv[4]);
	}

	// Create a structure which we will use to read and write to index files - it consists as follows:
	struct data {
		uint16_t offset;
		uint8_t length;
		uint8_t reserved;
	};
	// Create an instance of that structure to use as a buffer for Read()
	struct data index;

	// Check alignment of index file - size must be proportional to the above structure
	struct stat st;
	if (stat(argv[2], &st) < 0) {
		int old_errno = errno;
		close(fd1);
		close(fd2);
		close(fd3);
		close(fd4);
		errno = old_errno;
		err(4, "Error stat file %s", argv[2]);
	}
	if (st.st_size % sizeof(index) != 0) {
		close(fd1);
		close(fd2);
		close(fd3);
		close(fd4);
		errx(5, "Index file is not aligned properly");
	}

	// Here, we will save the number of bytes written to f2.dat -> we will use this number to write to f2.idx at the end of the cycle
	uint16_t last_offset = 0x0000;
	while ( read(fd2, &index, sizeof(index)) > 0 ) {
		// Jump to the part of the file specified in OFFSET
		if (lseek(fd1, index.offset, SEEK_SET) < 0) {
			int old_errno = errno;
			close(fd1);
			close(fd2);
			close(fd3);
			close(fd4);
			errno = old_errno;
			err(6, "Error while seeking file");
		}

		// Read f1 character by character
		char ch;
		if (read(fd1, &ch, 1) <= 0) {
			int old_errno = errno;
			close(fd1);
			close(fd2);
			close(fd3);
			close(fd4);
			errno = old_errno;
			err(7, "Error reading from file");
		}
		// Check if first letter is a capital letter?
		if (ch <= 'A' || ch >= 'Z') {
			// It is not, discard this sequence
			continue;
		}

		// Try to write character to f2
		if (write(fd3, &ch, 1) <= 0) {
			int old_errno = errno;
			close(fd1);
			close(fd2);
			close(fd3);
			close(fd4);
			errno = old_errno;
			err(8, "Error writing to file");
		}

		// Continue with the rest of the string, while we reach the specified LENGTH
		ssize_t bytes_read = 1; // We have already read the first letter
		while (bytes_read < index.length) {
			// Try to read from file
			ssize_t try_read = read(fd1, &ch, 1);
			if (try_read <= 0) {
				int old_errno = errno;
				close(fd1);
				close(fd2);
				close(fd3);
				close(fd4);
				errno = old_errno;
				err(7, "Error reading from file");
			}
			bytes_read += try_read;

			// Try writing to file
			if (write(fd3, &ch, 1) <= 0) {
				int old_errno = errno;
				close(fd1);
				close(fd2);
				close(fd3);
				close(fd4);
				errno = old_errno;
				err(8, "Error writing to file");
			}
		}

		// Now we need to write to f2.idx
		// Construct the data to write -> offset is the number of bytes written until now (saved in last_offset)
		// 			       -> length is the current length
		// 			       -> reserved we set to 0x00, but it can be anything
		struct data towrite = { last_offset, index.length, 0x00 };

		// Try to write to index file
		if (write(fd4, &towrite, sizeof(towrite)) < (ssize_t)sizeof(towrite)) {
			int old_errno = errno;
			close(fd1);
			close(fd2);
			close(fd3);
			close(fd4);
			errno = old_errno;
			err(8, "Error writing to file");
		}

		// Update last_offset with the bytes written in this iteration
		last_offset += index.length;
	}

	// Close everything and exit
	close(fd1);
	close(fd2);
	close(fd3);
	close(fd4);

	exit(0);
}
