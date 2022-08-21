#include <stdio.h>

#define Max(a, b) ((a > b) ? a : b)
#define Min(a, b) ((a > b) ? b : a)

#define NUMBER_LENGTH 53
#define BASE2_MAX_INT_LENGTH 53
#define OUTPUT_DOUBLE_PRECISION 12

typedef _Bool bool;

int ReadInt() {
    int Sign = 1;
    int Number = fgetc(stdin);
    if (Number == '-') {
        Sign *= -1;
        Number = fgetc(stdin);
    }
    Number -= '0';
    int InputChar;
    while ((InputChar = fgetc(stdin)) != ' ' && InputChar != EOF && InputChar != '\n') {
        Number = Number * 10 + InputChar - '0';
    }
    return Number * Sign;
}

bool CorrectBase(int Base) {
    return (Base >= 2 && Base <= 16);
}

int ReadLn(char* String) {
    int Length = 0;
    int InputChar;
    while ((InputChar = fgetc(stdin)) != '\n' && InputChar != EOF) {
        String[Length++] = (char)InputChar;
    }
    return Length;
}

void ToNormalForm(char* Number, int Length) {
    for (int i = 0; i < Length; i++) {
        if (Number[i] >= 'A' && Number[i] <= 'F') {
            Number[i] += - 'A' + '9' + 1;
        }
        if (Number[i] >= 'a' && Number[i] <= 'f') {
            Number[i] += - 'a' + '9' + 1;
        }
    }
}

bool CorrectNumberInBase(const char Number[], int Length, int Base) {
    if (Number[0] == '.' || Number[Length - 1] == '.') {
        return 0;
    }
    int MaxDigit = 0;
    short PointsCount = 0;
    for (int i = 0; i < Length; i++) {
        if (Number[i] == '.') {
            PointsCount++;
        }
        else if (!(Number[i] >= '0' && Number[i] <= '0' + 15)) {
            return 0;
        }
        MaxDigit = Max(MaxDigit, Number[i] - '0');
    }
    if (PointsCount > 1 || MaxDigit >= Base) {
        return 0;
    }
    return 1;
}

void ConvertFromBase1ToBase2(char* Number, int Length, int Base1, int Base2) {
    int PointPlace = Length + 1;
    for (int i = 0; i < Length; i++) {
        if (Number[i] == '.') {
            PointPlace = i;
        }
    }
    long long IntPartOfNumber = (int)Number[0] - '0';
    long double DoublePartOfNumber = ((PointPlace < Length) ? (double)(Number[Length - 1] - '0') / Base1 : 0);

    for (int i = 1; i < Min(PointPlace, Length); i++) {
        IntPartOfNumber = IntPartOfNumber * Base1 + Number[i] - '0';
    }

    for (int i = Length - 2; i > PointPlace; i--) {
        DoublePartOfNumber = DoublePartOfNumber / Base1 + (double)(Number[i] - '0') / Base1;
    }

    char BufIntInBase2[BASE2_MAX_INT_LENGTH];
    int BufIntInBase2Length = 0;

    if (IntPartOfNumber == 0) {
        BufIntInBase2[0] = '0';
        BufIntInBase2Length++;
    }

    else {
        for (; IntPartOfNumber > 0; BufIntInBase2Length++) {
            BufIntInBase2[BufIntInBase2Length] = (char) ((IntPartOfNumber % Base2) + '0');
            IntPartOfNumber /= Base2;
        }
    }

    int NumberBase2Length = 0;

    for (int i = BufIntInBase2Length - 1; i >= 0; i--) {
        if (BufIntInBase2[i] >= '0' && BufIntInBase2[i] <= '9') {
            Number[NumberBase2Length++] = BufIntInBase2[i];
        }
        else {
            Number[NumberBase2Length++] = (char)(BufIntInBase2[i] - '9' - 1 + 'A');
        }
    }

    if (PointPlace < Length && DoublePartOfNumber != 0) {
        Number[NumberBase2Length++] = '.';
        int DoubleBase2Length = 0;
        while (DoublePartOfNumber != 0 && DoubleBase2Length != OUTPUT_DOUBLE_PRECISION) {
            DoublePartOfNumber *= (double)(Base2);
            int IntPartOfDouble = (int)(DoublePartOfNumber);
            if (IntPartOfDouble >= 0 && IntPartOfDouble <= 9) {
                Number[NumberBase2Length++] = (char)(IntPartOfDouble + '0');
            }
            else {
                Number[NumberBase2Length++] = (char)(IntPartOfDouble + 'A' - 10);
            }
            DoublePartOfNumber -= (double)(IntPartOfDouble);
            DoubleBase2Length++;
        }
    }
    Number[NumberBase2Length] = '\0';

}

int main() {
    int Base1 = ReadInt();
    if (!CorrectBase(Base1)) {
        printf("bad input");
        return 0;
    }

    int Base2 = ReadInt();
    if (!CorrectBase(Base2)) {
        printf("bad input");
        return 0;
    }

    char Number[NUMBER_LENGTH];
    int NumberLength = ReadLn(Number);
    Number[NumberLength] = '\0';
    ToNormalForm(Number, NumberLength);

    if (!CorrectNumberInBase(Number, NumberLength, Base1)) {
        printf("bad input");
        return 0;
    }

    ConvertFromBase1ToBase2(Number, NumberLength, Base1, Base2);

    printf("%s", Number);

    return 0;
}
