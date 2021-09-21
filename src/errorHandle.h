//
// Created by chengmin on 2021/3/13
//

#ifndef PASCALSCOMPILER_ERRORHANDLE_H
#define PASCALSCOMPILER_ERRORHANDLE_H

#include "AST.h"
#include "parser.h"
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *file;
extern int parseError;
extern YYLTYPE yylloc;

extern void initErrorHandle();
extern void closeErrorHandle();

extern void printError(char const *s, ...);

extern void beginToken(int tokenLength);

extern int getNextChar(char *output, int maxBuffer);

#endif