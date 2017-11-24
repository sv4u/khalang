// Khalang

#include "mpc.h"

static char buffer[2048];

char *readline (char *prompt) {
	fputs(prompt, stdout);
	fgets(buffer, 2048, stdin);
	char *copy = malloc(strlen(buffer) + 1);
	strcpy(copy, buffer);
	copy[strlen(copy) - 1] = '\0';
	return copy;
}

void add_history(char *empty) {}

int main(int argc, char **argv) {
	return 0;
}
