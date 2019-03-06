#include "homework.h"
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#define MIN(a, b) ((a < b) ? a : b)

// Citeste fisierul de input si aloca memorie pentru imaginea de input
void read_input(const char *fileName, image *img) {
    int i, j, k;
    FILE *input_file = fopen(fileName, "r");
    char *buffer = (char*) malloc(sizeof(char) * 20);

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
    for (i = 0; i < img->height; ++i) {
        img->pixels[i] = (pixel*) malloc(sizeof(pixel) * img->width);
    }

    buffer = realloc(buffer, img->width * img->num_colors);
    for (i = 0; i < img->height; ++i) {
        fread(buffer, img->num_colors, img->width, input_file);
        if (img->num_colors == 3) {
            k = 0;
            for (j = 0; j < img->width; ++j) {
                img->pixels[i][j].red = buffer[k++];
                img->pixels[i][j].green = buffer[k++];
                img->pixels[i][j].blue = buffer[k++];
            }
        } else if (img->num_colors == 1) {
            for (j = 0; j < img->width; ++j) {
                img->pixels[i][j].gray = buffer[j];
            }
        }
    }

    free(buffer);
    fclose(input_file);
}

// Scrie imaginea de output
void write_data(const char *fileName, image *img) {
    int i, j;
    FILE *output_file = fopen(fileName, "w");

    if (img->num_colors == 1) {
        fprintf(output_file, "P5\n");
    } else {
        fprintf(output_file, "P6\n");
    }
    fprintf(output_file, "%d %d\n%d\n", img->width, img->height, img->maxval);
    
    for (i = 0; i < img->height; ++i) {
        for (j = 0; j < img->width; ++j) {
            if (img->num_colors == 1) {
                fwrite(&(img->pixels[i][j].gray), 1, 1, output_file);
            } else if (img->num_colors == 3) {
                fwrite(&(img->pixels[i][j]), sizeof(unsigned char), 3, output_file);
            }
        }
    }

    fclose(output_file);
}

// Copiaza pixelii de pe margini din imaginea de input in cea de output
void fill_borders(image *in, image *out) {
    int i, j;

    if (in->num_colors == 1) {
        for (i = 0; i < in->height; ++i) {
            out->pixels[i][0].gray = in->pixels[i][0].gray;
            out->pixels[i][out->width - 1].gray = in->pixels[i][in->width - 1].gray;
        }

        for (j = 0; j < in->width; ++j) {
            out->pixels[0][j].gray = in->pixels[0][j].gray;
            out->pixels[out->height - 1][j].gray = in->pixels[in->height - 1][j].gray;
        }
    } else {
        for (i = 0; i < in->height; ++i) {
            out->pixels[i][0].red = in->pixels[i][0].red;
            out->pixels[i][out->width - 1].red = in->pixels[i][in->width - 1].red;
            
            out->pixels[i][0].green = in->pixels[i][0].green;
            out->pixels[i][out->width - 1].green = in->pixels[i][in->width - 1].green;
            
            out->pixels[i][0].blue = in->pixels[i][0].blue;
            out->pixels[i][out->width - 1].blue = in->pixels[i][in->width - 1].blue;
        }

        for (j = 0; j < in->width; ++j) {
            out->pixels[0][j].red = in->pixels[0][j].red;
            out->pixels[out->height - 1][j].red = in->pixels[in->height - 1][j].red;
            
            out->pixels[0][j].green = in->pixels[0][j].green;
            out->pixels[out->height - 1][j].green = in->pixels[in->height - 1][j].green;
            
            out->pixels[0][j].blue = in->pixels[0][j].blue;
            out->pixels[out->height - 1][j].blue = in->pixels[in->height - 1][j].blue;
        }
    }
}

