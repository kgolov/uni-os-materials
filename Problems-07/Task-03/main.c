#include <unistd.h>
#include <err.h>

int main() {
	if (execlp("sleep", "sleep", "60", (char*)NULL) < 0) {
		err(1, "Error during exec");
	}
}
