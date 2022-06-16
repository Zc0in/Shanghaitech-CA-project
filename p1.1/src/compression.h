#ifndef COMPRESSION_H
#define COMPRESSION_H

/* The function used for compression */
void compress(char* in_code, int class, const int row, const int* offset_table);
/* THe function used for offset reset */
void change_offset(char* temp_offset, const int row, const int* offset_table, int final_length);

#endif