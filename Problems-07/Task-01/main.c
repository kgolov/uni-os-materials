#include <unistd.h>
#include <err.h>

int main() {
	if(execlp("date", "date", (char*)NULL) < 0) {
		err(1, "Error executing date");
	}
}
