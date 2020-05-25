#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main (int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "Program expects a file as a parameter! Usage: %s [FILE]", argv[0]);
	}
	
	int fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		err(2, "Error opening file %s", argv[1]);
	}

	char* str1 = "foo\n";
	char* str2 = "bar\n";

	if (write(fd, str1, 2) <= 0) {
		int old_errno = errno;
		close(fd);
		errno = old_errno;
		err(3, "Could not write to file %s from parent process", argv[1]);
	}

	pid_t pid = fork();
	if (pid < 0) {
		int old_errno = errno;
		close(fd);
		errno = old_errno;
		err(4, "Error forking process");
	}
	else if (pid == 0) {
		if (write(fd, str2, 4) <= 0) {
			int old_errno = errno;
			close(fd);
			errno = old_errno;
			err(3, "Could not write to %s from child process", argv[1]);
		}

		close(fd);
		exit(0);
	}

	int status;
	if (wait(&status) < 0) {
		int old_errno = errno;
		close(fd);
		errno = old_errno;
		err(4, "Could not wait for child");
	}

	if (WEXITSTATUS(status) != 0) {
		err(5, "Child did not terminate normally, file is closed.");
	}

	if (write(fd, str1 + 2, 2) <= 0) {
		int old_errno = errno;
		close(fd);
		errno = old_errno;
		err(3, "Could not write to %s from parent process", argv[1]);
	}

	close(fd);
	exit(0);
}
