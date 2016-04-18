/* A dead simple implementation of my own cat program. */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	char buf[100];
	size_t len;
	char *file_mode;
	int i;
	int num_files;
	FILE *inputfile;

	for (i = 1; i < argc; i++) {
		inputfile = fopen(argv[i], "r");

		while ((len = fread(&buf[0], 1, sizeof(buf), inputfile)) > 0) {
			fwrite(&buf[0], 1, len, stdout);
		}
		fclose(inputfile);
	}

	return 0;
}
