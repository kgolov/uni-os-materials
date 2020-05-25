#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

// To sort the file according to the restrictions we will use the following technique:
// Split the file in two - sort the first part with QSORT()
// 			   save the sorted first part in a file
// 			   sort the second part with QSORT()
// 			   save the sorted second part in a file
// 			   merge the sorted parts (while reading from temp files)
// Sorting will be done inplace.

int comparator(const void* p1, const void* p2) {
	uint32_t num1 = *(const uint32_t*) p1;
	uint32_t num2 = *(const uint32_t*) p2;

	return num1 - num2;
}

int main (int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "One parameter expected");
	}

	// Firstly, try to stat the file
	struct stat st;
	if (stat(argv[1], &st) < 0) {
		err(2, "Error stat file %s", argv[1]);
	}
	if (st.st_size == 0) {
		exit(0);
	}
	else if (st.st_size % sizeof(uint32_t) != 0) {
		errx(3, "File %s is not aligned properly", argv[1]);
	}

	int fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		err(4, "Error opening file %s", argv[1]);
	}

	// Split the file into two parts
	// Start sorting the first part
	uint32_t first_half_size = (st.st_size / sizeof(uint32_t)) / 2;
	uint32_t second_half_size = (st.st_size / sizeof(uint32_t)) - first_half_size;
	uint32_t* buffer = malloc(first_half_size * sizeof(uint32_t));
	if (!buffer) {
		int old_errno = errno;
		close(fd);
		errno = old_errno;
		err(5, "Error allocating memory");
	}

	if ( read(fd, buffer, (first_half_size * sizeof(uint32_t))) < 0 ) {
		int old_errno = errno;
		close(fd);
		free(buffer);
		errno = old_errno;
		err(6, "Error reading from file %s", argv[1]);
	}

	qsort(buffer, first_half_size, sizeof(uint32_t), comparator);
	
	// Save sorted sequence in temporary file
	int temp1 = open(".temp1.dat", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if (temp1 < 0) {
		int old_errno = errno;
		close(fd);
		free(buffer);
		errno = old_errno;
		err(4, "Error opening file temp1.dat");
	}

	if ( write(temp1, buffer, (first_half_size * sizeof(uint32_t))) <= 0 ) {
		int old_errno = errno;
		close(fd);
		close(temp1);
		free(buffer);
		errno = old_errno;
		err(7, "Error writing to file temp1.dat");
	}

	// We can now free the allocated memory, and allocate new memory for the second part
	free(buffer);
	buffer = malloc(second_half_size * sizeof(uint32_t));
	if (!buffer) {
		int old_errno = errno;
		close(fd);
		close(temp1);
		errno = old_errno;
		err(5, "Error allocating memory");
	}

	// Repeat the reading and sorting for the second part
	if ( read(fd, buffer, (second_half_size * sizeof(uint32_t))) < 0 ) {
		int old_errno = errno;
		close(fd);
		close(temp1);
		free(buffer);
		errno = old_errno;
		err(6, "Error reading from file %s", argv[1]);
	}

	qsort(buffer, second_half_size, sizeof(uint32_t), comparator);

	// Second part is now sorted - save in temporary file
	int temp2 = open(".temp2.dat", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if (temp2 < 0) {
		int old_errno = errno;
		close(fd);
		close(temp1);
		free(buffer);
		errno = old_errno;
		err(4, "Error opening file temp2.dat");
	}

	if (write(temp2, buffer, (second_half_size * sizeof(uint32_t))) <= 0) {
		int old_errno = errno;
		close(fd);
		close(temp1);
		close(temp2);
		free(buffer);
		errno = old_errno;
		err(7, "Error writing to file temp2.dat");
	}

	free(buffer);

	if (lseek(fd, 0, SEEK_SET) < 0) {
		int old_errno = errno;
		close(fd);
		close(temp1);
		close(temp2);
		errno = old_errno;
		err(8, "Error seeking in file %s", argv[1]);
	}

	// Begin merge sorting
	uint32_t first_index = 0;
	uint32_t second_index = 0;
	
	uint32_t num1;
	uint32_t num2;

	while (first_index < first_half_size && second_index < second_half_size) {
		if (lseek(temp1, (first_index * sizeof(uint32_t)), SEEK_SET) < 0) {
			int old_errno = errno;
			close(fd);
			close(temp1);
			close(temp2);
			errno = old_errno;
			err(8, "Error seeking in file temp1.dat");
		}
		if (lseek(temp2, (second_index * sizeof(uint32_t)), SEEK_SET) < 0) {
			int old_errno = errno;
			close(fd);
			close(temp1);
			close(temp2);
			errno = old_errno;
			err(8, "Error seeking in file temp2.dat");
		}

		if (read(temp1, &num1, sizeof(num1)) < 0) {
			int old_errno = errno;
			close(fd);
			close(temp1);
			close(temp2);
			errno = old_errno;
			err(6, "Error reading from file temp1.dat");
		}
		if (read(temp2, &num2, sizeof(num2)) < 0) {
			int old_errno = errno;
			close(fd);
			close(temp1);
			close(temp2);
			errno = old_errno;
			err(6, "Error reading from file temp2.dat");
		}

		if (comparator(&num1, &num2) < 0) {
			// Write num1
			if (write(fd, &num1, sizeof(num1)) <= 0) {
				int old_errno = errno;
				close(fd);
				close(temp1);
				close(temp2);
				errno = old_errno;
				err(7, "Error writing to file %s", argv[1]);
			}

			first_index++;
		}
		else {
			// Write num2
			if (write(fd, &num2, sizeof(num2)) <= 0) {
				int old_errno = errno;
				close(fd);
				close(temp1);
				close(temp2);
				errno = old_errno;
				err(7, "Error writing to file %s", argv[1]);
			}
			
			second_index++;
		}
	}

	while (first_index < first_half_size) {
		if (read(temp1, &num1, sizeof(num1)) < 0) {
			int old_errno = errno;
			close(fd);
			close(temp1);
			close(temp2);
			errno = old_errno;
			err(6, "Error reading from file temp1.dat");
		}

		if (write(fd, &num1, sizeof(num1)) <= 0) {
			int old_errno = errno;
			close(fd);
			close(temp1);
			close(temp2);
			errno = old_errno;
			err(7, "Error writing to file %s", argv[1]);
		}

		first_index++;
	}
	while (second_index < second_half_size) {
		if (read(temp2, &num2, sizeof(num2)) < 0) {
			int old_errno = errno;
			close(fd);
			close(temp1);
			close(temp2);
			errno = old_errno;
			err(6, "Error reading from file temp2.dat");
		}

		if (write(fd, &num2, sizeof(num2)) <= 0) {
			int old_errno = errno;
			close(fd);
			close(temp1);
			close(temp2);
			errno = old_errno;
			err(7, "Error writing to file %s", argv[1]);
		}

		second_index++;
	}
	
	// File should be merge sorted now, exiting
	close(fd);
	close(temp1);
	close(temp2);
	exit(0);
}
