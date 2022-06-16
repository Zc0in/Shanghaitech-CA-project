#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// #include <algorithm>
#include <sys/time.h>
#include <time.h>
#include <immintrin.h>
//inplement dymanic

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define PI 3.14159

unsigned tmpX, tmpY;

typedef struct FVec
{
    unsigned int length;
    unsigned int min_length;
    unsigned int min_deta;
    float* data;
    float* sum;
} FVec;

typedef struct Image
{
    unsigned int dimX, dimY, numChannels;
    float* data;
} Image;

void normalize_FVec(FVec v)
{
    // float sum = 0.0;
    unsigned int i,j;
    int ext = v.length / 2;
    v.sum[0] = v.data[ext];
    
    for (i = ext+1,j=1; i < v.length; i++,j++)
    {
        v.sum[j] = v.sum[j-1] + v.data[i]*2;
    }
    // for (i = 0; i <= ext; i++)
    // {
    //      v.data[i] /= v.sum[v.length - ext - 1 ] ;
    //      printf("%lf ",v.sum[i]);
    // }
}

float* get_pixel(Image img, int x, int y)
{
    return img.data + img.numChannels * (y * img.dimX + x);
}

float gd(float a, float b, float x)
{
    float c = (x-b) / a;
    return exp((-.5) * c * c) / (a * sqrt(2 * PI));
}

FVec make_gv(float a, float x0, float x1, unsigned int length, unsigned int min_length)
{
    FVec v;
    v.length = length;
    v.min_length = min_length;
    if(v.min_length > v.length){
        v.min_deta = 0;
    }else{
        v.min_deta = ((v.length - v.min_length) / 2);
    }
    v.data = malloc(length * sizeof(float));
    v.sum = malloc((length / 2 + 1)* sizeof(float));
    float step = (x1 - x0) / ((float)length);
    int offset = length/2;

    for (int i = 0; i < length; i++)
    {
        v.data[i] = gd(a, 0.0f, (i-offset)*step);
    }
    normalize_FVec(v);
    return v;
}

void print_fvec(FVec v)
{
    unsigned int i;
    printf("\n");
    for (i = 0; i < v.length; i++)
    {
        printf("%f ", v.data[i]);
    }
    printf("\n");
}

Image img_T(Image a)
{
    Image b = a;
    b.data = malloc(b.dimX * b.dimY * b.numChannels * sizeof(float));
    b.dimX = a.dimY; b.dimY = a.dimX;
    return b;
}

