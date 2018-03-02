#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_header.h"

// Structura pixel - 3 canale de culoare: r, g, b
typedef struct
{
    int r, g, b, bin;
} pixel;

// Structura ce retine numele de pentru fisierele de input: fisierul bmp si
// fisierul bin si numele pentru fisierele de output
typedef struct
{
    char bmp [100];
    char threshold [100];
    char bin [100];
    char *bw, *f1, *f2, *f3;
} input_data;

// Aloca dinamic o matrice si initializeaza elementele cu 0
pixel **create_matrix (int height, int width)
{
    int i, j;

    pixel **matrix;

    matrix = (pixel **) calloc (height, sizeof (pixel *)); 

    if (matrix == NULL)
        return NULL;

    for (i=0; i<height; i++)
    {
        matrix[i] = (pixel *) calloc (width, sizeof (pixel));
        if (matrix[i] == NULL)
        {
            for (j=0; j<i; j++)
                free (matrix[j]);
            return NULL;
        }
    }

    return matrix;
}

// Elibereaza memoria alocata unei matrici
void free_matrix (pixel ***matrix, int height)
{
    int i;

    for (i=0; i<height; i++)   
        free ((*matrix)[i]);
    
    free (*matrix);
    *matrix = NULL;
}

// Creeaza o matrice goala in care copiaza elementele matricei img argument
pixel **copy (pixel **img, int height, int width)
{
    int i, j;

    pixel **aux;
    aux = create_matrix (height, width);

    for (i=0; i<height; i++)
        for (j=0; j<width; j++)
        {
            aux[i][j].r = img[i][j].r;
            aux[i][j].g = img[i][j].g;
            aux[i][j].b = img[i][j].b;
        }
    
    return aux;
}

// Intoarce o matrice
pixel **flip (pixel **img, int height, int width)
{
    int i, j;

    pixel **aux;
    aux = create_matrix (height, width);

    for (i=0; i<height; i++)
        for (j=0; j<width; j++)
        {
            aux [height - i - 1][j].r = img [i][j].r;
            aux [height - i - 1][j].g = img [i][j].g;
            aux [height - i - 1][j].b = img [i][j].b;
        }

    return aux;   
}

// Creeaza numele pentru fisierele de output
void output_name (input_data image [100], int n, int code)
{
    if (code == 0)
    {
        char *strAux = malloc (100);
        strcpy (strAux, image[n].bmp);
        strAux [strlen (strAux) - 4] = '\0';
        image[n].bw = malloc (strlen (strAux) + 17);
        strcpy (image[n].bw, strAux);
        strcat (image[n].bw, "_black_white.bmp");
        free (strAux);
    }

    else if (code == 1)
    {
        char *strAux = malloc (100);
        strcpy (strAux, image[n].bmp);
        strAux [strlen (strAux) - 4] = '\0';
        image[n].f1 = malloc (strlen (strAux) + 20);
        strcpy (image[n].f1, strAux);
        strcat (image[n].f1, "_f1.bmp");
        free (strAux);
    }

    else if (code == 2)
    {
        char *strAux = malloc (100);
        strcpy (strAux, image[n].bmp);
        strAux [strlen (strAux) - 4] = '\0';
        image[n].f2 = malloc (strlen (strAux) + 20);
        strcpy (image[n].f2, strAux);
        strcat (image[n].f2, "_f2.bmp");
        free (strAux);
    }

    else if (code == 3)
    {
        char *strAux = malloc (100);
        strcpy (strAux, image[n].bmp);
        strAux [strlen (strAux) - 4] = '\0';
        image[n].f3 = malloc (strlen (strAux) + 20);
        strcpy (image[n].f3, strAux);
        strcat (image[n].f3, "_f3.bmp");
        free (strAux);
    }
}

