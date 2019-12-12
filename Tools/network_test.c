#include "../network.h"
#include "../training_example.h"
#include "../saving.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <err.h>

// This program can be used to test the performances of a network over a
// database. Example: ./NetworkTest.exe Data/OCR_network Data/letters_database

int main(int argc, char** argv) {

    srand((unsigned int)time(NULL));

    if(argc != 3)
        err(1, "Please pass a network and a database in parameter");

    int nbEx = 0;
    Network* network = readNetwork(argv[1]);
    TrainingExample** exArray = readDatabase(argv[2], &nbEx);

    int answer = 0;
    int label = 0;
    while(answer == label) {

        TrainingExample** subset = getExamplesSubset(exArray, nbEx, 1);
        TrainingExample* ex = subset[0];
        free(subset);
        double* outputs = getNetworkAnswer(network, ex->inputs);

        //Displays the image in the terminal
        for(int y = 0; y < 28; y++) {
            for(int x = 0; x < 28; x++)
                printf("%c", ex->inputs[y*28+x] ? 'O' : '.');
            printf("\n");
        }

        //Displays the label and the networks answer
        int nbOut = getOutputNumber(network);

        //Displays the output of each neuron of the output layer
        //for(int i = 0; i < nbOut; i++)
            //printf("[%i]: %lf\n", i, outputs[i]);

        //Displays the results
        answer = getMaxIndex(outputs, nbOut);
        label = getMaxIndex(ex->label, nbOut);
        printf("Answer: %i\n", answer);
        printf("Label:  %i\n", label);
        printf(answer == label ? "\x1b[44mOK\x1b[0m\n" : "\x1b[41mKO\x1b[0m\n");

        free(outputs)
    }

    return 0;
}
