#ifndef UTILITY_H
#define UTILITY_H

#define MAX_TOKEN_LEN 100
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


typedef enum
{
    False =0,
    True,
    success,
    failure
}Status;


char *readFileContent(char *fname);
void lexicalAnalyzer(char *code);
Status isDelimeter(char ch);
int matchOperator(char *src,char *operator_token);
Status isKeyword(char *str);
int isNumericConstant(char *str);
#endif