// Returneaza o matrice in care se aplica algoritmul de alb-negru
pixel **bw (pixel **img, input_data image [100], bmp_fileheader fileheader, 
            bmp_infoheader infoheader, unsigned char padding)
{
    int i, j, height = infoheader.height, width = infoheader.width;

    pixel **aux;
    aux = create_matrix (height, width);

    output_name (image, 0, 0);
    FILE *output_bmp_bw = fopen (image[0].bw, "wb");

    fwrite (&fileheader, sizeof (bmp_fileheader), 1, output_bmp_bw);
    fwrite (&infoheader, sizeof (bmp_infoheader), 1, output_bmp_bw);
    fseek (output_bmp_bw, fileheader.imageDataOffset, SEEK_SET);    

    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            aux[i][j].r = (img[i][j].r + img[i][j].g + img[i][j].b)/3;
            aux[i][j].g = (img[i][j].r + img[i][j].g + img[i][j].b)/3;
            aux[i][j].b = (img[i][j].r + img[i][j].g + img[i][j].b)/3;
            
            fwrite (&aux[i][j].b, sizeof (char), 1, output_bmp_bw);
            fwrite (&aux[i][j].g, sizeof (char), 1, output_bmp_bw);
            fwrite (&aux[i][j].r, sizeof (char), 1, output_bmp_bw);
        }
        fwrite (&padding, sizeof (char), padding, output_bmp_bw);
    }

    free (image[0].bw);

    if (output_bmp_bw != NULL)
        fclose (output_bmp_bw);
    
    return aux;
}