Image img_extend(Image a, FVec gv)
{
    Image b = a;
    b.dimX += gv.length / 2 * 2;
    b.data = malloc(b.dimX * b.dimY * b.numChannels * sizeof(float));
    register int x;
    #pragma omp parallel for schedule(dynamic, 2)
    for (register int i = 0; i < b.dimY; i++)
    {
        for (x = 0; x < gv.length / 2 - 4; x+=4)
        {
            (b.data + b.numChannels * (i * b.dimX + x))[0] = (a.data + a.numChannels * (i * a.dimX))[0];
            (b.data + b.numChannels * (i * b.dimX + x))[1] = (a.data + a.numChannels * (i * a.dimX))[1];
            (b.data + b.numChannels * (i * b.dimX + x))[2] = (a.data + a.numChannels * (i * a.dimX))[2];

            (b.data + b.numChannels * (i * b.dimX + x + 1))[0] = (a.data + a.numChannels * (i * a.dimX))[0];
            (b.data + b.numChannels * (i * b.dimX + x + 1))[1] = (a.data + a.numChannels * (i * a.dimX))[1];
            (b.data + b.numChannels * (i * b.dimX + x + 1))[2] = (a.data + a.numChannels * (i * a.dimX))[2];

            (b.data + b.numChannels * (i * b.dimX + x + 2))[0] = (a.data + a.numChannels * (i * a.dimX))[0];
            (b.data + b.numChannels * (i * b.dimX + x + 2))[1] = (a.data + a.numChannels * (i * a.dimX))[1];
            (b.data + b.numChannels * (i * b.dimX + x + 2))[2] = (a.data + a.numChannels * (i * a.dimX))[2];

            (b.data + b.numChannels * (i * b.dimX + x + 3))[0] = (a.data + a.numChannels * (i * a.dimX))[0];
            (b.data + b.numChannels * (i * b.dimX + x + 3))[1] = (a.data + a.numChannels * (i * a.dimX))[1];
            (b.data + b.numChannels * (i * b.dimX + x + 3))[2] = (a.data + a.numChannels * (i * a.dimX))[2];
        }

        for (; x < gv.length / 2; x++)
        {
            (b.data + b.numChannels * (i * b.dimX + x))[0] = (a.data + a.numChannels * (i * a.dimX))[0];
            (b.data + b.numChannels * (i * b.dimX + x))[1] = (a.data + a.numChannels * (i * a.dimX))[1];
            (b.data + b.numChannels * (i * b.dimX + x))[2] = (a.data + a.numChannels * (i * a.dimX))[2];
        }

        for (x = gv.length / 2; x < a.dimX + gv.length / 2 - 4; x+=4)
        {
            (b.data + b.numChannels * (i * b.dimX + x))[0] = (a.data + a.numChannels * (i * a.dimX + x - gv.length / 2))[0];
            (b.data + b.numChannels * (i * b.dimX + x))[1] = (a.data + a.numChannels * (i * a.dimX + x - gv.length / 2))[1];
            (b.data + b.numChannels * (i * b.dimX + x))[2] = (a.data + a.numChannels * (i * a.dimX + x - gv.length / 2))[2];

            (b.data + b.numChannels * (i * b.dimX + x + 1))[0] = (a.data + a.numChannels * (i * a.dimX + x + 1 - gv.length / 2))[0];
            (b.data + b.numChannels * (i * b.dimX + x + 1))[1] = (a.data + a.numChannels * (i * a.dimX + x + 1 - gv.length / 2))[1];
            (b.data + b.numChannels * (i * b.dimX + x + 1))[2] = (a.data + a.numChannels * (i * a.dimX + x + 1 - gv.length / 2))[2];

            (b.data + b.numChannels * (i * b.dimX + x + 2))[0] = (a.data + a.numChannels * (i * a.dimX + x + 2 - gv.length / 2))[0];
            (b.data + b.numChannels * (i * b.dimX + x + 2))[1] = (a.data + a.numChannels * (i * a.dimX + x + 2 - gv.length / 2))[1];
            (b.data + b.numChannels * (i * b.dimX + x + 2))[2] = (a.data + a.numChannels * (i * a.dimX + x + 2 - gv.length / 2))[2];

            (b.data + b.numChannels * (i * b.dimX + x + 3))[0] = (a.data + a.numChannels * (i * a.dimX + x + 3 - gv.length / 2))[0];
            (b.data + b.numChannels * (i * b.dimX + x + 3))[1] = (a.data + a.numChannels * (i * a.dimX + x + 3 - gv.length / 2))[1];
            (b.data + b.numChannels * (i * b.dimX + x + 3))[2] = (a.data + a.numChannels * (i * a.dimX + x + 3 - gv.length / 2))[2];
        }

        for (; x < a.dimX + gv.length / 2; x++)
        {
            (b.data + b.numChannels * (i * b.dimX + x))[0] = (a.data + a.numChannels * (i * a.dimX + x - gv.length / 2))[0];
            (b.data + b.numChannels * (i * b.dimX + x))[1] = (a.data + a.numChannels * (i * a.dimX + x - gv.length / 2))[1];
            (b.data + b.numChannels * (i * b.dimX + x))[2] = (a.data + a.numChannels * (i * a.dimX + x - gv.length / 2))[2];
        }

        for (int x = a.dimX + gv.length / 2; x < a.dimX + gv.length / 2 * 2 - 4; x+=4)
        {
            (b.data + b.numChannels * (i * b.dimX + x))[0] = (a.data + a.numChannels * (i * a.dimX + a.dimX - 1))[0];
            (b.data + b.numChannels * (i * b.dimX + x))[1] = (a.data + a.numChannels * (i * a.dimX + a.dimX - 1))[1];
            (b.data + b.numChannels * (i * b.dimX + x))[2] = (a.data + a.numChannels * (i * a.dimX + a.dimX - 1))[2];

            (b.data + b.numChannels * (i * b.dimX + x + 1))[0] = (a.data + a.numChannels * (i * a.dimX + a.dimX - 1))[0];
            (b.data + b.numChannels * (i * b.dimX + x + 1))[1] = (a.data + a.numChannels * (i * a.dimX + a.dimX - 1))[1];
            (b.data + b.numChannels * (i * b.dimX + x + 1))[2] = (a.data + a.numChannels * (i * a.dimX + a.dimX - 1))[2];

            (b.data + b.numChannels * (i * b.dimX + x + 2))[0] = (a.data + a.numChannels * (i * a.dimX + a.dimX - 1))[0];
            (b.data + b.numChannels * (i * b.dimX + x + 2))[1] = (a.data + a.numChannels * (i * a.dimX + a.dimX - 1))[1];
            (b.data + b.numChannels * (i * b.dimX + x + 2))[2] = (a.data + a.numChannels * (i * a.dimX + a.dimX - 1))[2];

            (b.data + b.numChannels * (i * b.dimX + x + 3))[0] = (a.data + a.numChannels * (i * a.dimX + a.dimX - 1))[0];
            (b.data + b.numChannels * (i * b.dimX + x + 3))[1] = (a.data + a.numChannels * (i * a.dimX + a.dimX - 1))[1];
            (b.data + b.numChannels * (i * b.dimX + x + 3))[2] = (a.data + a.numChannels * (i * a.dimX + a.dimX - 1))[2];
        }

        for (; x < a.dimX + gv.length / 2 * 2; x++)
        {
            (b.data + b.numChannels * (i * b.dimX + x))[0] = (a.data + a.numChannels * (i * a.dimX + a.dimX - 1))[0];
            (b.data + b.numChannels * (i * b.dimX + x))[1] = (a.data + a.numChannels * (i * a.dimX + a.dimX - 1))[1];
            (b.data + b.numChannels * (i * b.dimX + x))[2] = (a.data + a.numChannels * (i * a.dimX + a.dimX - 1))[2];
        }

    }
    return b;
}

