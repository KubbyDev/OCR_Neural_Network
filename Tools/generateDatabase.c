#include "../letters_classifier.h"
#include "../file_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

// This code will generate a network database from a raw image file
// The raw image file is a file with the number of images in the first line
// And on each line the width, the height and the pixels values separated by
// spaces. The pixels values are 0 and 1.

#define SIDELENGTH 28
#define NBLABELS 70

//#define AUTO_LABELS
//#define USER_LABELS
#define FILE_LABELS

int main(int argc, char** argv) {

    if(argc != 3)
        errx(1, "You must provide a raw image file and the database path in parameter");

    char* stream = readfile(argv[1]);
    int nbExamples = readint(&stream);

    char* generated = malloc(sizeof(char) *
            (NBLABELS*2 + SIDELENGTH*SIDELENGTH*2)*nbExamples + 20);
    char* buffer = generated;

#ifdef FILE_LABELS
    char* labels = readfile("/home/kubby/Downloads/loremipsum");
#endif

    buffer = writeint(buffer, nbExamples, ' ');
    buffer = writeint(buffer, SIDELENGTH*SIDELENGTH, ' ');
    buffer = writeint(buffer, NBLABELS, '\n');

    for(int i = 0; i < nbExamples; i++) {

        //Reads the image
        int width = readint(&stream);
        int height = readint(&stream);
        int* img = malloc(sizeof(int)*width*height);
        for(int j = 0; j < width*height; j++)
            img[j] = readint(&stream);

            printf("\n");
            printf("\n");
            printf("\n");

        for(int y = 0; y < height; y++) {
            for(int x = 0; x < width; x++)
                printf("%c ", img[y*width+x] > 0 ? 'O' : '.');
            printf("\n");
        }
        printf("\n");

        //Scales it for the network
        double* image = scaleForNetwork(img, width, height);
        free(img);

        char label = 0;
#ifdef AUTO_LABELS
        //When the examples are ordered we can just use this
        label = i%NBLABELS;
#endif
#ifdef USER_LABELS
        //Asks the user what letter it is
        for(int y = 0; y < SIDELENGTH; y++) {
            for(int x = 0; x < SIDELENGTH; x++)
                printf("%c ", image[y*SIDELENGTH+x] > 0 ? 'O' : '.');
            printf("\n");
        }
        scanf("%c", &label);
        label = neuronIndexFromChar(label);
#endif
#ifdef FILE_LABELS
        //Gets the labels from a file
        while(labels[0] == ' ' || labels[0] == '\n')
            labels += 1;
        label = neuronIndexFromChar(labels[0]);
        printf("%c", labels[0]);
        labels++;
#endif

        //Writes it on the generated database
        for(int j = 0; j < SIDELENGTH*SIDELENGTH; j++)
            buffer = writeint(buffer, image[j] > 0.5, ' ');
        for(int j = 0; j < NBLABELS; j++)
            buffer = writeint(buffer, j == label ? 1 : 0, ' ');
        buffer[-1] = '\n';
    }

    writefile(argv[2], generated);
}