// Returneaza o matrice in care se aplica un filtru
pixel **apply_filter (pixel **img, int height, int width, int filter [3][3])
{
    int i, j;

    pixel **aux = create_matrix (height, width);

    for (i=0; i<height; i++)
        for (j=0; j<width; j++)
        {    
            if (i!=0 && i!=height-1 && j!=0 && j!=width-1)
            {
                aux[i][j].r = img [i-1][j-1].r * filter [0][0] + 
                              img [i-1][j].r   * filter [0][1] + 
                              img [i-1][j+1].r * filter [0][2] + 
                              img [i][j-1].r   * filter [1][0] +
                              img [i][j].r     * filter [1][1] + 
                              img [i][j+1].r   * filter [1][2] +
                              img [i+1][j-1].r * filter [2][0] +
                              img [i+1][j].r   * filter [2][1] +
                              img [i+1][j+1].r * filter [2][2];
                
                aux[i][j].g = img [i-1][j-1].g * filter [0][0] + 
                              img [i-1][j].g   * filter [0][1] + 
                              img [i-1][j+1].g * filter [0][2] + 
                              img [i][j-1].g   * filter [1][0] +
                              img [i][j].g     * filter [1][1] + 
                              img [i][j+1].g   * filter [1][2] +
                              img [i+1][j-1].g * filter [2][0] +
                              img [i+1][j].g   * filter [2][1] +
                              img [i+1][j+1].g * filter [2][2];
   
                aux[i][j].b = img [i-1][j-1].b * filter [0][0] + 
                              img [i-1][j].b   * filter [0][1] + 
                              img [i-1][j+1].b * filter [0][2] + 
                              img [i][j-1].b   * filter [1][0] +
                              img [i][j].b     * filter [1][1] + 
                              img [i][j+1].b   * filter [1][2] +
                              img [i+1][j-1].b * filter [2][0] +
                              img [i+1][j].b   * filter [2][1] +
                              img [i+1][j+1].b * filter [2][2];
            }

            if (i==0 && i<height && j==0 && j<width)
            { 
                aux[i][j].r = img [i][j].r     * filter [1][1] +
                              img [i][j+1].r   * filter [1][2] +
                              img [i+1][j].r   * filter [2][1] +
                              img [i+1][j+1].r * filter [2][2];
                
                aux[i][j].g = img [i][j].g     * filter [1][1] +
                              img [i][j+1].g   * filter [1][2] +
                              img [i+1][j].g   * filter [2][1] +
                              img [i+1][j+1].g * filter [2][2];
   
                aux[i][j].b = img [i][j].b     * filter [1][1] +
                              img [i][j+1].b   * filter [1][2] +
                              img [i+1][j].b   * filter [2][1] +
                              img [i+1][j+1].b * filter [2][2]; 
            }

            if (i==0 && i<height && j==width-1 && j>0)
            { 
                aux[i][j].r = img [i][j-1].r   * filter [1][0] +
                              img [i][j].r     * filter [1][1] + 
                              img [i+1][j-1].r * filter [2][0] +
                              img [i+1][j].r   * filter [2][1];
                
                aux[i][j].g = img [i][j-1].g   * filter [1][0] +
                              img [i][j].g     * filter [1][1] + 
                              img [i+1][j-1].g * filter [2][0] +
                              img [i+1][j].g   * filter [2][1];
   
                aux[i][j].b = img [i][j-1].b   * filter [1][0] +
                              img [i][j].b     * filter [1][1] + 
                              img [i+1][j-1].b * filter [2][0] +
                              img [i+1][j].b   * filter [2][1];
            }

            if (i==height-1 && i>0 && j==0 && j<width)
            {
                aux[i][j].r = img [i-1][j].r   * filter [0][1] + 
                              img [i-1][j+1].r * filter [0][2] + 
                              img [i][j].r     * filter [1][1] + 
                              img [i][j+1].r   * filter [1][2];
                
                aux[i][j].g = img [i-1][j].g   * filter [0][1] + 
                              img [i-1][j+1].g * filter [0][2] + 
                              img [i][j].g     * filter [1][1] + 
                              img [i][j+1].g   * filter [1][2];
   
                aux[i][j].b = img [i-1][j].b   * filter [0][1] + 
                              img [i-1][j+1].b * filter [0][2] + 
                              img [i][j].b     * filter [1][1] + 
                              img [i][j+1].b   * filter [1][2];
            }

            if (i==height-1 && i>0 && j==width-1 && j>0)
            {
                aux[i][j].r = img [i-1][j-1].r * filter [0][0] + 
                              img [i-1][j].r   * filter [0][1] + 
                              img [i][j-1].r   * filter [1][0] +
                              img [i][j].r     * filter [1][1];
                
                aux[i][j].g = img [i-1][j-1].g * filter [0][0] + 
                              img [i-1][j].g   * filter [0][1] + 
                              img [i][j-1].g   * filter [1][0] +
                              img [i][j].g     * filter [1][1]; 
   
                aux[i][j].b = img [i-1][j-1].b * filter [0][0] + 
                              img [i-1][j].b   * filter [0][1] + 
                              img [i][j-1].b   * filter [1][0] +
                              img [i][j].b     * filter [1][1];
            }

            if (i==0 && i<height && j!=0 && j!=width-1)
            {
                aux[i][j].r = img [i][j-1].r   * filter [1][0] +
                              img [i][j].r     * filter [1][1] + 
                              img [i][j+1].r   * filter [1][2] +
                              img [i+1][j-1].r * filter [2][0] +
                              img [i+1][j].r   * filter [2][1] +
                              img [i+1][j+1].r * filter [2][2];
                
                aux[i][j].g = img [i][j-1].g   * filter [1][0] +
                              img [i][j].g     * filter [1][1] + 
                              img [i][j+1].g   * filter [1][2] +
                              img [i+1][j-1].g * filter [2][0] +
                              img [i+1][j].g   * filter [2][1] +
                              img [i+1][j+1].g * filter [2][2];
   
                aux[i][j].b = img [i][j-1].b   * filter [1][0] +
                              img [i][j].b     * filter [1][1] + 
                              img [i][j+1].b   * filter [1][2] +
                              img [i+1][j-1].b * filter [2][0] +
                              img [i+1][j].b   * filter [2][1] +
                              img [i+1][j+1].b * filter [2][2];
            }
           
            if (i==height-1 && i>0 && j!=0 && j!=width-1)
            {
                aux[i][j].r = img [i-1][j-1].r * filter [0][0] + 
                              img [i-1][j].r   * filter [0][1] + 
                              img [i-1][j+1].r * filter [0][2] + 
                              img [i][j-1].r   * filter [1][0] +
                              img [i][j].r     * filter [1][1] + 
                              img [i][j+1].r   * filter [1][2];
                
                aux[i][j].g = img [i-1][j-1].g * filter [0][0] + 
                              img [i-1][j].g   * filter [0][1] + 
                              img [i-1][j+1].g * filter [0][2] + 
                              img [i][j-1].g   * filter [1][0] +
                              img [i][j].g     * filter [1][1] + 
                              img [i][j+1].g   * filter [1][2];
   
                aux[i][j].b = img [i-1][j-1].b * filter [0][0] + 
                              img [i-1][j].b   * filter [0][1] + 
                              img [i-1][j+1].b * filter [0][2] + 
                              img [i][j-1].b   * filter [1][0] +
                              img [i][j].b     * filter [1][1] + 
                              img [i][j+1].b   * filter [1][2];
            }

            if (j==0 && j<width && i!=0 && i!=height-1)
            {
                aux[i][j].r = img [i-1][j].r   * filter [0][1] + 
                              img [i-1][j+1].r * filter [0][2] + 
                              img [i][j].r     * filter [1][1] + 
                              img [i][j+1].r   * filter [1][2] +
                              img [i+1][j].r   * filter [2][1] +
                              img [i+1][j+1].r * filter [2][2];
                
                aux[i][j].g = img [i-1][j].g   * filter [0][1] + 
                              img [i-1][j+1].g * filter [0][2] + 
                              img [i][j].g     * filter [1][1] + 
                              img [i][j+1].g   * filter [1][2] +
                              img [i+1][j].g   * filter [2][1] +
                              img [i+1][j+1].g * filter [2][2];
   
                aux[i][j].b = img [i-1][j].b   * filter [0][1] + 
                              img [i-1][j+1].b * filter [0][2] + 
                              img [i][j].b     * filter [1][1] + 
                              img [i][j+1].b   * filter [1][2] +
                              img [i+1][j].b   * filter [2][1] +
                              img [i+1][j+1].b * filter [2][2];
            }
          
            if (j==width-1 && j>0 && i!=0 && i!=height-1)
            {
                aux[i][j].r = img [i-1][j-1].r * filter [0][0] + 
                              img [i-1][j].r   * filter [0][1] + 
                              img [i][j-1].r   * filter [1][0] +
                              img [i][j].r     * filter [1][1] + 
                              img [i+1][j-1].r * filter [2][0] +
                              img [i+1][j].r   * filter [2][1];
                
                aux[i][j].g = img [i-1][j-1].g * filter [0][0] + 
                              img [i-1][j].g   * filter [0][1] + 
                              img [i][j-1].g   * filter [1][0] +
                              img [i][j].g     * filter [1][1] + 
                              img [i+1][j-1].g * filter [2][0] +
                              img [i+1][j].g   * filter [2][1];
   
                aux[i][j].b = img [i-1][j-1].b * filter [0][0] + 
                              img [i-1][j].b   * filter [0][1] + 
                              img [i][j-1].b   * filter [1][0] +
                              img [i][j].b     * filter [1][1] + 
                              img [i+1][j-1].b * filter [2][0] +
                              img [i+1][j].b   * filter [2][1];
            } 
        }

    for (i=0; i<height; i++)
        for (j=0; j<width; j++)
        {
            if (aux[i][j].r < 0)
                aux[i][j].r = 0; 

            if (aux[i][j].g < 0)
                aux[i][j].g = 0;

            if (aux[i][j].b < 0)
                aux[i][j].b = 0;

            if (aux[i][j].r > 255)
                aux[i][j].r = 255; 

            if (aux[i][j].g > 255)
                aux[i][j].g = 255;

            if (aux[i][j].b > 255)
                aux[i][j].b = 255;
        }

    return aux;
}       

