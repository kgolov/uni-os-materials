#include <stdlib.h>
#include <stdint.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int comparator(const void* p1, const void* p2) {
	return (*(uint32_t*)p1) - (*(uint32_t*)p2);
}

int main (int argc, char** argv) {
	if (argc != 3) {
		errx(1, "Program expects two parameters! Usage: %s [INPUT.BIN] [OUTPUT.BIN]", argv[0]);
	}

	struct stat st;
	if (stat(argv[1], &st) < 0) {
		err(2, "Error stat file %s", argv[1]);
	}

	if (st.st_size == 0) {
		exit(0);
	}
	else if (st.st_size % sizeof(uint32_t) != 0) {
		errx(3, "File is not aligned properly!");
	}

	// Open file descriptor for reading
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 0) {
		err(4, "Error opening file %s", argv[1]);
	}

	uint32_t numbers = st.st_size / sizeof(uint32_t);
	uint32_t first_half_size = numbers / 2;
	uint32_t second_half_size = numbers - first_half_size;

	// Firstly, sort the first half using qsort. Save in temp file called ".temp1.dat"
	uint32_t* buff = malloc(first_half_size * sizeof(uint32_t));
	if (buff == NULL) {
		int old_errno = errno;
		close(fd1);
		errno = old_errno;
		err(5, "Error allocating %ld bytes of memory", first_half_size * sizeof(uint32_t));
	}

	if (read(fd1, buff, first_half_size * sizeof(uint32_t)) < 0) {
		int old_errno = errno;
		close(fd1);
		free(buff);
		errno = old_errno;
		err(6, "Error reading from file %s", argv[1]);
	}

	int tempfd1 = open(".temp1.dat", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if (tempfd1 < 0) {
		int old_errno = errno;
		close(fd1);
		free(buff);
		errno = old_errno;
		err(4, "Error opening file %s", ".temp1.dat");
	}

	qsort(buff, first_half_size, sizeof(uint32_t), comparator);

	if (write(tempfd1, buff, first_half_size * sizeof(uint32_t)) < 0) {
		int old_errno = errno;
		close(fd1);
		close(tempfd1);
		free(buff);
		errno = old_errno;
		err(7, "Error writing to file %s", ".temp1.dat");
	}

	free(buff);
	
	// Now sort the second half using qsort. Save in temp file called "temp2.dat"
	buff = malloc(second_half_size * sizeof(uint32_t));
	if (buff == NULL) {
		int old_errno = errno;
		close(fd1);
		close(tempfd1);
		errno = old_errno;
		err(5, "Error allocating %ld bytes of memory", second_half_size * sizeof(uint32_t));
	}

	if (read(fd1, buff, second_half_size * sizeof(uint32_t)) < 0) {
		int old_errno = errno;
		close(fd1);
		close(tempfd1);
		free(buff);
		errno = old_errno;
		err(6, "Error reading from file %s", argv[1]);
	}

	close(fd1);

	int tempfd2 = open(".temp2.dat", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if (tempfd2 < 0) {
		int old_errno = errno;
		close(tempfd1);
		free(buff);
		errno = old_errno;
		err(4, "Error opening file %s", ".temp2.dat");
	}

	qsort(buff, second_half_size, sizeof(uint32_t), comparator);

	if (write(tempfd2, buff, first_half_size * sizeof(uint32_t)) < 0) {
		int old_errno = errno;
		close(tempfd1);
		close(tempfd2);
		free(buff);
		errno = old_errno;
		err(7, "Error writing to file %s", ".temp2.dat");
	}

	free(buff);
	
	// Now merge the two sorted halves.
	int fd2 = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd2 < 0) {
		int old_errno = errno;
		close(tempfd1);
		close(tempfd2);
		errno = old_errno;
		err(4, "Error opening file %s", argv[2]);
	}
/*
	if ((lseek(tempfd1, 0, SEEK_SET) < 0) || (lseek(tempfd2, 0, SEEK_SET) < 0)) {
		int old_errno = errno;
		close(tempfd1);
		close(tempfd2);
		close(fd2);
		errno = old_errno;
		err(7, "Error seeking in temporary files");
	}
*/
	uint32_t first_index = 0;
	uint32_t second_index = 0;

	uint32_t num1;
	uint32_t num2;

	while (first_index < first_half_size && second_index < second_half_size) {
		if ((lseek(tempfd1, first_index*sizeof(uint32_t), SEEK_SET) < 0) || 
					(lseek(tempfd2, second_index*sizeof(uint32_t), SEEK_SET) < 0)) {
			int old_errno = errno;
			close(tempfd1);
			close(tempfd2);
			close(fd2);
			errno = old_errno;
			err(8, "Error seeking in temporary files!");
		}

		if ((read(tempfd1, &num1, sizeof(num1)) < 0) || 
				(read(tempfd2, &num2, sizeof(num2)) < 0)) {
			int old_errno = errno;
			close(tempfd1);
			close(tempfd2);
			close(fd2);
			errno = old_errno;
			err(6, "Error reading from temporary files!");
		}
	
		if (comparator(&num1, &num2) < 0) {
			if (write(fd2, &num1, sizeof(num1)) < 0) {
				int old_errno = errno;
				close(tempfd1);
				close(tempfd2);
				close(fd2);
				errno = old_errno;
				err(7, "Error writing to file %s", argv[2]);
			}

			first_index++;
		}
		else {
			if (write(fd2, &num2, sizeof(num2)) < 0) {
				int old_errno = errno;
				close(tempfd1);
				close(tempfd2);
				close(fd2);
				errno = old_errno;
				err(7, "Error writing to file %s", argv[2]);
			}

			second_index++;
		}
	}

	while(first_index < first_half_size) {
		if (lseek(tempfd1, first_index*sizeof(uint32_t), SEEK_SET) < 0) {
			int old_errno = errno;
			close(tempfd1);
			close(tempfd2);
			close(fd2);
			errno = old_errno;
			err(8, "Error seeking in temp file");
		}

		if (read(tempfd1, &num1, sizeof(num1)) < 0) {
			int old_errno = errno;
			close(tempfd1);
			close(tempfd2);
			close(fd2);
			errno = old_errno;
			err(6, "Error reading from temp file");
		}

		if (write(fd2, &num1, sizeof(num1)) < 0) {
			int old_errno = errno;
			close(tempfd1);
			close(tempfd2);
			close(fd2);
			errno = old_errno;
			err(7, "Error writing to file %s", argv[2]);
		}
		
		first_index++;
	}
	while(second_index < second_half_size) {
		if (lseek(tempfd2, second_index*sizeof(uint32_t), SEEK_SET) < 0) {
			int old_errno = errno;
			close(tempfd1);
			close(tempfd2);
			close(fd2);
			errno = old_errno;
			err(8, "Error seeking in temp file");
		}

		if (read(tempfd2, &num2, sizeof(num2)) < 0) {
			int old_errno = errno;
			close(tempfd1);
			close(tempfd2);
			close(fd2);
			errno = old_errno;
			err(6, "Error reading from temp file");
		}

		if (write(fd2, &num2, sizeof(num2)) < 0) {
			int old_errno = errno;
			close(tempfd1);
			close(tempfd2);
			close(fd2);
			errno = old_errno;
			err(7, "Error writing to file %s", argv[2]);
		}
		
		second_index++;
	}

	close(tempfd1);
	close(tempfd2);
	close(fd2);

	exit(0);
}
