#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

// Works with relative and absolute paths, thanks to library call to basename(3)

int main (int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "Two arguments expected");
	}

	char* source_basename = malloc(strlen(basename(argv[1])) + 1);
	if (!source_basename) {
		err(2, "Error allocating memory");
	}
	strcpy(source_basename, basename(argv[1]));

	// Next, check if destination is a directory, or a filename
	struct stat st_dest;
	char* destination_path;
	bool memory_allocated = false;
	if (stat(argv[2], &st_dest) < 0) {
		// Probably is a file that doesnt exist, so we will create it
		destination_path = argv[2];
	} else {
		if (S_ISDIR(st_dest.st_mode)) {
			destination_path = malloc(strlen(argv[2]) + 1 + strlen(source_basename) + 1);
			if (!destination_path) {
				int old_errno = errno;
				free(source_basename);
				errno = old_errno;
				err(2, "Error allocating memory");
			}
			memory_allocated = true;
			strcpy(destination_path, argv[2]);
			strcat(destination_path, "/");
			strcat(destination_path, source_basename);
		}
       		else {
			destination_path = argv[2];
		}
	}

	free(source_basename);

	// Stat source file to get permissions as well
	struct stat st_src;
	if (stat(argv[1], &st_src) < 0) {
		int old_errno = errno;
		if (memory_allocated) {
			free(destination_path);
		}
		errno = old_errno;
		err(3, "Error stat source: %s", argv[1]);
	}

	int fd2 = open(destination_path, O_CREAT | O_TRUNC | O_WRONLY, S_IWUSR);
	if (fd2 < 0) {
		int old_errno = errno;
		if (memory_allocated) {
			free(destination_path);
		}
		errno = old_errno;
		err(4, "Error opening path for writing");
	}
	
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 0) {
		int old_errno = errno;
		if (memory_allocated) {
			free(destination_path);
		}
		close(fd2);
		errno = old_errno;
		err(6, "Error opening file %s for reading", argv[1]);
	}
	
	char buff[4096];
	ssize_t bytes_read;
	while ( (bytes_read = read(fd1, &buff, sizeof(buff))) > 0) {
		if ( write(fd2, &buff, bytes_read) != bytes_read ) {
			int old_errno = errno;
			if (memory_allocated) {
				free(destination_path);
			}
			close(fd1);
			close(fd2);
			errno = old_errno;
			err(7, "Error writing to destination file");
		}
	}

	close(fd1);
	close(fd2);

	// Last, try to chmod dest file
	if ( chmod(destination_path, st_src.st_mode) < 0 ) {
		int old_errno = errno;
		if (memory_allocated) {
			free(destination_path);
		}
		errno = old_errno;
		err(8, "Error chmod dest file");
	}

	if (memory_allocated) {
		free(destination_path);
	}
	exit(0);
}
