#ifndef HOMEWORK_H
#define HOMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct {
    unsigned char red, green, blue, gray;
} pixel;

typedef struct {
    int width, height, maxval, num_colors;
    pixel** pixels;
} image;

typedef struct {
    image *in, *out;
    int start, end;
} thread_s;

void readInput(const char * fileName, image *img);

void writeData(const char * fileName, image *img);

void resize(image *in, image * out);

#endif /* HOMEWORK_H */