Image gb(Image a, FVec gv)
{
    Image c = img_extend(a, gv);    
    Image b = img_T(a);  
    int ext = gv.length / 2;

    #pragma omp parallel for schedule(dynamic, 2) 
    for (register unsigned int y = 0; y < a.dimY; y += 1)
    {
        for (register unsigned int x = 0; x < a.dimX; x += 1)
        {
            __m128 pixel_sum_0 = _mm_setzero_ps();
            __m128 pixel_sum_1 = _mm_setzero_ps();
            __m128 pixel_sum_2 = _mm_setzero_ps();
            __m128 pixel_sum_3 = _mm_setzero_ps();
            __m128 pixel_sum_4 = _mm_setzero_ps();
            __m128 pixel_sum_5 = _mm_setzero_ps();
            __m128 pixel_sum_6 = _mm_setzero_ps();
            __m128 pixel_sum_7 = _mm_setzero_ps();
            __m128 pixel_0, sum_iter_0, pixel_1, sum_iter_1, pixel_2, sum_iter_2;
            __m128 pixel_3, sum_iter_3, pixel_4, sum_iter_4, pixel_5, sum_iter_5;
            __m128 pixel_6, sum_iter_6, pixel_7, sum_iter_7, pixel_t, sum_iter_t;

            float tmp_sum_0[4];

            float * pc = get_pixel(b, y, x);
            unsigned int deta = fmin(fmin(a.dimY - y - 1, y),fmin(a.dimX - x - 1, x));
            deta = fmin(deta, gv.min_deta);
            register int i;
            float* temp;
            for (i = deta; i < gv.length - deta - 8; i += 8)
            {
                temp = get_pixel(c, x + i, y);
                pixel_0 = _mm_loadu_ps(temp);
                pixel_1 = _mm_loadu_ps(temp + 3);
                pixel_2 = _mm_loadu_ps(temp + 6);
                pixel_3 = _mm_loadu_ps(temp + 9);
                pixel_4 = _mm_loadu_ps(temp + 12);
                pixel_5 = _mm_loadu_ps(temp + 15);
                pixel_6 = _mm_loadu_ps(temp + 18);
                pixel_7 = _mm_loadu_ps(temp + 21);
                
                sum_iter_0 = _mm_load1_ps(gv.data + i);
                sum_iter_1 = _mm_load1_ps(gv.data + i + 1);
                sum_iter_2 = _mm_load1_ps(gv.data + i + 2);
                sum_iter_3 = _mm_load1_ps(gv.data + i + 3);
                sum_iter_4 = _mm_load1_ps(gv.data + i + 4);
                sum_iter_5 = _mm_load1_ps(gv.data + i + 5);
                sum_iter_6 = _mm_load1_ps(gv.data + i + 6);
                sum_iter_7 = _mm_load1_ps(gv.data + i + 7);

                pixel_sum_0 = _mm_add_ps(pixel_sum_0, _mm_mul_ps(sum_iter_0, pixel_0));
                pixel_sum_1 = _mm_add_ps(pixel_sum_1, _mm_mul_ps(sum_iter_1, pixel_1));
                pixel_sum_2 = _mm_add_ps(pixel_sum_2, _mm_mul_ps(sum_iter_2, pixel_2));
                pixel_sum_3 = _mm_add_ps(pixel_sum_3, _mm_mul_ps(sum_iter_3, pixel_3));    
                pixel_sum_4 = _mm_add_ps(pixel_sum_4, _mm_mul_ps(sum_iter_4, pixel_4));
                pixel_sum_5 = _mm_add_ps(pixel_sum_5, _mm_mul_ps(sum_iter_5, pixel_5));
                pixel_sum_6 = _mm_add_ps(pixel_sum_6, _mm_mul_ps(sum_iter_6, pixel_6));
                pixel_sum_7 = _mm_add_ps(pixel_sum_7, _mm_mul_ps(sum_iter_7, pixel_7)); 
            }
            for (; i < gv.length - deta; i++)
            {
                pixel_t = _mm_loadu_ps(get_pixel(c, x + i, y));
                sum_iter_t = _mm_load1_ps(gv.data + i);
                pixel_sum_0 = _mm_add_ps(pixel_sum_0, _mm_mul_ps(pixel_t, sum_iter_t));
            }   

            pixel_sum_0 = _mm_add_ps(_mm_add_ps(pixel_sum_0, pixel_sum_1), _mm_add_ps(pixel_sum_2,pixel_sum_3));
            pixel_sum_1 = _mm_add_ps(_mm_add_ps(pixel_sum_4, pixel_sum_5), _mm_add_ps(pixel_sum_6,pixel_sum_7));
            pixel_sum_0 = _mm_add_ps(pixel_sum_0, pixel_sum_1);


            
            _mm_storeu_ps(tmp_sum_0, _mm_div_ps(pixel_sum_0, _mm_load_ps1(&(gv.sum[ext - deta]))));

            pc[0] = tmp_sum_0[0];
            pc[1] = tmp_sum_0[1];
            pc[2] = tmp_sum_0[2];
        }
    }
    free(c.data);
    return b;
}



