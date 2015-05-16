#ifndef PI_H
#define PI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int pi_read(FILE **file, char **filedata, size_t *filesize, char *filename) {
	int rc = 0;
	char *start = NULL;

	*file = fopen(filename, "rb");

	if (!*file) {
		printf("Could not open %s\n", filename);

		rc = 1;
		goto cleanup;
	}

	(void)fseek(*file, 0, SEEK_END);
	*filesize = (size_t)ftell(*file);
	(void)fseek(*file, 0, SEEK_SET);

	if (*filesize == 0) {
		printf("%s is empty\n", filename);

		rc = 1;
		goto cleanup;
	}

	*filedata = malloc(sizeof(char) * *filesize + 1);

	if (!*filedata) {
		printf("Memory error\n");

		rc = 1;
		goto cleanup;
	}

	start = malloc(sizeof(char) * 3);

	if (fread(start, 1, 2, *file) != 2) {
		printf("Could not read %s\n", filename);

		rc = 1;
		goto cleanup;
	}

	if (strncmp(start, "3.", 2) == 0) {
		*filesize -= 2;
	} else {
		(void)fseek(*file, 0, SEEK_SET);
	}

	if (fread(*filedata, 1, *filesize, *file) != *filesize) {
		printf("Could not read %s\n", filename);

		rc = 1;
		goto cleanup;
	}

cleanup:
	if (start) {
		free(start);
	}

	return rc;
}

static int pi_write(FILE **file, char *filedata, size_t filesize, char *filename) {
	int rc = 0;

	*file = fopen(filename, "wb");

	if (!*file) {
		printf("Could not create %s\n", filename);

		rc = 1;
		goto cleanup;
	}

	if (fwrite(filedata, 1, filesize, *file) != filesize) {
		printf("Could not write %s\n", filename);

		rc = 1;
		goto cleanup;
	}

cleanup:
	return rc;
}

static void pi_pack(char **output, int *output_size, char *input, int input_size) {
	int byte = 0, size = 0, i = 0, l = 0;
	char *out = NULL;

	out = malloc(sizeof(char) * ceil((double)(input_size / 2)) + 1);

	for (i = 0, l = input_size; i < l; i += 2) {
		byte = ((int)input[i] - 48) << 4;
		byte += (int)input[i + 1] - 48;

		out[size++] = (char)byte;
	}

	*output = out;
	*output_size = size;
}

static void pi_clean(char **clean, char *dirty) {
	int count = 0, i = 0, l = 0;
	size_t length = 0;

	length = strlen(dirty);
	*clean = malloc(sizeof(char) * length + 1);

	for (i = 0, l = (int)length; i < l; i++) {
		if (dirty[i] >= '0' && dirty[i] <= '9') {
			strncat(*clean, dirty + i, 1);
			count++;
		}
	}

	printf("%s\n", *clean);
}

static void pi_table(int **table, char *input, int size) {
	int c = 0, p = 0;
	int *t = NULL;

	t = malloc(sizeof(int) * size);

	t[0] = -1;
	t[1] = 0;

	p = 2;

	while (p < size) {
		if (input[p - 1] == input[p]) {
			c++;
			t[p] = c;
			p++;
		} else if (c > 0) {
			c = t[c];
		} else {
			t[p] = 0;
			p++;
		}
	}

	*table = t;
}

static void pi_search(int *index, int *table, char *filedata, int filesize, char *input, int inputsize) {
	int byte = 0, pos = -1, i = 0, m = 0;

	while (m + i < filesize * 2) {
		byte = (int)filedata[(int)floor((double)((m + i) / 2))];

		if ((m + i) % 2 == 0) {
			byte >>= 4;
		}

		byte &= 0x0f;

		if ((int)input[i] - 48 == byte) {
			if (i == inputsize - 1) {
				pos = m;
				break;
			}

			i++;
		} else {
			if (table[i] > -1) {
				m += i - table[i];
				i = table[i];
			} else {
				i = 0;
				m++;
			}
		}
	}

	*index = pos;
}

#endif /* PI_H */