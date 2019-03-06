#ifndef HOMEWORK_H
#define HOMEWORK_H

typedef struct {
    unsigned char red, green, blue, gray;
} pixel;

typedef struct {
    int width, height, maxval, num_colors;
    pixel** pixels;
} image;

void read_input(const char *fileName, image *img);
void write_data(const char *fileName, image *img);
void fill_borders(image *in, image *out);
void apply_filter(image *in, image *out, int process_height, float filter_matrix[3][3]);
void calculate_tasks(int tasks[], int N, int nProcesses);
void matrix_to_buffer(image *img, unsigned char *buffer, int start, int end);
void buffer_to_matrix(image *img, unsigned char *buffer, int start, int end);

#endif /* HOMEWORK_H */
