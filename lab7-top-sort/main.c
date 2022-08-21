#include <stdio.h>
#include <stdlib.h>

#define u_char unsigned char

#define in  stdin

int Read();

int main() {
    int NumberOfVertices = Read();
    int NumberOfEdges    = Read();
    if (NumberOfVertices == EOF - '0' || NumberOfEdges == EOF - '0') {
        printf("bad number of lines");
        return 0;
    }
    if (NumberOfVertices < 0 || NumberOfVertices > 2000) {
        printf("bad number of vertices");
        return 0;
    }
    if (NumberOfEdges < 0 || NumberOfEdges > NumberOfVertices * (NumberOfVertices + 1) / 2) {
        printf("bad number of edges");
        return 0;
    }
    int StringLength = NumberOfVertices / 8 + (NumberOfVertices % 8 > 0);

    u_char* Matrix = (u_char*) calloc(sizeof(u_char), NumberOfVertices * StringLength);
    if (Matrix == NULL) {
        return EXIT_FAILURE;
    }

    short* NumberOfIncomingEdges = (short*) calloc(sizeof(short), NumberOfVertices);
    if (NumberOfIncomingEdges == NULL) {
        free(Matrix);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < NumberOfEdges; i++) {
        int Start = Read();
        if (Start == EOF - '0') {
            free(Matrix);
            free(NumberOfIncomingEdges);
            printf("bad number of lines");
            return 0;
        }

        int End = Read();
        if (Start < 1 || Start > NumberOfVertices || End < 1 || End > NumberOfVertices) {
            printf("bad vertex");
            free(NumberOfIncomingEdges);
            free(Matrix);
            return 0;
        }

        if (Start == End) {
            printf("impossible to sort");
            free(NumberOfIncomingEdges);
            free(Matrix);
            return 0;
        }

        Matrix[(Start - 1) * StringLength + (End - 1) / 8] |= (128 >> ((End - 1) % 8));
        NumberOfIncomingEdges[End - 1]++;
    }

    short* SortedGraph = (short*) calloc(sizeof(short), NumberOfVertices);
    if (SortedGraph == NULL) {
        free(NumberOfIncomingEdges);
        free(Matrix);
        return EXIT_FAILURE;
    }

    for (int k = 0; k < NumberOfVertices; k++) {
        for (int i = 0; i < NumberOfVertices; i++) {
            if (NumberOfIncomingEdges[i] == 0) {
                for (int j = 0; j < NumberOfVertices; j++) {
                    if (Matrix[i * StringLength + j / 8] >> (7 - (j % 8)) & 1) {
                        NumberOfIncomingEdges[j]--;
                    }
                }
                SortedGraph[k] = (short)(i + 1);
                NumberOfIncomingEdges[i] = -1;
                break;
            }
        }
    }

    if (!SortedGraph[NumberOfVertices - 1]) {
        printf("impossible to sort");
        free(SortedGraph);
        free(NumberOfIncomingEdges);
        free(Matrix);
        return 0;
    }

    for (int i = 0; i < NumberOfVertices; i++) {
        printf("%d ", SortedGraph[i]);
    }
    free(NumberOfIncomingEdges);
    free(Matrix);
    free(SortedGraph);
}

int Read() {
    int Number = fgetc(in);
    char Sign = 1;
    if (Number == '-') {
        Sign = -1;
        Number = fgetc(in);
    }
    Number -= '0';
    int Input;
    while ((Input = fgetc(in)) != ' ' && Input != '\n' && Input != EOF) {
        Number = Number * 10 + Input - '0';
    }
    return Number * Sign;
}
