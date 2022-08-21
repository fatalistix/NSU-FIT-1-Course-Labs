#include <stdio.h>

#define array_size 16

int readSample(int* array) {
    int i = 0;
    int iInput;
    while ((iInput = fgetc(stdin)) != '\n') {
        array[i++] = iInput;
    }
    return i;
}

int read(int* array, int start, int len) {
    int i = start;
    int iInput;
    while (i < len && (iInput = fgetc(stdin)) != '\n' && iInput != EOF) {
        array[i++] = iInput;
    }
    if (iInput == EOF) {
        return 0;
    }
    return i;
}

int powInt(int a, int b) {
    return ((b == 0) ? 1 : ((b % 2 == 0) ? powInt(a * a, b >> 1) : a * powInt(a, b - 1)));
}

int fastHashing(int const iArray[], int iStart, int iEnd) {
    int iAns = 0;
    for (int i = iStart; i < iEnd; i++) {
        iAns += (iArray[i] % 3) * powInt(3, i - iStart);
    }
    return iAns;
}

void shift(int* array, int len) {
    for (int i = 1; i < len; i++) {
        array[i - 1] = array[i];
    }
}

int main() {
    int iArrSample[array_size];
    int iArrText[array_size];
    int iLength = readSample(iArrSample);
    int iPassed = 0;
    int iStart = 0;
    int iHashSample = fastHashing(iArrSample, 0, iLength);
    int iHash = 0;
    int p = powInt(3, iLength - 1);
    printf("%d ", iHashSample);
    do {
        int condition = read(iArrText, iStart, iLength);
        if (condition == 0) {
            break;
        }
        else if (condition < iLength) {
            iStart = 0;
            iPassed += condition + 1;
            continue;
        }
        else if (iStart == 0) {
            iHash = fastHashing(iArrText, 0, iLength);
            iStart = iLength - 1;
            iPassed += iStart;
        }
        else {
            iHash += (iArrText[iLength - 1] % 3) * p;
            iPassed++;
        }
        if (iHash == iHashSample) {
            for (int j = 0; j < iLength; j++) {
                printf("%d ", iPassed - iLength + j + 1 + 1);
                if (iArrText[j] != iArrSample[j]) {
                    break;
                }
            }
        }
        
        iHash -= iArrText[0] % 3;
        iHash /= 3;
        shift(iArrText, iLength);
    } while (1);
    return 0;
}
