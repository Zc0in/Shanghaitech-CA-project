#include<stdio.h>
#include<stdint.h>
#include<time.h>

#define size(x) (printf("Size of %s: %u\n", #x, sizeof(x)))

int main()
{
    size(char);
    size(short);
    size(short int);
    size(int);
    size(long int);
    size(unsigned int);
    size(void*);
    size(size_t);
    size(float);
    size(double);
    size(int8_t);
    size(int16_t);
    size(int32_t);
    size(int64_t);
    size(time_t);
    size(clock_t);
    size(struct tm);
    size(NULL);
    return 0;
}
