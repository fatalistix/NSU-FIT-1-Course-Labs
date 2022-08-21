#include <stdio.h>

#define bool _Bool

#define max_array_size 11

int read();
int readLine(char* string);
void swap(char* a, char* b);
bool checkSymbols(char const string[], int len);
void nextPermutation(char* string, int len);

int main() {
    char szString[max_array_size];
    int iLength = readLine(szString);
    if (iLength == 11) {
        printf("bad input");
        return 0;
    }
    szString[iLength] = '\0';
    if (checkSymbols(szString, iLength)) {
        printf("bad input");
        return 0;
    }
    int nPermutations = read();
    for (int i = 0; i < nPermutations; i++) {
        nextPermutation(szString, iLength);
    }
    return 0;
}

int read() {
    int iNumber = fgetc(stdin) - '0';
    int iInput;
    while ((iInput = fgetc(stdin)) != EOF && iInput != '\n' && iInput != ' ') {
        iNumber = iNumber * 10 + iInput;
    }
    return iNumber;
}

int readLine(char* string) {
    char symbol;
    int i = 0;
    while (i < max_array_size && (symbol = (char)fgetc(stdin)) != EOF && symbol != '\n') {
        string[i++] = symbol;
    }
    return i;
}

void swap(char *a, char * b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

bool checkSymbols(char const string[], int len) {
    bool table[10] = {0};
    for (int i = 0; i < len; i++) {
        if (string[i] < '0' || string[i] > '9') {
            return 1;
        }
        if (table[string[i] - '0']) {
            return 1;
        }
        table[string[i] - '0'] = 1;
    }
    return 0;
}

void nextPermutation(char* Number, int Length) {
    for (int i = Length - 1; i > 0; i--) {
        if (Number[i] > Number[i-1]) {
            int p = i;
            for (int j = i; j < Length; j++) {
                if (Number[j] > Number[i-1]) {
                    p = j;
                }
            }
            swap(&Number[i-1], &Number[p]);
            for (int j = i, k = Length - 1; j < k; j++, k--) {
                swap(&Number[j], &Number[k]);
            }
            printf("%s\n", Number);
            break;
        }
    }
}

