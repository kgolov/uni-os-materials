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
	if (argc < 3) {
		errx(1, "At least two arguments expected");
	}

	for (int i = 1; i < argc - 1; i++) {
		char* source_basename = malloc(strlen(basename(argv[i])) + 1);
		if (!source_basename) {
			warn("Error allocating memory");
			continue;
		}
		strcpy(source_basename, basename(argv[i]));
		
		char* destination_path = malloc(strlen(argv[argc - 1]) + 1 + strlen(source_basename) + 1);
		if (!destination_path) {
			int old_errno = errno;
			free(source_basename);
			errno = old_errno;
			warn("Error allocating memory");
			continue;
		}
		
		strcpy(destination_path, argv[argc - 1]);
		strcat(destination_path, "/");
		strcat(destination_path, source_basename);
		free(source_basename);

		// Stat source file to get permissions
		struct stat st_src;
		if (stat(argv[i], &st_src) < 0) {
			int old_errno = errno;
			free(destination_path);
			errno = old_errno;
			warn("Error stat source: %s", argv[i]);
			continue;
		}

		int fd2 = open(destination_path, O_CREAT | O_TRUNC | O_WRONLY, S_IWUSR);
		if (fd2 < 0) {
			int old_errno = errno;
			free(destination_path);
			errno = old_errno;
			warn("Error opening path for writing");
			continue;
		}
	
		int fd1 = open(argv[i], O_RDONLY);
		if (fd1 < 0) {
			int old_errno = errno;
			free(destination_path);
			close(fd2);
			errno = old_errno;
			warn("Error opening file %s for reading", argv[i]);
			continue;
		}
	
		char buff[4096];
		ssize_t bytes_read;
		while ( (bytes_read = read(fd1, &buff, sizeof(buff))) > 0) {
			if ( write(fd2, &buff, bytes_read) != bytes_read ) {
				int old_errno = errno;
				free(destination_path);
				close(fd1);
				close(fd2);
				errno = old_errno;
				warn("Error writing to destination file");
				continue;
			}
		}

		close(fd1);
		close(fd2);

		// Last, try to chmod dest file
		if ( chmod(destination_path, st_src.st_mode) < 0 ) {
			int old_errno = errno;
			free(destination_path);
			errno = old_errno;
			warn("Error chmod dest file");
			continue;
		}

		free(destination_path);
	}

	exit(0);
}
