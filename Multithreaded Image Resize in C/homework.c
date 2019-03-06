#include "homework.h"

int num_threads;
int resize_factor;

// Citeste fisierul de input si aloca memorie pentru imaginea de input
void readInput(const char * fileName, image *img) {
    int i, j, k;
    FILE *input_file = fopen(fileName, "r");
    char *buffer = (char *)malloc(sizeof(char) * 20);

    fgets(buffer, 20, input_file);
    if (strncmp(buffer, "P5", 2) == 0) {
        img->num_colors = 1;
    } else {
        img->num_colors = 3;
    }

    fgets(buffer, 20, input_file);
    img->width = atoi(strtok(buffer, " "));
    img->height = atoi(strtok(NULL, "\n"));

    fgets(buffer, 20, input_file);
    img->maxval = atoi(strtok(buffer, "\n"));

    img->pixels = (pixel**) malloc(sizeof(pixel*) * img->height);
    for (i = 0; i < img->height; i++) {
        img->pixels[i] = (pixel*) malloc(sizeof(pixel) * img->width);
    }

    buffer = realloc(buffer, img->width * img->num_colors);
    for (i = 0; i < img->height; i++) {
        fread(buffer, img->num_colors, img->width, input_file);
        if (img->num_colors == 3) {
            k = 0;
            for (j = 0; j < img->width; j++) {
                img->pixels[i][j].red = buffer[k++];
                img->pixels[i][j].green = buffer[k++];
                img->pixels[i][j].blue = buffer[k++];
            }
        } else if (img->num_colors == 1) {
            for (j = 0; j < img->width; j++) {
                img->pixels[i][j].gray = buffer[j];
            }
        }
    }

    free(buffer);
    fclose(input_file);
}

// Scrie imaginea de output
void writeData(const char *fileName, image *img) {
    int i, j;
    FILE *output_file = fopen(fileName, "w");

    if (img->num_colors == 1) {
        fprintf(output_file, "P5\n");
    } else {
        fprintf(output_file, "P6\n");
    }
    fprintf(output_file, "%d %d\n%d\n", img->width, img->height, img->maxval);
    
    for (i = 0; i < img->height; i++) {
        for (j = 0; j < img->width; j++) {
            if (img->num_colors == 1) {
                fwrite(&(img->pixels[i][j].gray), 1, 1, output_file);
            } else if (img->num_colors == 3) {
                fwrite(&(img->pixels[i][j]), sizeof(unsigned char), 3, output_file);
            }
        }
    }

    for (i = 0; i < img->height; i++) {
        free(img->pixels[i]);
    }
    free(img->pixels);
    fclose(output_file);
}

// Functia accesta de fiecare thread
void* resize_function(void* var) {
    thread_s *thread = (thread_s*)var;

    int i, j, k, l;
    int gray, red, green, blue;
    int squared_resize_factor = resize_factor * resize_factor;
    int gaussian_kernel[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
    int kernel_x, kernel_y;

    if (resize_factor % 2 == 0) {
        if (thread->out->num_colors == 1) {
            for (i = thread->start; i <= thread->end; i++) {
                for (j = 0; j < thread->out->width; j++) {
                    gray = 0;
                    for (k = i * resize_factor; k < i * resize_factor + resize_factor; k++) {
                        for (l = j * resize_factor; l < j * resize_factor + resize_factor; l++) {
                            gray += thread->in->pixels[k][l].gray;
                        }
                    }
                    thread->out->pixels[i][j].gray = gray / squared_resize_factor;
                }
            }
        } else {
            for (i = thread->start; i <= thread->end; i++) {
                for (j = 0; j < thread->out->width; j++) {
                    red = green = blue = 0;
                    for (k = i * resize_factor; k < i * resize_factor + resize_factor; k++) {
                        for (l = j * resize_factor; l < j * resize_factor + resize_factor; l++) {
                            red += thread->in->pixels[k][l].red;
                            green += thread->in->pixels[k][l].green;
                            blue += thread->in->pixels[k][l].blue;
                        }
                    }
                    thread->out->pixels[i][j].red = red / squared_resize_factor;
                    thread->out->pixels[i][j].green = green / squared_resize_factor;
                    thread->out->pixels[i][j].blue = blue / squared_resize_factor;
                }
            }
        }
    } else if (resize_factor == 3) {
        if (thread->out->num_colors == 1) {
            for (i = thread->start; i <= thread->end; i++) {
                for (j = 0; j < thread->out->width; j++) {
                    gray = 0;
                    for (k = i * resize_factor; k < i * resize_factor + resize_factor; k++) {
                        kernel_x = k % 3;
                        for (l = j * resize_factor; l < j * resize_factor + resize_factor; l++) {
                            kernel_y = l % 3;
                            gray += thread->in->pixels[k][l].gray * gaussian_kernel[kernel_x][kernel_y];
                        }
                    }
                    thread->out->pixels[i][j].gray = gray / 16;
                }
            }
        } else {
            for (i = thread->start; i <= thread->end; i++) {
                for (j = 0; j < thread->out->width; j++) {
                    red = green = blue = 0;
                    for (k = i * resize_factor; k < i * resize_factor + resize_factor; k++) {
                        kernel_x = k % 3;
                        for (l = j * resize_factor; l < j * resize_factor + resize_factor; l++) {
                            kernel_y = l % 3;
                            red += thread->in->pixels[k][l].red * gaussian_kernel[kernel_x][kernel_y];
                            green += thread->in->pixels[k][l].green * gaussian_kernel[kernel_x][kernel_y];
                            blue += thread->in->pixels[k][l].blue * gaussian_kernel[kernel_x][kernel_y];
                        }
                    }
                    thread->out->pixels[i][j].red = red / 16;
                    thread->out->pixels[i][j].green = green / 16;
                    thread->out->pixels[i][j].blue = blue / 16;
                }
            }
        }
    }

    return NULL;
}

void resize(image *in, image *out) {
    pthread_t tid[num_threads];
    thread_s threads[num_threads];
    int i, tasks[num_threads];

    out->height = in->height / resize_factor;
    out->width = in->width / resize_factor;
    out->maxval = in->maxval;
    out->num_colors = in->num_colors;
    out->pixels = (pixel**) malloc(sizeof(pixel*) * out->height);
    for (i = 0; i < out->height; i++) {
        out->pixels[i] = (pixel*) malloc(sizeof(pixel) * out->width);
    }

    // Calculeaza vectorul ce contine capetele intervalelor atribuite
    // thread-urilor
    for (i = 0; i < num_threads; i++) {
        tasks[i] = 0;
    }

    for (i = 0; i < out->height % num_threads; i++) {
        tasks[i] += out->height / num_threads + 1;
    }

    for (i = out->height % num_threads; i < num_threads; i++) {
        tasks[i] += out->height / num_threads;
    }

    for (i = 1; i < num_threads; i++) {
        tasks[i] = tasks[i-1] + tasks[i];
    }
    
    for (i = 0; i < num_threads; i++) {
        tasks[i]--;
    }

	for (i = 0; i < num_threads; i++) {
        threads[i].in = in;
        threads[i].out = out;
        threads[i].start = (i == 0 ? 0 : tasks[i - 1] + 1);
        threads[i].end = tasks[i];
	}

    // Creeaza si porneste thread-urile
	for (i = 0; i < num_threads; i++) {
		pthread_create(&(tid[i]), NULL, resize_function, &(threads[i]));
	}

	for (i = 0; i < num_threads; i++) {
		pthread_join(tid[i], NULL);
	}

    for (i = 0; i < in->height; i++) {
        free(in->pixels[i]);
    }
    free(in->pixels);
}
