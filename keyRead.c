#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUFSIZE 1024

void terminate(const char *message)
{
	if (errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}

	exit(1);
}

int countLines(char *keyFile)
{
	int lineCount = 0, charStream;
	FILE *fptr;
	fptr = fopen(keyFile, "r");
	fseek(fptr, 0L, SEEK_SET);

	if (!fptr) terminate("Key file not found.");

	do {
		charStream = fgetc(fptr);
		if (charStream == '\n')
			lineCount++;
	} while (charStream != EOF);
	fclose(fptr);
	
	return lineCount;
}

char **keyFile_parse(char *keyFile, int lineCount)
{
	char lineBuffer[BUFSIZE];
	FILE *fptr;
	fptr = fopen(keyFile, "r");
	fseek(fptr, 0L, SEEK_SET);
	int bufLen;

	if (!fptr) terminate("Key file not found.");
	char **contentArray;
	int lineTrans = 0;

	contentArray = malloc(lineCount * sizeof(char*));

	while (fgets(lineBuffer, sizeof(lineBuffer), fptr)) {
		lineTrans++;
		bufLen = strlen(lineBuffer);
		if (lineBuffer[bufLen - 1] == '\n') lineBuffer[bufLen - 1] = '\0';
		contentArray[lineTrans - 1] = strdup(lineBuffer);
	}

	fclose(fptr);

	return contentArray;
}

void keySplit(char *oKey)
{
	char *eKey;

	strtok_r(oKey, ":", &eKey);

	printf("%s\n%s\n", oKey, eKey);
}

int main(int argc, char *argv[])
{
	int i;
	int n = countLines("Foo.txt");
	char **array = keyFile_parse("Foo.txt", n);
	for (i = 0; i < n; i++) {
		printf("%s\n", array[i]);
		keySplit(array[i]);
		free(array[i]);
	}
	free(array);
	return 0;
}
