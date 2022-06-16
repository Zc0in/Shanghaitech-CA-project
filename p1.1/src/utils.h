#ifndef UTILS_H
#define UTILS_H

/* The function used to read the string in the file with space */
void myfscanf(FILE* input, char* target);
/* The function used to change binary int(in string) into int */
int bstoi(const char* string);
/* The function used to change decimal int into binary int(in string) */
void itobs(char* temp_offset, int n, int length);
/* The function used to get the substring */
void substring(const char* in_code, char* sub, int start, int end);
/* The function used to judge whether the register legal */
int rigister_judger(const char* in_code, int start);
/* The function used to put the immediate back into the right sequence and return its decimal int */
int put_together(const char* in_code, int flag);
/* The function used to classify the code */
int classify(const char* in_code);

#endif