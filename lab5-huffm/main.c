#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define u_char unsigned char
#define u_int  unsigned int
#define MAX_CODE_SIZE 32

typedef _Bool bool;

typedef struct CompressTree_t {
    struct CompressTree_t* Right;
    struct CompressTree_t* Left;
    u_char* Code;
    u_int   Freq;
    u_char  Letter;
    u_char  Depth;
} CompressTree;

typedef struct PriorityQueueElement_t {
    struct PriorityQueueElement_t* Next;
    CompressTree* Tree;
} PQE;
typedef struct PriorityQueueHead_t {
    PQE* Head;
    void(*pop) (struct PriorityQueueHead_t*);
    void(*push)(struct PriorityQueueHead_t*, CompressTree*);
    CompressTree*(*top) (struct PriorityQueueHead_t*);
    size_t size;
} PriorityQueue;

void PriorityQueue_Pop(PriorityQueue* q) {
    if (q->Head == NULL) return;
    q->size--;
    PQE* ForDeleting = q->Head;
    q->Head = q->Head->Next;
    free(ForDeleting);
}
void PriorityQueue_Push(PriorityQueue* q, CompressTree* t) {
    PQE* ForPushing = (PQE*) malloc(sizeof(PQE));
    ForPushing->Tree = t;
    q->size++;
    if (q->Head == NULL || q->Head->Tree->Freq > t->Freq) {
        ForPushing->Next = q->Head;
        q->Head = ForPushing;
    } else {
        PQE* ShiftingElement = q->Head;
        while (ShiftingElement->Next != NULL && t->Freq > ShiftingElement->Next->Tree->Freq) {
            ShiftingElement = ShiftingElement->Next;
        }
        ForPushing->Next = ShiftingElement->Next;
        ShiftingElement->Next = ForPushing;
    }
}
CompressTree* PriorityQueue_Top(PriorityQueue* q) {
    return q->Head ? q->Head->Tree : NULL;
}
void PriorityQueue_Init(PriorityQueue* q) {
    q->Head = NULL;
    q->size = 0;
    q->pop  = &PriorityQueue_Pop;
    q->push = &PriorityQueue_Push;
    q->top  = &PriorityQueue_Top;
}

void Compress  (FILE* in, FILE* out, bool RandomFile);
void Decompress(FILE* in, FILE* out);

int main(int argc, char* argv[]) {
    FILE* in;
    FILE* out;
    int Mode;
    bool RandomFile = 0;
    if (argc == 1) {
        in   = stdin;
        out  = stdout;
        Mode = fgetc(in);
    }
    else {
        in   = fopen(argv[2], "r");
        out  = fopen(argv[3], "w");
        Mode = (int)argv[1][0];
        RandomFile = 1;
    }
    if (Mode == 'c') {
        Compress(in, out, RandomFile);
    }
    else if (Mode == 'd') {
        Decompress(in, out);
    }
    fclose(in);
    fclose(out);
    return 0;
}

void ResetOutputPos(u_char* Output, int* CurPos, FILE* out) {
    if (*CurPos == 8) {
        fprintf(out, "%c", *Output);
        *Output = 0;
        (*CurPos) = 0;
    }
}

void WriteByte(u_char* Output, const u_char Input[], int Start, int End, int* CurPos, FILE* out) {
    for (int i = Start; i < End; i++) {
        int Index = i % 8;
        ResetOutputPos(Output, CurPos, out);
        if ((*CurPos) >= Index) {
            *Output |= (Input[i / 8] >> ((*CurPos) - Index)) & 128 >> *CurPos;
        }
        else {
            *Output |= (Input[i / 8] << (Index - (*CurPos))) & 128 >> *CurPos;
        }
        (*CurPos)++;
    }
    ResetOutputPos(Output, CurPos, out);
}

u_int RecFillCodes(CompressTree* t, u_char CurDepth) {
    if (t->Right == NULL && t->Left == NULL) {
        t->Depth = CurDepth;
        return 8 + t->Freq * CurDepth;
    }
    for (int i = 0; i < MAX_CODE_SIZE; i++) {
        t->Right->Code[i] |= t->Code[i];
        t->Left ->Code[i] |= t->Code[i];
    }
    t->Right->Code[CurDepth / 8] |= (128 >> (CurDepth % 8));
    return 2 + RecFillCodes(t->Right, CurDepth + 1) + RecFillCodes(t->Left,  CurDepth + 1);
}
u_int FillCodes(CompressTree* t) {
    return RecFillCodes(t, 0) + 1;
}

void PrintTree(CompressTree* t, u_char* Output, int* CurPos, FILE* out) {
    if (t->Right == NULL && t->Left == NULL) {
        u_char BIT_MASK = 128;
        WriteByte(Output, &BIT_MASK, 0, 1, CurPos, out);
        WriteByte(Output, &(t->Letter), 0, 8, CurPos, out);
        return;
    }
    u_char BIT_MASK_ZERO = 0;
    WriteByte(Output, &BIT_MASK_ZERO, 0, 1, CurPos, out);
    PrintTree(t->Left, Output, CurPos, out);
    PrintTree(t->Right, Output, CurPos, out);
}

void DestroyTree(CompressTree* t) {
    if (t == NULL) {
        return;
    }
    DestroyTree(t->Right);
    DestroyTree(t->Left);
    free(t->Code);
    free(t);
}

