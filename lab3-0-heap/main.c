#include <stdio.h>
#include <stdlib.h>

int ReadInt();
int* ReadArray(size_t Length);
void HeapSort(int* Array, size_t Length);
void MaxHeap(int* Array, size_t Length, size_t Index);
void Swap(int* a, int* b);
void PrintArray(int* Array, size_t Length);

int main() {
    size_t ArrayLength = ReadInt();
    int* Array = ReadArray(ArrayLength);
    if (Array == NULL) return EXIT_FAILURE;
    HeapSort(Array, ArrayLength);
    PrintArray(Array, ArrayLength);
    free(Array);
    return 0;
}

int ReadInt() {
    char Sign = 1;
    int Number = fgetc(stdin);
    if (Number == '-') {
        Sign = -1;
        Number = fgetc(stdin);
    }
    Number -= '0';
    int Input;
    while ((Input = fgetc(stdin)) != '\n' && Input != ' ' && Input != EOF) {
        Number = Number * 10 + Input - '0';
    }
    return Number * Sign;
}

int* ReadArray(size_t Length) {
    int* Array = (int*) malloc(Length * sizeof(int));
    if (Array == NULL) return NULL;
    for (size_t i = 0; i < Length; i++) {
        Array[i] = ReadInt();
    }
    return Array;
}

void HeapSort(int* Array, size_t Length) {
    for (size_t i = (Length / 2) + 1; i > 0; i--) {
        MaxHeap(Array, Length, i - 1);
    }
    while (Length > 0) {
        Swap(&Array[0], &Array[--Length]);
        MaxHeap(Array, Length, 0);
    }
}

void MaxHeap(int* Array, size_t Length, size_t Index) {
    if (Index * 2 + 1 >= Length) {
        return;
    }
    if (Index * 2 + 2 >= Length) {
        if (Array[Index * 2 + 1] > Array[Index]) {
            Swap(&Array[Index], &Array[Index * 2 + 1]);
            MaxHeap(Array, Length, Index * 2 + 1);
        }
    }
    else {
        if (Array[Index * 2 + 1] > Array[Index * 2 + 2]) {
            if (Array[Index * 2 + 1] > Array[Index]) {
                Swap(&Array[Index], &Array[Index * 2 + 1]);
                MaxHeap(Array, Length, Index * 2 + 1);
            }
        }
        else {
            if (Array[Index * 2 + 2] > Array[Index]) {
                Swap(&Array[Index], &Array[Index * 2 + 2]);
                MaxHeap(Array, Length, Array[Index * 2 + 2]);
            }
        }
    }
}

void Swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void PrintArray(int* Array, size_t Length) {
    for (size_t i = 0; i < Length; i++) {
        printf("%d ", Array[i]);
    }
}