// Aplica un filtru pe o imagine
void apply_filter(image *in, image *out, int final_row, float filter_matrix[3][3]) {
    int i, j;

    if (in->num_colors == 1) {
        for (i = 1; i <= final_row; ++i) {
            for (j = 1; j < in->width - 1; ++j) {
                out->pixels[i][j].gray = in->pixels[i-1][j-1].gray * filter_matrix[0][0] + 
                                        in->pixels[i-1][j].gray * filter_matrix[0][1] + 
                                        in->pixels[i-1][j+1].gray * filter_matrix[0][2] + 
                                        in->pixels[i][j-1].gray * filter_matrix[1][0] + 
                                        in->pixels[i][j].gray * filter_matrix[1][1] + 
                                        in->pixels[i][j+1].gray * filter_matrix[1][2] + 
                                        in->pixels[i+1][j-1].gray * filter_matrix[2][0] + 
                                        in->pixels[i+1][j].gray * filter_matrix[2][1] + 
                                        in->pixels[i+1][j+1].gray * filter_matrix[2][2];
            }
        }
        
        for (i = 1; i <= final_row; ++i) {
            for (j = 1; j < in->width - 1; ++j) {
                in->pixels[i][j].gray = out->pixels[i][j].gray;
            }
        }   
    } else {
        for (i = 1; i <= final_row; ++i) {
            for (j = 1; j < in->width - 1; ++j) {
                out->pixels[i][j].red = in->pixels[i-1][j-1].red * filter_matrix[0][0] + 
                                        in->pixels[i-1][j].red * filter_matrix[0][1] + 
                                        in->pixels[i-1][j+1].red * filter_matrix[0][2] + 
                                        in->pixels[i][j-1].red * filter_matrix[1][0] + 
                                        in->pixels[i][j].red * filter_matrix[1][1] + 
                                        in->pixels[i][j+1].red * filter_matrix[1][2] + 
                                        in->pixels[i+1][j-1].red * filter_matrix[2][0] + 
                                        in->pixels[i+1][j].red * filter_matrix[2][1] + 
                                        in->pixels[i+1][j+1].red * filter_matrix[2][2];

                out->pixels[i][j].green = in->pixels[i-1][j-1].green * filter_matrix[0][0] + 
                                            in->pixels[i-1][j].green * filter_matrix[0][1] + 
                                            in->pixels[i-1][j+1].green * filter_matrix[0][2] + 
                                            in->pixels[i][j-1].green * filter_matrix[1][0] + 
                                            in->pixels[i][j].green * filter_matrix[1][1] + 
                                            in->pixels[i][j+1].green * filter_matrix[1][2] + 
                                            in->pixels[i+1][j-1].green * filter_matrix[2][0] + 
                                            in->pixels[i+1][j].green * filter_matrix[2][1] + 
                                            in->pixels[i+1][j+1].green * filter_matrix[2][2];

                out->pixels[i][j].blue = in->pixels[i-1][j-1].blue * filter_matrix[0][0] + 
                                            in->pixels[i-1][j].blue * filter_matrix[0][1] + 
                                            in->pixels[i-1][j+1].blue * filter_matrix[0][2] + 
                                            in->pixels[i][j-1].blue * filter_matrix[1][0] + 
                                            in->pixels[i][j].blue * filter_matrix[1][1] + 
                                            in->pixels[i][j+1].blue * filter_matrix[1][2] + 
                                            in->pixels[i+1][j-1].blue * filter_matrix[2][0] + 
                                            in->pixels[i+1][j].blue * filter_matrix[2][1] + 
                                            in->pixels[i+1][j+1].blue * filter_matrix[2][2];
            }   
        }
        
        for (i = 1; i <= final_row; ++i) {
            for (j = 1; j < in->width - 1; ++j) {
                in->pixels[i][j].red = out->pixels[i][j].red;
                in->pixels[i][j].green = out->pixels[i][j].green;
                in->pixels[i][j].blue = out->pixels[i][j].blue;
            }
        }   
    }
}

// Calculeaza in vectorul tasks valorile de inceput si sfarsit ale
// fiecarui proces
void calculate_tasks(int tasks[], int N, int nProcesses) {
    int i;

    for (i = 0; i < nProcesses; i++) {
        tasks[i] = 0;
    }

    for (i = 0; i < N % nProcesses; i++) {
        tasks[i] += N / nProcesses + 1;
    }

    for (i = N % nProcesses; i < nProcesses; i++) {
        tasks[i] += N / nProcesses;
    }

    for (i = 1; i < nProcesses; i++) {
        tasks[i] = tasks[i-1] + tasks[i];
    }
    
    for (i = 0; i < nProcesses; i++) {
        tasks[i]--;
    }
}

