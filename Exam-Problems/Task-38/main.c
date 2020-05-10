#include <err.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "Program expects two parameters");
	}

	int mode = 0;
	if (strcmp(argv[1], "-d") == 0) {
		mode = 1;
	}
	else if (strcmp(argv[1], "-s") == 0) {
		mode = 2;
	}
	else {
		if (strlen(argv[1]) != strlen(argv[2])) {
			errx(2, "Two parameters need to be the same length");
		}	
	}

	char buff;
	char last_char = 0;
	while (read(0, &buff, sizeof(buff)) > 0) {
		if (mode == 0) {
			for (int i = 0; argv[1][i] != '\0'; i++) {
				if (buff == argv[1][i]) {
					buff = argv[2][i];
				}
			}

			if(write(1, &buff, sizeof(buff)) != sizeof(buff)) {
				err(3, "Error writing to stdout");
			}
		}
		else if (mode == 1) {
			bool print = true;
			for (int i = 0; argv[2][i] != '\0'; i++) {
				if (buff == argv[2][i]) {
					print = false;
				}
			}
			if (print) {
				if (write(1, &buff, sizeof(buff)) != sizeof(buff)) {
					err(3, "Error writing to stdout");
				}
			}
		} 
		else {
			if ( (buff != argv[2][0]) || (buff == argv[2][0] && buff != last_char) ) {
				if (write(1, &buff, sizeof(buff)) != sizeof(buff)) {
					err(3, "Error writing to stdout");
				}
			}
			last_char = buff;		
		}
	}

	exit(0);
}
