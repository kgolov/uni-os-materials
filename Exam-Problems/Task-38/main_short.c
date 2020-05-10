#include <err.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

// No error checking
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

	char buff;
	char last_char = 0;
	while (read(0, &buff, sizeof(buff)) > 0) {
		if (mode == 0) {
			for (int i = 0; argv[1][i] != '\0'; i++) {
				if (buff == argv[1][i]) {
					buff = argv[2][i];
				}
			}
			write(1, &buff, sizeof(buff));
		}
		else if (mode == 1) {
			bool print = true;
			for (int i = 0; argv[2][i] != '\0'; i++) {
				if (buff == argv[2][i]) {
					print = false;
				}
			}
			if (print) {
				write(1, &buff, sizeof(buff));
			}
		} 
		else {
			if ( (buff != argv[2][0]) || (buff == argv[2][0] && buff != last_char) ) {
				write(1, &buff, sizeof(buff));
			}
			last_char = buff;		
		}
	}

	exit(0);
}