// Face decompresia fisierului binar si creeaza imaginea bmp cu
// o calitate scazuta, dar si dimensiune redusa
void decompress (char *filename, bmp_fileheader fileheader, 
                bmp_infoheader infoheader, unsigned char padding)
{
    int i, j, height = infoheader.height, width = infoheader.width;
    pixel **compressed, **aux;

    FILE *input_compressed = fopen (filename, "rb");  
    FILE *output_decompressed = fopen ("decompressed.bmp", "wb");
    
    fwrite (&fileheader, sizeof (bmp_fileheader), 1, output_decompressed);
    fwrite (&infoheader, sizeof (bmp_infoheader), 1, output_decompressed);
    fseek (output_decompressed, fileheader.imageDataOffset, SEEK_SET);

    fseek (input_compressed, fileheader.imageDataOffset, SEEK_SET);
    compressed = create_matrix (height, width);

    for (i=0; i<height; i++)
        for (j=0; j<width; j++)
            compressed[i][j].bin = 0;

    while (feof (input_compressed) == 0)
    {
        fread (&i, sizeof (char), 2, input_compressed); 
        fread (&j, sizeof (char), 2, input_compressed);
        fread (&compressed[i-1][j-1].r, sizeof (char), 1, input_compressed);
        fread (&compressed[i-1][j-1].g, sizeof (char), 1, input_compressed);
        fread (&compressed[i-1][j-1].b, sizeof (char), 1, input_compressed);
        compressed[i-1][j-1].bin = 1;
    }

    for (i=0; i<height; i++)
        for (j=0; j<width - 1; j++)
            if (compressed[i][j+1].bin == 0)
            {
                compressed[i][j+1].r = compressed[i][j].r;
                compressed[i][j+1].g = compressed[i][j].g;
                compressed[i][j+1].b = compressed[i][j].b;
            }
    
    aux = flip (compressed, height, width);
    free_matrix (&compressed, height);
    
    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            fwrite (&aux[i][j].b, sizeof (char), 1, output_decompressed);
            fwrite (&aux[i][j].g, sizeof (char), 1, output_decompressed);
            fwrite (&aux[i][j].r, sizeof (char), 1, output_decompressed);
        }
        fwrite (&padding, sizeof (char), padding, output_decompressed);
    }
    
    free_matrix (&aux, height);

    if (input_compressed != NULL)
        fclose (input_compressed);

    if (output_decompressed != NULL)
        fclose (output_decompressed);
}

