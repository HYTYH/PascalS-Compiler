//
// Created by chengmin on 2021/3/13
//

#include "errorHandle.h"
#include <iostream>

#define BUFFER_MAX_LENTH 2000
#define ERROR_MAX_LENTH 10000

using namespace std;

void initErrorHandle();
void closeErrorHandle();

void printRow();
void printError(char const *s, ...);

void beginToken(char *token);

int getNextChar(char *output, int maxBuffer);

int getNextLine();

static int eof = 0; // flag the eof
static int curRowNum = 0; // current row num startWith 1

static int curBufferLenth = 0; // current buffer lenth
static int bufferNext = 0; // next input chat index of buffer

static int tokenStart = 0;
static int tokenLenth = 0;
static int tokenNextStart = 0;

static char *buffer;

void initErrorHandle() {
    // malloc buffer
    buffer = (char *)malloc(BUFFER_MAX_LENTH);
}

void closeErrorHandle() {
    // free buffer
    free(buffer);
}

void printRow() {
    if (curRowNum == 0) {
        return;
    }
    fprintf(stdout, "%6d |%.*s", curRowNum, curBufferLenth, buffer);
    if (buffer[curBufferLenth-1] != '\n') {
        fprintf(stdout, "\n");
    }
}

void printError(char const *s, ...) {
    static char errorMessage[ERROR_MAX_LENTH];
    va_list args;

    int start = tokenStart;
    int end = start + tokenLenth - 1;
    int i;
    printRow();

    if (eof) {
        fprintf(stdout, "....... !");
        for (i = 0; i < curBufferLenth; i++) {
            fprintf(stdout, ".");
        }
        fprintf(stdout, "^-EOF\n");
    } else {
        fprintf(stdout, "...... !");
        for (i = 1; i < start; i++)
            fprintf(stdout, ".");
        for (i = start; i <= end; i++)
            fprintf(stdout, "^");
        for (i = end + 1; i < curBufferLenth; i++)
            fprintf(stdout, ".");
        fprintf(stdout, "   token from %d to %d\n", start, end);
    }

    va_start(args, s);
    vsprintf(errorMessage, s, args);
    va_end(args);

    fprintf(stdout, "Error: %s\n", errorMessage);
}

void beginToken(int tokenLength) {
    tokenStart = tokenNextStart;
    tokenLenth = tokenLength;
    tokenNextStart = bufferNext;
    
    yylloc.first_line = curRowNum;
    yylloc.first_column = tokenStart;
    yylloc.last_line = curRowNum;
    yylloc.last_column = tokenStart + tokenLenth - 1;
}

int getNextChar(char *output, int maxBuffer) {
    if (eof) {
        return 0;
    }
    while (bufferNext >= curBufferLenth) {
        if (getNextLine() != 0) {
            return 0;
        }
    }
    if (sizeof(output) < 1) {
        return 0;
    }
    output[0] = buffer[bufferNext];
    bufferNext += 1;
    return output[0] == 0 ? 0 : 1;
}

int getNextLine() {
    char *temp;

    bufferNext = 0;
    tokenStart = -1;
    tokenNextStart = 1;
    eof = 0;

    temp = fgets(buffer, BUFFER_MAX_LENTH, file);

    if (temp == NULL) {
        if (ferror(file)) {
            return -1; // means file error
        }
        eof = 1;
        return 1; // means file point the tail of file
    }

    curRowNum += 1;
    curBufferLenth = strlen(buffer);

    return 0;
}