Image apply_gb(Image a, FVec gv)
{
    Image b = gb(a, gv);
    Image c = gb(b, gv);
    free(b.data);
    return c;
}

int main(int argc, char** argv)
{
    struct timeval start_time, stop_time, elapsed_time; 
    gettimeofday(&start_time,NULL);
    if (argc < 6)
    {
        printf("Usage: ./gb.exe <inputjpg> <outputname> <float: a> <float: x0> <float: x1> <unsigned int: dim>\n");
        exit(0);
    }

    float a, x0, x1;
    unsigned int dim, min_dim;

    sscanf(argv[3], "%f", &a);
    sscanf(argv[4], "%f", &x0);
    sscanf(argv[5], "%f", &x1);
    sscanf(argv[6], "%u", &dim);
    sscanf(argv[7], "%u", &min_dim);

    FVec v = make_gv(a, x0, x1, dim, min_dim);
    // print_fvec(v);
    Image img;
    img.data = stbi_loadf(argv[1], &(img.dimX), &(img.dimY), &(img.numChannels), 0);

    Image imgOut = apply_gb(img, v);
    stbi_write_jpg(argv[2], imgOut.dimX, imgOut.dimY, imgOut.numChannels, imgOut.data, 90);
    gettimeofday(&stop_time,NULL);
    timersub(&stop_time, &start_time, &elapsed_time); 
    printf("%f \n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);
    free(imgOut.data);
    free(v.data);
    free(v.sum);
    return 0;
}