void Compress(FILE* in, FILE* out, bool RandomFile) {
    CompressTree** Letters = (CompressTree**) calloc(sizeof(CompressTree*), UCHAR_MAX + 1);
    int Input;
    while ((Input = fgetc(in)) != EOF) {
        if (Letters[Input] == NULL) {
            Letters[Input] = (CompressTree*) calloc(sizeof(CompressTree), 1);
            Letters[Input]->Letter = Input;
            Letters[Input]->Code = (u_char*) calloc(sizeof(char), MAX_CODE_SIZE);
        }
        Letters[Input]->Freq++;
    }

    PriorityQueue q;
    PriorityQueue_Init(&q);

    for (int i = 0; i < UCHAR_MAX + 1; i++) {
        if (Letters[i] != NULL) {
            q.push(&q, Letters[i]);
        }
    }

    u_char Output = 0;
    int CurPos = 0;
    u_char BIT_MASK = 128;

    if (q.size == 0) {
        free(q.Head);
        free(Letters);
        return;
    }
    if (q.size == 1) {
        WriteByte(&Output, &BIT_MASK, 0, 1, &CurPos, out);
        CurPos = 8 - (int)((8 + q.top(&q)->Freq) % 8) - 1;
        WriteByte(&Output, &BIT_MASK, 0, 1, &CurPos, out);
        if (CurPos == 1) {
            fprintf(out, "%c", 0);
        }
        q.top(&q)->Depth += 1; // cppcheck doesn't allow using q.top(&q)->Depth = 1;
        WriteByte(&Output, &(q.top(&q)->Letter), 0, 8, &CurPos, out);
    }
    else {
        while (q.size > 1) {
            CompressTree *Buf = (CompressTree *) calloc(sizeof(CompressTree), 1);
            Buf->Code = (u_char *) calloc(sizeof(u_char), MAX_CODE_SIZE);
            Buf->Right = q.top(&q);
            q.pop(&q);
            Buf->Left = q.top(&q);
            q.pop(&q);
            Buf->Freq = Buf->Right->Freq + Buf->Left->Freq;
            q.push(&q, Buf);
        }
        CurPos = 8 - (int)(FillCodes(q.top(&q)) % 8) - 1;
        WriteByte(&Output, &BIT_MASK, 0, 1, &CurPos, out);
        if (CurPos == 1) {
            fprintf(out, "%c", 0);
        }
        PrintTree(q.top(&q), &Output, &CurPos, out);
    }

    fseek(in, 1 - RandomFile, SEEK_SET);

    while ((Input = fgetc(in)) != EOF) {
        WriteByte(&Output, Letters[Input]->Code, 0, Letters[Input]->Depth, &CurPos, out);
    }
    DestroyTree(q.Head->Tree);
    free(q.Head);
    free(Letters);
}

void ResetInputPos(int* Input, int* CurPos, FILE* in) {
    if (*CurPos == 8) {
        *CurPos = 0;
        *Input = fgetc(in);
    }
}

void WriteLetter(u_char* Letter, int* Input, int Start, int End, int* CurPos, FILE* in) {
    ResetInputPos(Input, CurPos, in);
    for (int i = Start; i < End; i++) {
        if (*CurPos >= i) {
            *Letter |= (*Input << (*CurPos - i)) & (128 >> i);
        }
        else {
            *Letter |= (*Input >> (i - *CurPos)) & (128 >> i);
        }
        (*CurPos)++;
        ResetInputPos(Input, CurPos, in);
    }
}

void MakeDecompressTree(CompressTree* Tree, int* Input, int* pos, FILE* in) {
    if ((*Input >> (7 - *pos)) & 1) {
        (*pos)++;
        WriteLetter(&(Tree->Letter), Input, 0, 8, pos, in);
    }
    else {
        Tree->Left  = (CompressTree*) calloc(sizeof(CompressTree), 1);
        Tree->Right = (CompressTree*) calloc(sizeof(CompressTree), 1);
        (*pos)++;
        ResetInputPos(Input, pos, in);
        MakeDecompressTree(Tree->Left,  Input, pos, in);
        MakeDecompressTree(Tree->Right, Input, pos, in);
    }
}

void Decompress(FILE* in, FILE* out) {
    CompressTree* Tree = (CompressTree*) calloc(sizeof(CompressTree), 1);
    int CurPos = 0;
    int Input = fgetc(in);
    if (Input == EOF) {
        free(Tree);
        return;
    }
    if ((Input >> 7) & 1) {
        CurPos++;
        while (!((Input >> (7 - CurPos)) & 1)) {
            CurPos++;
            ResetInputPos(&Input, &CurPos, in);
        }
        CurPos++;
        Tree->Left = (CompressTree*) calloc(sizeof(CompressTree), 1);
        WriteLetter(&(Tree->Left->Letter), &Input, 0, 8, &CurPos, in);
    }
    else {
        while (!((Input >> (7 - CurPos)) & 1)) {
            CurPos++;
            ResetInputPos(&Input, &CurPos, in);
        }
        CurPos++;
        ResetInputPos(&Input, &CurPos, in);
        MakeDecompressTree(Tree, &Input, &CurPos, in);
    }

    CompressTree* Buf = Tree;
    for (int i = CurPos; i < 8; i++) {
        if ((Input >> (7 - i)) & 1) {
            Buf = Buf->Right;
        }
        else {
            Buf = Buf->Left;
        }
        if (Buf->Right == NULL && Buf->Left == NULL) {
            fprintf(out, "%c", Buf->Letter);
            Buf = Tree;
        }
    }
    while ((Input = fgetc(in)) != EOF) {
        for (int i = 0; i < 8; i++) {
            if ((Input >> (7 - i)) & 1) {
                Buf = Buf->Right;
            }
            else {
                Buf = Buf->Left;
            }
            if (Buf->Right == NULL && Buf->Left == NULL) {
                fprintf(out, "%c", Buf->Letter);
                Buf = Tree;
            }
        }
    }
    DestroyTree(Tree);
}
