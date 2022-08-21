#include <stdio.h>
#include <stdlib.h>

#define SAMPLE_MAX_SIZE  16
#define SHIFT_TABLE_SIZE 256

typedef unsigned char u_char;

typedef struct DLList_t {
    struct DLList_t* Right;
    struct DLList_t* Left;
    u_char Value;
} DLList;

int     ReadLine(u_char* Sample);
void    FillShiftTable(u_char* Table, const u_char Sample[], int Length);
DLList* MakeCyclicalList(int Length);
void    BoyerMooreAlgorithm(DLList* Buffer, const u_char Sample[], const u_char Table[], int Length);
void    FreeBuffer(DLList* Buffer, int Length);

int main() {
    u_char Sample[SAMPLE_MAX_SIZE];
    int Length = ReadLine(Sample);
    u_char ShiftTable[SHIFT_TABLE_SIZE];
    FillShiftTable(ShiftTable, Sample, Length);
    DLList* Buffer = MakeCyclicalList(Length);
    BoyerMooreAlgorithm(Buffer, Sample, ShiftTable, Length);
    FreeBuffer(Buffer, Length);
    return 0;
}

int ReadLine(u_char* Sample) {
    int Input;
    int i = 0;
    while ((Input = fgetc(stdin)) != '\n' && Input != EOF) {
        Sample[i++] = (u_char)Input;
    }
    return i;
}
void FillShiftTable(u_char* Table, const u_char Sample[], int Length) {
    for (int i = 0; i < SHIFT_TABLE_SIZE; i++) {
        Table[i] = Length;
    }
    for (int i = Length - 2; i >= 0; i--) {
        if (Table[Sample[i]] == Length) {
            Table[Sample[i]] =  Length - 1 - i;
        }
    }
}
DLList* MakeElement() {
    return (DLList*) calloc(1, sizeof(DLList));
}
DLList* MakeCyclicalList(int Length) {
    DLList* l = MakeElement();
    DLList* Start = l;
    for (int i = 1; i < Length; i++) {
        DLList* Prev = l;
        l->Right = MakeElement();
        l = l->Right;
        l->Left = Prev;
    }
    l->Right = Start;
    Start->Left = l;
    return Start;
}
void BoyerMooreAlgorithm(DLList* Buffer, const u_char Sample[], const u_char Table[], int Length) {
    int Passed = 0;
    int Read = Length;
    do {
        for (int i = 0; i < Read; i++) {
            int Input = fgetc(stdin);
            if (Input == EOF) return;
            Buffer->Value = (u_char)Input;
            Buffer = Buffer->Right;
        }

        _Bool Coincided = 0;
        DLList* l = Buffer->Left;

        for (int i = Length - 1; i >= 0; i--) {
            printf("%d ", Passed + i + 1);
            if (Sample[i] != l->Value) {
                if (!Coincided) {
                    Read = Table[l->Value];
                    break;
                }
                else {
                    Read = Table[Sample[Length - 1]];
                    break;
                }
            }
            else {
                Coincided = 1;
                l = l->Left;
            }
            if (i == 0) {
                Read = Length;
            }
        }
        Passed += Read;
    } while(1);
}
void FreeBuffer(DLList* Buffer, int Length) {
    for (int i = 0; i < Length; i++) {
        DLList* buf = Buffer;
        Buffer = Buffer->Right;
        free(buf);
    }
}
