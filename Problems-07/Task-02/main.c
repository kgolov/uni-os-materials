#include <unistd.h>
#include <err.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "Program expects 1 argument! Usage: %s [path]", argv[0]);
	}

	if (execlp("ls", "ls", argv[1], (char*)NULL) < 0) {
		err(2, "Error exectuing exec");
	}
}
