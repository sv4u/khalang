// Standard Khalang Prompt System

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static char input[2048];

int main (int argc, char** argv) {
	
	puts("Khalang");
	puts("#blessup");

	while (true) {

		fputs("lion:>>> ", stdout);

		fgets(input, 2048, stdin);

		printf("#blessed %s\n", input);

	}

	return 0;
}
