#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// No error checking!
int main(int argc, char* argv[]) {
	if (argc < 3) {
		errx(1, "Not enough parameters");
	}

	if (strcmp(argv[1], "-c") == 0) {
		int n, m;
		if (strlen(argv[2]) == 1) {
			n = argv[2][0] - '0';
			m = n;
		}
		else if (strlen(argv[2]) == 3) {
			n = argv[2][0] - '0';
			m = argv[2][2] - '0';

			if (n > m) {
				errx(2, "Wrong second parameter");
			}
		}
		else {
			errx(2, "Wrong second parameter");
		}
		
		ssize_t bytes_read = 0;
		char buff;
		while ( read(0, &buff, sizeof(buff)) > 0) {
			bytes_read += sizeof(buff);

			if (buff == '\n') {
				bytes_read = 0;
				write(1, &buff, sizeof(buff));
				continue;
			}

			if (bytes_read >= n && bytes_read <= m) {
				write(1, &buff, sizeof(buff));
			}
		}
	}
	else if (strcmp(argv[1], "-d") == 0) {
		if (argc != 5) {
			errx(3, "In this mode four parameters are expected");
		}

		if (strcmp(argv[3], "-f") != 0) {
			errx(4, "Wrong third parameter");
		}

		char delim = argv[2][0];
		int n, m;
		if (strlen(argv[4]) == 1) {
			n = argv[4][0] - '0';
			m = n;
		}
		else if (strlen(argv[4]) == 3) {
			n = argv[4][0] - '0';
			m = argv[4][2] - '0';

			if (n > m) {
				errx(5, "Wrong fourth parameter");
			}
		}
		else {
			errx(5, "Wrong fourth parameter");
		}

		int column = 1;
		char buff;
		while (read(0, &buff, sizeof(buff)) > 0) {
			if (buff == delim) {
				column++;

				if (column > n && column <= m) {
					write(1, &buff, sizeof(buff));
				}
				continue;
			}
			else if (buff == '\n') {
				column = 1;
				write(1, &buff, sizeof(buff));
				continue;
			}

			if (column >= n && column <= m) {
				write(1, &buff, sizeof(buff));
			}
		}
	}
	
	exit(0);
}
