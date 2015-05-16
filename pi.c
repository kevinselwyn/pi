#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pi.h"

static int search(char *filename, char *input) {
	int rc = 0, index = -1;
	int *table = NULL;
	size_t filesize = 0, inputsize = 0;
	char *filedata = NULL, *input_clean = NULL;
	FILE *file = NULL;

	if (pi_read(&file, &filedata, &filesize, filename) != 0) {
		rc = 1;
		goto cleanup;
	}

	pi_clean(&input_clean, input);

	inputsize = strlen(input_clean);

	pi_table(&table, input, (int)inputsize);
	pi_search(&index, table, filedata, (int)filesize, input_clean, (int)inputsize);

	printf("%d\n", index);

cleanup:
	if (file) {
		(void)fclose(file);
	}

	if (filedata) {
		free(filedata);
	}

	if (input_clean) {
		free(input_clean);
	}

	return rc;
}

static int compress(char *filename, char *outputname) {
	int rc = 0, counter = 0;
	size_t input_filesize = 0;
	char *input_filename = NULL, *input_filedata = NULL;
	char *output_filename = NULL, *output_filedata = NULL;
	FILE *input = NULL, *output = NULL;

	input_filename = malloc(sizeof(char) * (int)strlen(filename) + 1);

	if (!input_filename) {
		printf("Memory error\n");

		rc = 1;
		goto cleanup;
	}

	strcpy(input_filename, filename);

	if (pi_read(&input, &input_filedata, &input_filesize, input_filename) != 0) {
		rc = 1;
		goto cleanup;
	}

	if (outputname) {
		output_filename = malloc(sizeof(char) * (int)strlen(outputname) + 1);
	} else {
		output_filename = malloc(sizeof(char) * (int)strlen(input_filename) + 10);
	}

	if (!output_filename) {
		printf("Memory error\n");

		rc = 1;
		goto cleanup;
	}

	if (outputname) {
		strcpy(output_filename, outputname);
	} else {
		strcpy(output_filename, input_filename);
		strcat(output_filename, ".compress");
	}

	pi_pack(&output_filedata, &counter, input_filedata, (int)input_filesize);

	if (pi_write(&output, output_filedata, (size_t)counter, output_filename) != 0) {
		rc = 1;
		goto cleanup;
	}

cleanup:
	if (input_filename) {
		free(input_filename);
	}

	if (output_filename) {
		free(output_filename);
	}

	if (input_filedata) {
		free(input_filedata);
	}

	if (output_filedata) {
		free(output_filedata);
	}

	return rc;
}

static void usage(char *exec) {
	int length = 0;

	length = (int)strlen(exec) + 7;

	printf("Usage: %s [-s,--search] <infile> <search>\n", exec);
	printf("%*s [-c,--compress] <infile> (<outfile>)\n", length, "");
	printf("%*s [-h,--help]\n", length, "");
}

int main(int argc, char *argv[]) {
	int rc = 0;
	char *exec = NULL, *action = NULL, *input = NULL;
	char *filename = NULL, *outputname = NULL;

	exec = argv[0];

	if (argc < 2) {
		usage(exec);

		rc = 1;
		goto cleanup;
	}

	action = argv[1];

	if (strcmp(action, "-h") == 0 || strcmp(action, "--help") == 0) {
		usage(exec);

		rc = 1;
		goto cleanup;
	} else if (strcmp(action, "-s") == 0 || strcmp(action, "--search") == 0) {
		if (argc < 4) {
			usage(exec);

			rc = 1;
			goto cleanup;
		}

		filename = argv[2];
		input = argv[3];

		if (search(filename, input) != 0) {
			rc = 1;
			goto cleanup;
		}
	} else if (strcmp(action, "-c") == 0 || strcmp(action, "--compress") == 0) {
		if (argc < 3) {
			usage(exec);

			rc = 1;
			goto cleanup;
		}

		filename = argv[2];
		outputname = argv[3];

		if (compress(filename, outputname) != 0) {
			rc = 1;
			goto cleanup;
		}
	} else {
		printf("Invalid action %s\n", action);

		rc = 1;
		goto cleanup;
	}

cleanup:
	return rc;
}