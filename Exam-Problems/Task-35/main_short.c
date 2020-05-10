#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

// No error checking
int main (int argc, char* argv[]) {
	if (argc != 4) {
		errx(1, "Three arguments expected");
	}
		
	int file1 = open(argv[1], O_RDONLY);
	int file2 = open(argv[2], O_RDONLY);
	int patch = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);

	uint8_t buff1;
	uint8_t buff2;

	ssize_t bytes_read;
	uint16_t offset = 0;
	while ( (bytes_read = read(file1, &buff1, sizeof(buff1)) > 0) && (read(file2, &buff2, sizeof(buff2)) > 0) ) {
		if (buff1 != buff2) {
			// Save in patch file
			write(patch, &offset, sizeof(offset));
			write(patch, &buff1, sizeof(buff1));
			write(patch, &buff2, sizeof(buff2));
		}

		offset += bytes_read;
	}

	// Check if we are at the end of both files (try to read further)
	if ( (read(file1, &buff1, sizeof(buff1)) > 0) || (read(file2, &buff2, sizeof(buff2)) > 0) ) {
		// Error!
		// Clear patch file and exit
		close(patch);
		patch = open(argv[3], O_TRUNC);
		close(patch);
		
		close(file1);
		close(file2);
		
		errx(2, "Files not the same length!");
	}

	close(file1);
	close(file2);
	close(patch);
	exit(0);
}
