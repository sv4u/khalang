// Unix Prompt System

#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>
#include <editline/history.h>

int main(int argc, char** argv) {

	puts("Khalang");
	puts("#blessup");

	while (true) {

		char* input = readline("lion:>>> ");
		add_history(input);

		printf("#blessed %s\n", input);

		free(input);

	}

	return 0;

}
