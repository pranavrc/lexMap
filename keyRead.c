#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUFSIZE 1024

typedef struct {
	char *keyString;
	char *valueString;
} keyPairs;

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

keyPairs keySplit(char *oKey)
{
	keyPairs ret;

	ret.keyString = oKey;

	strtok_r(oKey, ":", &ret.valueString);

	return ret;
}

char *replace_string(const char *str, const char *old, const char *new)
{
	char *ret;
	int i, count = 0;
	size_t newlen = strlen(new);
	size_t oldlen = strlen(old);

	for (i = 0; str[i] != '\0'; i++) {
		if (strstr(&str[i], old) == &str[i]) {
			count++;
			i += oldlen - 1;
		}
	}

	ret = malloc(i + count * (newlen - oldlen));
	if (ret == NULL) exit(EXIT_FAILURE);

	i = 0;
	while (*str) {
		if (strstr(str, old) == str) {
			strcpy(&ret[i], new);
			i += newlen;
			str += oldlen;
		} else ret[i++] = *str++;
	}
	ret[i] = '\0';

	return ret;
}

char *targetFile_read(char *targetFile)
{
	char *fileContent;
	long fileSize;

	FILE *fptr;
	fptr = fopen(targetFile, "r");

	if (!fptr) terminate("Target file not found.");

	fseek(fptr, 0L, SEEK_END);
	fileSize = ftell(fptr);
	fseek(fptr, 0L, SEEK_SET);

	fileContent = malloc(fileSize * sizeof(char));

	fread(fileContent, 1, fileSize, fptr);
	fclose(fptr);

	return fileContent;
}

int main(int argc, char *argv[])
{
	char *t = targetFile_read("target.txt");
	printf("Target file contents: \n%s\n", t);
	int i;
	int n = countLines("key.txt");
	keyPairs foo[n];
	char **array = keyFile_parse("key.txt", n);
	for (i = 0; i < n; i++) {
		foo[i] = keySplit(array[i]);
		t = replace_string(t, foo[i].keyString, foo[i].valueString);
		printf("Replaced:\n%s\n", t);
		free(array[i]);
	}
	free(array);
	free(t);

	return 0;
}
