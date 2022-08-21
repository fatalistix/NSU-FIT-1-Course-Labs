#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MIN(a, b) ((a > b) ? b : a)
#define MAX(a, b) ((a > b) ? a : b)

typedef struct GraphEdge_t
{
    int   Length;
    short Start;
    short End;
} GraphEdge;

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
char NotMinimalSpanningTree(GraphEdge* Graph, int NumOfVert)
{
    if (Graph[NumOfVert - 2].Start == 0)
    {
        printf("no spanning tree");
        return 1;
    }
    return 0;
}

int cmp(GraphEdge *a, GraphEdge* b)
{
    return b->Length < a->Length;
}

char KruskalAlgo(GraphEdge* Graph, GraphEdge* SortedGraph, int NumOfVert, int NumOfEdges)
{
    qsort(Graph, NumOfEdges, sizeof(GraphEdge), (int(*) (const void*, const void*)) cmp);

    short* TreeID = (short*) malloc(sizeof(short) * NumOfVert);

    if (TreeID == NULL)
    {
        return 0;
    }

    for (int i = 0; i < NumOfVert; i++)
    {
        TreeID[i] = (short) i;
    }

    short s = 0;

    for (int i = 0; i < NumOfEdges; i++)
    {
        short Start  = Graph[i].Start;
        short End    = Graph[i].End;
        if (TreeID[Start - 1] != TreeID[End - 1])
        {
            SortedGraph[s].Start = Start;
            SortedGraph[s++].End = End;

            short NewID = TreeID[Start - 1];
            short OldID = TreeID[End   - 1];

            for (int j = 0; j < NumOfVert; j++)
            {
                if (TreeID[j] == OldID)
                {
                    TreeID[j] = NewID;
                }
            }
        }
    }

    for (int i = 1; i < NumOfVert; i++)
    {
        if (TreeID[i] != TreeID[0])
        {
            SortedGraph[NumOfVert - 2].Start = 0;
            break;
        }
    }

    free(TreeID);
    return 1;
}

void PrintGraph(GraphEdge* Graph, int NumOfEdges)
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
    GraphEdge* Graph = (GraphEdge*) malloc(sizeof(GraphEdge) * NumberOfEdges);

    if (Graph == NULL)
    {
        return EXIT_FAILURE;
    }

    for (int i = 0; i < NumberOfEdges; i++)
    {
        int Start        = ReadInt();
        int End          = ReadInt();
        long long Length = ReadLongLong();
        if (BadGraphEdge(Start, End, Length, NumberOfVertices))
        {
            free(Graph);
            return 0;
        }
        Graph[i].Start  = (short) Start;
        Graph[i].End    = (short) End;
        Graph[i].Length = (int)   Length;
    }

    GraphEdge* SortedGraph = (GraphEdge*) calloc(sizeof(GraphEdge), NumberOfVertices - 1);

    if (SortedGraph == NULL)
    {
        free(Graph);
        return EXIT_FAILURE;
    }

    if (!KruskalAlgo(Graph, SortedGraph, NumberOfVertices, NumberOfEdges))
    {
        free(Graph);
        free(SortedGraph);
        return EXIT_FAILURE;
    }

    if (NotMinimalSpanningTree(SortedGraph, NumberOfVertices))
    {
        free(Graph);
        free(SortedGraph);
        return 0;
    }

    PrintGraph(SortedGraph, NumberOfVertices - 1);

    free(Graph);
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