// Functie ce pune pixelii dintr-un buffer intr-o matrice
void matrix_to_buffer(image *img, unsigned char *buffer, int start, int end) {
    int i, j, k = 0;

    if (img->num_colors == 1) {
        for (i = start; i <= end; ++i) {
            for (j = 0; j < img->width; j++) {
                buffer[k++] = img->pixels[i][j].gray;
            }
        }
    } else {
        for (i = start; i <= end; ++i) {
            for (j = 0; j < img->width; j++) {
                buffer[k++] = img->pixels[i][j].red;
                buffer[k++] = img->pixels[i][j].green;
                buffer[k++] = img->pixels[i][j].blue;
            }
        }
    }
}

// Functie ce pune pixelii dintr-o matrice intr-un buffer
void buffer_to_matrix(image *img, unsigned char *buffer, int start, int end) {
    int i, j, k = 0;

    if (img->num_colors == 1) {
        for (i = start; i <= end; ++i) {
            for (j = 0; j < img->width; j++) {
                img->pixels[i][j].gray = buffer[k++];
            }
        }
    } else {
        for (i = start; i <= end; ++i) {
            for (j = 0; j < img->width; j++) {
                img->pixels[i][j].red = buffer[k++];
                img->pixels[i][j].green = buffer[k++];
                img->pixels[i][j].blue = buffer[k++];
            }
        }
    }
}

