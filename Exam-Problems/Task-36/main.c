#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
	if (argc == 1) {
		// Read from stdin, print to stdout
		char buff[4096];
		
		ssize_t bytes_read;
		while( (bytes_read = read(0, &buff, sizeof(buff))) > 0 ) {
			if(write(1, &buff, bytes_read) != bytes_read) {
				err(1, "Error writing to stdout");
			}
		}

		exit(0);
	}

	// We have at least one argument
	int index = 1;
	bool numberingLines = false;
	int lines = 1;
	bool newLine = true;
	setbuf(stdout, NULL);

	if (strcmp(argv[1], "-n") == 0) {
		numberingLines = true;
		index = 2;

		// If line numbering is set, but we dont have any files, we read only from stdin
		if (argc == 2) {
			// If line numbering is set, but we don't have any files, we read only from stdin
			char buff;
			while (read(0, &buff, sizeof(buff)) > 0) {
				if (newLine) {
					printf("%d ", lines);
					newLine = false;
				}

				if (buff == '\n') {
					lines++;
					newLine = true;
				}

				if (write(1, &buff, sizeof(buff)) <= 0) {
					err(1, "Error writing to stdout");
				}
			}

			exit (0);
		}
	}

	// Else, we have files to read
	for (; index < argc; index++) {
		// See if we need to open file, or read from stdin
		int fd;
		if (strcmp(argv[index], "-") == 0) {
			fd = 0;
		}
		else {
			fd = open(argv[index], O_RDONLY);
			if (fd < 0) {
				warn("Error opening file %s", argv[index]);
				continue;
			}
		}

		char buff;
		while(read(fd, &buff, sizeof(buff)) > 0) {
			if (numberingLines && newLine) {
				printf("%d ", lines);
				newLine = false;
			}

			if (buff == '\n') {
				lines++;
				newLine = true;
			}

			if (write(1, &buff, sizeof(buff)) <= 0) {
				err(1, "Error writing to stdout");
			}
		}

		if (fd != 0) {
			close(fd);
		}	
	}
	
	exit(0);
}
