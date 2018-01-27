#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Debugger file for background processes
int main(int argc, char* argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage: inf tag interval\n");
	} else {
		const char* tag = argv[1];
		int interval = atoi(argv[2]);
		while(1) {
			printf("%s\n", tag);
			sleep(interval);
		}
	}
}