int main ()
{
    int i, j, k = 0, height, width, widthAux;
    unsigned char padding;
    pixel **img, **aux, **bw_img;

    bmp_fileheader fileheader;
    bmp_infoheader infoheader;
    input_data image [100];

    FILE *input_file = fopen ("input.txt", "rt");

    int filter1 [3][3] = {  {-1, -1, -1},
                            {-1, 8, -1},
                            {-1, -1, -1} };
    int filter2 [3][3] = {  {0, 1, 0},
                            {1, -4, 1},
                            {0, 1, 0}    };
    int filter3 [3][3] = {  {1, 0, -1},
                            {0, 0, 0},
                            {-1, 0, 1}   };

    // Citire date de input
    fgets (image[k].bmp, 100, input_file); 
    fgets (image[k].threshold, 100, input_file);
    fgets (image[k].bin, 100, input_file);
   
    image[k].bmp [strlen (image[k].bmp) - 1] = '\0';
    image[k].bin [strlen (image[k].bin) - 1] = '\0';
    
    char *input_bmp_filename = malloc (70);
    strcat (input_bmp_filename, "inputs/");
    strcat (input_bmp_filename, image[k].bmp);

    char *input_bin_filename = malloc (70);
    strcat (input_bin_filename, "inputs/");
    strcat (input_bin_filename, image[k].bin);
    
    // Citire imagine din bmp
    FILE *input_bmp = fopen (input_bmp_filename, "rb");
   
    fread (&fileheader, sizeof (bmp_fileheader), 1, input_bmp);
    fread (&infoheader, sizeof (bmp_infoheader), 1, input_bmp);

    padding = 0;
    height = infoheader.height;
    width = infoheader.width;  
    widthAux = width * 3;

    while (widthAux % 4 != 0)
    {
        padding++;
        widthAux++;
    }
    
    fseek (input_bmp, fileheader.imageDataOffset, SEEK_SET);
    img = create_matrix (height, width);

    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            fread (&img[i][j].b, sizeof (char), 1, input_bmp);
            fread (&img[i][j].g, sizeof (char), 1, input_bmp);
            fread (&img[i][j].r, sizeof (char), 1, input_bmp);
        }   
        fseek (input_bmp, padding, SEEK_CUR);
    }

    // Creare imagine alb negru
    bw_img = bw (img, image, fileheader, infoheader, padding);
    free_matrix (&img, height);

    // Filtru 1
    output_name (image, 0, 1);
    FILE *output_bmp_f1 = fopen (image[0].f1, "wb");

    fwrite (&fileheader, sizeof (bmp_fileheader), 1, output_bmp_f1);
    fwrite (&infoheader, sizeof (bmp_infoheader), 1, output_bmp_f1);
    fseek (output_bmp_f1, fileheader.imageDataOffset, SEEK_SET);    

    aux = flip (bw_img, height, width);
    img = copy (aux, height, width);
    free_matrix (&aux, height);
    aux = apply_filter (img, height, width, filter1);
    free_matrix (&img, height);
    img = flip (aux, height, width);
    free_matrix (&aux, height);

    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            fwrite (&img[i][j].b, sizeof (char), 1, output_bmp_f1);
            fwrite (&img[i][j].g, sizeof (char), 1, output_bmp_f1);
            fwrite (&img[i][j].r, sizeof (char), 1, output_bmp_f1);
        }
        fwrite (&padding, sizeof (char), padding, output_bmp_f1);
    }

    free_matrix (&img, height);

    // Filtru 2
    output_name (image, 0, 2);
    FILE *output_bmp_f2 = fopen (image[0].f2, "wb");

    fwrite (&fileheader, sizeof (bmp_fileheader), 1, output_bmp_f2);
    fwrite (&infoheader, sizeof (bmp_infoheader), 1, output_bmp_f2);
    fseek (output_bmp_f2, fileheader.imageDataOffset, SEEK_SET);    

    aux = flip (bw_img, height, width);
    img = copy (aux, height, width);
    free_matrix (&aux, height);
    aux = apply_filter (img, height, width, filter2);
    free_matrix (&img, height);
    img = flip (aux, height, width);
    free_matrix (&aux, height);

    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            fwrite (&img[i][j].b, sizeof (char), 1, output_bmp_f2);
            fwrite (&img[i][j].g, sizeof (char), 1, output_bmp_f2);
            fwrite (&img[i][j].r, sizeof (char), 1, output_bmp_f2);
        }
        fwrite (&padding, sizeof (char), padding, output_bmp_f2);
    }

    free_matrix (&img, height);

    // Filtru 3
    output_name (image, 0, 3);
    FILE *output_bmp_f3 = fopen (image[0].f3, "wb");

    fwrite (&fileheader, sizeof (bmp_fileheader), 1, output_bmp_f3);
    fwrite (&infoheader, sizeof (bmp_infoheader), 1, output_bmp_f3);
    fseek (output_bmp_f3, fileheader.imageDataOffset, SEEK_SET);    

    aux = flip (bw_img, height, width);
    img = copy (aux, height, width);
    free_matrix (&aux, height);
    aux = apply_filter (img, height, width, filter3);
    free_matrix (&img, height);
    img = flip (aux, height, width);
    free_matrix (&aux, height);

    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            fwrite (&img[i][j].b, sizeof (char), 1, output_bmp_f3);
            fwrite (&img[i][j].g, sizeof (char), 1, output_bmp_f3);
            fwrite (&img[i][j].r, sizeof (char), 1, output_bmp_f3);
        }
        fwrite (&padding, sizeof (char), padding, output_bmp_f3);
    }

    free_matrix (&img, height);

    // Decompresie
    decompress (input_bin_filename, fileheader, infoheader, padding);

    // Elibari de memorie
    free (image[k].f1);
    free (image[k].f2);
    free (image[k].f3);
    free_matrix (&bw_img, height);
    free (input_bmp_filename);
    free (input_bin_filename);

    if (input_bmp != NULL)
        fclose (input_bmp);

    if (input_file != NULL) 
        fclose (input_file);

    if (output_bmp_f1 != NULL)
        fclose (output_bmp_f1);

    if (output_bmp_f2 != NULL)
        fclose (output_bmp_f2);

    if (output_bmp_f3 != NULL)
        fclose (output_bmp_f3);

    return 0;
}
