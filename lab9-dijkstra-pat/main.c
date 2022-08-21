#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef unsigned short u_short;

int ReadInt();
long long ReadLongLong();

int BadGraphVertex(int Vertex, int NumOfVert)
{
    if (Vertex < 1 || Vertex > NumOfVert)
    {
        printf("bad vertex");
        return 1;
    }
    return 0;
}

int BadGraphInput(int NumOfVert, int Start, int Finish, int NumOfEdges)
{
    if (NumOfVert == EOF - '0' || Start == EOF - '0' || NumOfEdges == EOF - '0')
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
    return BadGraphVertex(Start, NumOfVert) || BadGraphVertex(Finish, NumOfVert);
}

int BadGraphEdge(int Start, int End, long long Length, int NumOfVert)
{
    if (Start == EOF - '0')
    {
        printf("bad number of lines");
        return 1;
    }
    if (Length < 0 || Length > INT_MAX)
    {
        printf("bad length");
        return 1;
    }
    return BadGraphVertex(Start, NumOfVert) || BadGraphVertex(End, NumOfVert);
}

void FillIntArray(int* Array, int Length, int Value)
{
    for (int i = 0; i < Length; i++)
    {
        Array[i] = Value;
    }
}

void FillLongLongArray(long long* Array, int Length, long long Value)
{
    for (int i = 0; i < Length; i++)
    {
        Array[i] = Value;
    }
}

void PrintGraphLengths(const long long Lengths[], int NumOfVert)
{
    for (int i = 0; i < NumOfVert; i++)
    {
        if (Lengths[i] == LLONG_MIN)
        {
            printf("oo ");
        }
        else if (Lengths[i] <= INT_MIN)
        {
            printf("INT_MAX+ ");
        }
        else
        {
            printf("%lld ", -Lengths[i]);
        }
    }
    printf("\n");
}

void PrintGraphWay(const short Prev[], int Start, int Finish, const long long Dist[], const u_short Counter[])
{
    if (Prev[Finish - 1] == 0 && Start != Finish)
    {
        printf("no path");
    }
    else if (Counter[Finish - 1] > 1 && Dist[Finish - 1] <= INT_MIN)
    {
        printf("overflow");
    }

    else
    {
        short Vertex = (short) (Finish);
        //printf("Vertex = %d\n", Vertex);
        while (Prev[Vertex - 1] != 0) {
            //printf("Prev[%d] = %d\n", Vertex - 1, Prev[Vertex-1]);
            printf("%d ", Vertex);
            Vertex = Prev[Vertex - 1];
        }
        printf("%d", Vertex);
    }
}

void DijkstraAlgo(const int Matrix[], int NumOfVert, int Start, int Finish)
{
    char*      Used      = (char*)      calloc(sizeof(char),     NumOfVert);
    long long* Distances = (long long*) calloc(sizeof(long long),NumOfVert);
    short*     Previous  = (short*)     calloc(sizeof(short),    NumOfVert);
    u_short*   Counter   = (u_short*)   calloc(sizeof(u_short),  NumOfVert);
    FillLongLongArray(Distances, NumOfVert, LLONG_MIN);

    int MinVertex = Start - 1;
    long long MinLabel = LLONG_MIN;
    Distances[Start - 1] = 0;

    for (int i = 0; i < NumOfVert; i++)
    {
        if (Matrix[(Start - 1) * NumOfVert + i] != INT_MIN)
        {
            Counter[Start - 1]++;
        }
    }

    for (int k = 0; k < NumOfVert; k++)
    {
        for (int i = 0; i < NumOfVert; i++)
        {
            if (!Used[i] && Distances[i] > MinLabel)
            {
                MinVertex = i;
                MinLabel = Distances[i];
            }
        }

        for (int i = 0; i < NumOfVert; i++)
        {
            if (Matrix[MinVertex * NumOfVert + i] > INT_MIN && MinVertex != i)
            {

                if (Distances[i] < Distances[MinVertex] + Matrix[MinVertex * NumOfVert + i])
                {
                    Distances[i] = Distances[MinVertex] + Matrix[MinVertex * NumOfVert + i];
                    Previous[i] = (short) (MinVertex + 1);
                    Counter[i] += Counter[MinVertex];

                }

            }
        }
        //for (int i = 0; i < NumOfVert; i++)
        //{
        //    printf("%lld ", Distances[i]);

        //}
        //printf("\n%d\n", k);
        Used[MinVertex] = 1;
        MinLabel = LLONG_MIN;
    }
    //for (int l = 0; l < NumOfVert; l++)
    //{
    //    printf("%d ", Counter[l]);
    //}
    //printf("\n");
    PrintGraphLengths(Distances, NumOfVert);
    PrintGraphWay(Previous, Start, Finish, Distances, Counter);

    free(Used);
    free(Distances);
    free(Previous);
    free(Counter);
}

int main()
{
    //fprintf(stderr, "%d\n", INT_MAX);
    int NumberOfVertices = ReadInt();
    int StartVertex      = ReadInt();
    int FinishVertex     = ReadInt();
    int NumberOfEdges    = ReadInt();

    if (BadGraphInput(NumberOfVertices, StartVertex, FinishVertex, NumberOfEdges))
    {
        return 0;
    }

    int* Matrix = (int*) malloc(NumberOfVertices * NumberOfVertices * sizeof(int));

    if (Matrix == NULL)
    {
        return EXIT_FAILURE;
    }

    FillIntArray(Matrix, NumberOfVertices * NumberOfVertices, INT_MIN);

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

        Matrix[(Start - 1) * NumberOfVertices + End   - 1] = (int) - Length;
        Matrix[(End   - 1) * NumberOfVertices + Start - 1] = (int) - Length;
    }
    //for (int i = 0; i < NumberOfVertices; i++)
    //{
    //    for (int j = 0; j < NumberOfVertices; j++)
    //    {
    //        printf("%d ", Matrix[i * NumberOfVertices + j]);
    //    }
    //    printf("\n");
    //}
    DijkstraAlgo(Matrix, NumberOfVertices, StartVertex, FinishVertex);

    free(Matrix);
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
