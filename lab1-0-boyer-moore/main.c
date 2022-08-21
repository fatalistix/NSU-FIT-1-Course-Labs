#include <stdio.h>

#define array_size 17
#define ascii_size 256
#define bool _Bool

int readSample(int* array) {
    int iInput;
    int i = 0;
    while ((iInput = getchar()) != '\n') {
        array[i++] = iInput;
    }
    return i;
}

int read(int* array, int start, int len) {
    int i = start;
    int iInput;
    while (i < len && (iInput = fgetc(stdin)) != EOF) {
        array[i++] = iInput;
    }
    if (iInput == EOF) {
        return 0;
    }
    return i;
}

int algorithm(int const iSampleArray[], int const iTextArray[], int const iTableArray[], int iLen, int iLPassed) {
    bool bLetterFound = 0;

    for (int i = iLen - 1; i >= 0; i--) {
        printf("%d ", iLPassed + i + 1);
        if (iSampleArray[i] != iTextArray[i]) {
            if (!bLetterFound) {
                return iTableArray[iTextArray[i]];
            }
            else {
                return iTableArray[iSampleArray[iLen - 1]];
            }
        }
        else {
            bLetterFound = 1;
        }
    }
    return iTableArray[iSampleArray[iLen - 1]];
}

void shift(int* array, int iLShift, int len) {
    for (int i = 0; i < len - iLShift; i++) {
        array[i] = array[i + iLShift];
    }
}

int main() {
    int iArrSample[array_size];
    int iArrText[array_size];
    int iLength = readSample(iArrSample);
    int iPassed = 0;
    int iStart = 0;
    int iArrTable[ascii_size];
    for (int i = 0; i < ascii_size; i++) {
        iArrTable[i] = iLength;
    }
    for (int i = iLength - 2; i >= 0; i--) {
        if (iArrTable[iArrSample[i]] == iLength) {
            iArrTable[iArrSample[i]] = iLength - i - 1;
        }
    }
    do {
        int condition = read(iArrText, iStart, iLength);
        if (condition == 0) {
            break;
        }
        else {
            int iShift = algorithm(iArrSample, iArrText, iArrTable, iLength, iPassed);
            iStart = iLength - iShift;
            iPassed += iShift;
            shift(iArrText, iShift, iLength);
        }
    } while (1);
    return 0;
}
