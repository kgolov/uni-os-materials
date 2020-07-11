#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>

int main (int argc, char** argv) {
	if (argc != 4) {
		errx(1, "Program expects three parameters! Usage: %s [F1] [F2] [OUTPUT]", argv[0]);
	}
		
	struct data {
		uint32_t offset;
		uint32_t length;
	};
	struct data dat;
	struct stat st;

	if (stat(argv[1], &st) < 0) {
		err(2, "Error stat file %s", argv[1]);
	}
	
	if (st.st_size % sizeof(dat) != 0) {
		errx(3, "File %s not aligned properly!", argv[1]);
	}

	if (stat(argv[2], &st) < 0) {
		err(2, "Error stat file %s", argv[2]);
	}

	if (st.st_size % sizeof(uint32_t) != 0) {
		errx(3, "File %s not aligned properly!", argv[2]);
	}

	// Open file descriptors
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 0) {
		err(4, "Error opening file %s", argv[1]);
	}
	int fd2 = open(argv[2], O_RDONLY);
	if (fd2 < 0) {
		int old_errno = errno;
		close(fd1);
		errno = old_errno;
		err(4, "Error opening file %s", argv[2]);
	}
	int fd3 = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd3 < 0) {
		int old_errno = errno;
		close(fd1);
		close(fd2);
		errno = old_errno;
		err(4, "Error opening file %s", argv[3]);
	}

	while (read(fd1, &dat, sizeof(dat)) > 0) {
		if (lseek(fd2, (dat.offset - 1) * sizeof(uint32_t), SEEK_SET) < 0) {
			int old_errno = errno;
			close(fd1);
			close(fd2);
			close(fd3);
			errno = old_errno;
			err(5, "Invalid offset!");
		}
		
		uint32_t counter = 0;
		uint32_t buf;
		ssize_t bytes_read;
		while (counter < dat.length) {
			bytes_read = read(fd2, &buf, sizeof(buf));
			if (bytes_read < 0) {
				int old_errno = errno;
				close(fd1);
				close(fd2);
				close(fd3);
				errno = old_errno;
				err(6, "Error reading from file %s", argv[2]);
			} 
			else if (bytes_read == 0) {
				break;
			}
			
			if (write(fd3, &buf, sizeof(buf)) < 0) {
				int old_errno = errno;
				close(fd1);
				close(fd2);
				close(fd3);
				errno = old_errno;
				err(7, "Error writing to file %s", argv[3]);
			}
			counter++;
		}
	}

	exit(0);
}