int main(int argc, char * argv[]) {
	if (argc < 4) {
		exit(-1);
	}

	image input, output;
    int i, rank, nProcesses, start, end, tasks[100], process_height, extra_row;
    float smooth_matrix[3][3] = {{1.0 / 9, 1.0 / 9, 1.0 / 9}, {1.0 / 9, 1.0 / 9, 1.0 / 9}, {1.0 / 9, 1.0 / 9, 1.0 / 9}};
    float blur_matrix[3][3] = {{1.0 / 16, 2.0 / 16, 1.0 / 16}, {2.0 / 16, 4.0 / 16, 2.0 / 16}, {1.0 / 16, 2.0 / 16, 1.0 / 16}};
    float sharpen_matrix[3][3] = {{0, -2.0 / 3, 0}, {-2.0 / 3, 11.0 / 3, -2.0 / 3}, {0, -2.0 / 3, 0}};
    float mean_matrix[3][3] = {{-1, -1, -1}, {-1, 9, -1}, {-1, -1, -1}};
    float emboss_matrix[3][3] = {{0, 1, 0}, {0, 0, 0}, {0, -1, 0}};
    unsigned char *buffer;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    extra_row = rank != 0 && rank != nProcesses - 1;

    // Procesul parinte (0) citeste imaginea de input si trimite celorlalte
    // procese inaltimea, latimea, numarul de culori si cate o submatrice
    // din imaginea initiala
    if (rank == 0) {
	    read_input(argv[1], &input);
        calculate_tasks(tasks, input.height, nProcesses);
        process_height = tasks[0] + 1 + (nProcesses > 1);

        if (nProcesses > 1) {
            for (i = 1; i < nProcesses; ++i) {
                MPI_Send(&(input.height), 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&(input.width), 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&(input.num_colors), 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                
                start = tasks[i - 1] + 1;
                end = tasks[i];

                buffer = (unsigned char*) malloc(sizeof(unsigned char) * sizeof(pixel) * input.height * input.width);
                matrix_to_buffer(&input, buffer, start, end);
                MPI_Send(buffer, sizeof(pixel) * (end - start + 1) * input.width, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        // Celelalte procese primesc de la parinte informatiile si le salveaza
        // in imaginea lor de input
        MPI_Recv(&(input.height), 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&(input.width), 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&(input.num_colors), 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        calculate_tasks(tasks, input.height, nProcesses);
        start = tasks[rank - 1] + 1;
        end = tasks[rank];
        process_height = end - start + 2 + extra_row;

        input.pixels = (pixel**) malloc(sizeof(pixel*) * process_height);
        for (i = 0; i < process_height; ++i) {
            input.pixels[i] = (pixel*) malloc(sizeof(pixel) * input.width);
        }

        buffer = (unsigned char*) malloc(sizeof(unsigned char) * sizeof(pixel) * input.height * input.width);
        MPI_Recv(buffer, sizeof(pixel) * (end - start + 1) * input.width, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        buffer_to_matrix(&input, buffer, 1, end - start + 1);
    }


    // Fiecare proces isi creeaza o matrice de output
    output.height = (rank == 0) ? input.height : process_height;
    output.width = input.width;
    output.maxval = input.maxval;
    output.num_colors = input.num_colors;
    output.pixels = (pixel**) malloc(sizeof(pixel*) * output.height);
    for (i = 0; i < output.height; ++i) {
        output.pixels[i] = (pixel*) malloc(sizeof(pixel) * output.width);
    }

    // Procesul 0 copiaza pixelii de pe margini in matricea sa de output
    if (rank == 0) {
        fill_borders(&input, &output);
    }

    // Fiecare proces trimite prima linie catre procesul anterior (daca exista)
    // si ultima linie catre procesul urmator (daca exista) si primeste
    // la randul lui ultima linie de la procesul anterior (daca exista) si
    // prima linie de la urmatorul proces (daca exista). Astfel, toate procesele
    // au toate liniile necesare. Apoi, fiecare proces aplica toate filtrele
    // si isi suprascriu submatricea obtinuta dupa aplicarea filtrului
    // in submatricea de input
    for (i = 3; i < argc; i++) {
        if (nProcesses > 1) {
            if (argc > 4) {
                if (rank == 0) {
                    MPI_Ssend(input.pixels[tasks[0]], sizeof(pixel) * input.width, MPI_UNSIGNED_CHAR, rank + 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(input.pixels[tasks[0] + 1], sizeof(pixel) * input.width, MPI_UNSIGNED_CHAR, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                } else if (rank == nProcesses - 1) {
                    MPI_Recv(input.pixels[0], sizeof(pixel) * input.width, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    MPI_Ssend(input.pixels[1], sizeof(pixel) * input.width, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD);
                } else {
                    MPI_Recv(input.pixels[0], sizeof(pixel) * input.width, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    MPI_Ssend(input.pixels[1], sizeof(pixel) * input.width, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD);

                    MPI_Ssend(input.pixels[process_height - 2], sizeof(pixel) * input.width, MPI_UNSIGNED_CHAR, rank + 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(input.pixels[process_height - 1], sizeof(pixel) * input.width, MPI_UNSIGNED_CHAR, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
            }
        }

        if (strncmp(argv[i], "smooth", 6) == 0) {
            apply_filter(&input, &output, process_height - 2, smooth_matrix);
        } else if (strncmp(argv[i], "blur", 4) == 0) {
            apply_filter(&input, &output, process_height - 2, blur_matrix);
        } else if (strncmp(argv[i], "sharpen", 7) == 0) {
            apply_filter(&input, &output, process_height - 2, sharpen_matrix);
        } else if (strncmp(argv[i], "mean", 4) == 0) {
            apply_filter(&input, &output, process_height - 2, mean_matrix);
        } else if (strncmp(argv[i], "emboss", 6) == 0) {
            apply_filter(&input, &output, process_height - 2, emboss_matrix);
        }
    }


    // Procesul 0 primeste submatricile rezultate de la fiecare proces,
    // iar celelalte procese le trimit la procesul 0
    if (rank == 0) {
        if (nProcesses > 1) {
            for (i = 1; i < nProcesses; ++i) {
                extra_row = i != 0 && i != nProcesses - 1;
                start = tasks[i - 1] + 1;
                end = tasks[i];
                process_height = end - start + 2 + extra_row;

                MPI_Recv(buffer, sizeof(pixel) * (process_height - 2) * input.width, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                buffer_to_matrix(&output, buffer, start, start + process_height - 2);
            }
        }
        
	    write_data(argv[2], &output);
    } else {
        matrix_to_buffer(&input, buffer, 1, process_height - 2);
        MPI_Send(buffer, sizeof(pixel) * (process_height - 2) * input.width, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
	return 0;
}