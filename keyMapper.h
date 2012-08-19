#ifndef KEYMAPPER_H
#define KEYMAPPER_H

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

void terminate(const char *message);
int countLines(char *keyFile);
char **keyFile_parse(char *keyFile, int lineCount);
keyPairs keySplit(char *oKey, char *deLim);
char *replace_string(const char *str, const char *old, const char *new);
char *targetFile_read(char *targetFile);
void *targetFile_write(char *targetFile, char *str);
void lex(char *targetFile, char *keyFile, char *mapped);

#endif
