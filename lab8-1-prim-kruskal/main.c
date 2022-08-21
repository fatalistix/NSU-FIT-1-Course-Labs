#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MIN(a, b) ((a > b) ? b : a)
#define MAX(a, b) ((a > b) ? a : b)

typedef struct Graph_t
{
    short Start;
    short End;
} Graph;

int ReadInt();
long long ReadLongLong();

char BadGraphInput(int NumOfVert, int NumOfEdges)
{
    if (NumOfVert == EOF - '0' || NumOfEdges == EOF - '0')
    {
        printf("bad number of lines");
        return 1;
    }
    if (NumOfVert < 0 || NumOfVert > 5000)
    {
        printf("bad number of vertices");
        return 1;
    }
    if (NumOfEdges < 0 || NumOfEdges > NumOfVert * (NumOfVert + 1) / 2)
    {
        printf("bad number of edges");
        return 1;
    }
    if (NumOfVert == 0)
    {
        printf("no spanning tree");
        return 1;
    }
    if (NumOfVert == 1 && NumOfEdges == 0)
    {
        return 1;
    }
    return 0;
}
char BadGraphEdge(int Start, int End, long long Length, int NumOfVert)
{
    if (Start == EOF - '0')
    {
        printf("bad number of lines");
        return 1;
    }
    if (Start < 1 || Start > NumOfVert || End < 1 || End > NumOfVert)
    {
        printf("bad vertex");
        return 1;
    }
    if (Length < 0 || Length > INT_MAX)
    {
        printf("bad length");
        return 1;
    }
    return 0;
}
char NotMinimalSpanningTree(Graph* Graph, int NumOfVert)
{
    if (Graph[NumOfVert - 2].Start == 0)
    {
        printf("no spanning tree");
        return 1;
    }
    return 0;
}

void FillArray(int* Array, int Length, int Value)
{
    for (int i = 0; i < Length; i++)
    {
        Array[i] = Value;
    }
}

char PrimAlgo(const int Matrix[], int NumOfVert, Graph* SortedGraph)
{
    char* InUse = (char*) calloc(sizeof(char), NumOfVert);
    if (InUse == NULL)
    {
        return 0;
    }

    int*  MinEdge = (int*) malloc(sizeof(int) * NumOfVert);
    if (MinEdge == NULL)
    {
        free(InUse);
        return 0;
    }
    FillArray(MinEdge, NumOfVert, INT_MIN);
    MinEdge[0] = 0;

    int*  EndEdge = (int*)  malloc(sizeof(int) * NumOfVert);
    if (EndEdge == NULL)
    {
        free(InUse);
        free(MinEdge);
        return 0;
    }
    FillArray(EndEdge, NumOfVert, -1);

    short s = 0;

    for (int i = 0; i < NumOfVert; i++)
    {
        int Vertex = -1;
        for (int j = 0; j < NumOfVert; j++)
        {
            if (!InUse[j] && (Vertex == -1 || MinEdge[j] > MinEdge[Vertex]))
            {
                Vertex = j;
            }
        }
        InUse[Vertex] = 1;
        if (EndEdge[Vertex] != -1)
        {
            SortedGraph[s].Start = (short) (Vertex + 1);
            SortedGraph[s++].End = (short) (EndEdge[Vertex] + 1);
        }
        for (int to = 0; to < NumOfVert; to++)
        {
            if (Matrix[Vertex * NumOfVert + to] > MinEdge[to])
            {
                MinEdge[to] = Matrix[Vertex * NumOfVert + to];
                EndEdge[to] = Vertex;
            }
        }
    }

    free(InUse);
    free(MinEdge);
    free(EndEdge);
    return 1;
}

void PrintGraph(Graph* Graph, int NumOfEdges)
{
    for (int i = 0; i < NumOfEdges; i++)
    {
        printf("%d %d\n", MIN(Graph[i].Start, Graph[i].End), MAX(Graph[i].Start, Graph[i].End));
    }
}

int main()
{
    int NumberOfVertices = ReadInt();
    int NumberOfEdges    = ReadInt();
    if (BadGraphInput(NumberOfVertices, NumberOfEdges))
    {
        return 0;
    }

    int* Matrix = (int*) malloc(NumberOfVertices * NumberOfVertices * sizeof(int));

    if (Matrix == NULL)
    {
        return EXIT_FAILURE;
    }

    FillArray(Matrix, NumberOfVertices * NumberOfVertices, INT_MIN);

    for (int i = 0; i < NumberOfEdges; i++)
    {
        int Start        = ReadInt();
        int End          = ReadInt();
        long long Length = ReadLongLong();

        if (BadGraphEdge(Start, End, Length, NumberOfVertices))
        {
            free(Matrix);
            return 0;
        }

        Matrix[(Start - 1) * NumberOfVertices + End   - 1] = (int) -Length;
        Matrix[(End   - 1) * NumberOfVertices + Start - 1] = (int) -Length;
    }

    Graph* SortedGraph = (Graph*) calloc(sizeof(Graph), NumberOfVertices - 1);

    if (SortedGraph == NULL)
    {
        free(Matrix);
        return EXIT_FAILURE;
    }

    if (!PrimAlgo(Matrix, NumberOfVertices, SortedGraph))
    {
        free(Matrix);
        free(SortedGraph);
        return EXIT_FAILURE;
    }

    if (NotMinimalSpanningTree(SortedGraph, NumberOfVertices))
    {
        free(Matrix);
        free(SortedGraph);
        return 0;
    }

    PrintGraph(SortedGraph, NumberOfVertices - 1);

    free(Matrix);
    free(SortedGraph);

    return 0;
}

int ReadInt()
{
    int Number = fgetc(stdin);
    char Sign = 1;
    if (Number == '-')
    {
        Sign = -1;
        Number = fgetc(stdin);
    }
    Number -= '0';
    int Input;
    while ((Input = fgetc(stdin)) != ' ' && Input != '\n' && Input != EOF)
    {
        Number = Number * 10 + Input - '0';
    }
    return Number * Sign;
}
long long ReadLongLong()
{
    long long Number = fgetc(stdin);
    char Sign = 1;
    if (Number == '-')
    {
        Sign = -1;
        Number = fgetc(stdin);
    }
    Number -= '0';
    int Input;
    while ((Input = fgetc(stdin)) != ' ' && Input != '\n' && Input != EOF)
    {
        Number = Number * 10 + Input - '0';
    }
    return Number * Sign;
}
