#include <stdio.h>
#include <stdlib.h>

int  ReadInt();
int* ReadIntArray(int Length);
void PrintIntArray(int* Array, int Length);
void QuickSort(int* Array, int Length);
void QS(int* Array, int l, int r);
int  Partition(int* Array, int l, int r);
void SwapInt(int* a, int* b);

int main() {
    int Length = ReadInt();
    int* Array = ReadIntArray(Length);
    if (Array == NULL) return EXIT_FAILURE;
    QuickSort(Array, Length);
    PrintIntArray(Array, Length);
    free(Array);
    return 0;
}

int  ReadInt() {
    char Sign = 1;
    int Number = fgetc(stdin);
    if (Number == '-') {
        Sign = -1;
        Number = fgetc(stdin);
    }
    Number -= '0';
    int Input;
    while ((Input = fgetc(stdin)) != ' ' && Input != '\n' && Input != EOF) {
        Number = Number * 10 + Input - '0';
    }
    return Number * Sign;
}
int* ReadIntArray(int Length) {
    int* Array = (int*) malloc(Length * sizeof(int));
    if (Array == NULL) return NULL;
    for (int i = 0; i < Length; i++) {
        Array[i] = ReadInt();
    }
    return Array;
}
void PrintIntArray(int* Array, int Length) {
    for (int i = 0; i < Length; i++) {
        printf("%d ", Array[i]);
    }
    printf("\n");
}
void QuickSort(int* Array, int Length) {
    QS(Array, 0, Length - 1);
}
void QS(int* Array, int l, int r) {
    if (l >= 0 && r >= 0 && l < r) {
        int pivot = Partition(Array, l, r);
        QS(Array, l, pivot);
        QS(Array, pivot + 1, r);
    }
}
int  Partition(int* Array, int l, int r) {
    int pivot = Array[(l + r) / 2];
    int i = l - 1;
    int j = r + 1;
    while (1) {
        do i++; while (Array[i] < pivot);
        do j--; while (Array[j] > pivot);
        if (i >= j) return j;
        SwapInt(&Array[i], &Array[j]);
    }
}
void SwapInt(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}
