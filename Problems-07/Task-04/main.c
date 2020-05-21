#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>

int main() {
	pid_t pid = fork();
	
	if (pid < 0) {
		err(1, "Error forking process");
	}
	else if (pid == 0) {
		// Child process
		for (int i = 0; i < 30; i++) {	
			printf("Child\n");
		}
	}
	else {
		// Parent
		for (int i = 0; i < 30; i++) {
			printf("Parent\n");
		}
	}

	exit(0);
}
