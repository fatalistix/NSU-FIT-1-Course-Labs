#include <stdio.h>

#define max_size 1000

typedef _Bool bool;

typedef struct stack stack;

void delete_last(stack* st);
bool checkEmpty(stack* st);
double lastElement(stack* st);
void pushBack(stack* st, double symbol);

void activateStack(stack* st);

struct stack {
    double arr[max_size];
    int size;
    void(*pop)(stack*);
    bool(*empty)(stack*);
    double(*top)(stack*);
    void(*push)(stack*, double);
};

bool calculate(stack* numbers, stack* op);

int main() {
    stack operands;
    stack operations;
    activateStack(&operands);
    activateStack(&operations);
    int iInput;
    int prev = -(int)1e9;
    int cntBrackets = 0;
    int number = -(int)1e9;
    while ((iInput = fgetc(stdin)) != EOF && iInput != '\n') {
        if (iInput >= '0' && iInput <= '9') {
            if (number == -(int)1e9) {
                number = iInput - '0';
            }
            else {
                number = number * 10 + iInput - '0';
            }
        }
        else {
            if (number != -(int)1e9) {
                operands.push(&operands, number);
                number = -(int)1e9;
            }
            if (iInput == '*' || iInput == '/') {
                if (prev < '0' && prev != ')') {
                    printf("syntax error");
                    return 0;
                }
                if (!operations.empty(&operations) && (operations.top(&operations) == '*' || operations.top(&operations) == '/')) {
                    if(!calculate(&operands, &operations)) {
                        printf("division by zero");
                        return 0;
                    }
                }
                operations.push(&operations, iInput);
            }
            else if (iInput == '+' || iInput == '-') {
                if (prev < '0' && prev != ')') {
                    printf("syntax error");
                    return 0;
                }
                if (!operations.empty(&operations) && operations.top(&operations) != '(') {
                    if (!calculate(&operands, &operations)) {
                        printf("division by zero");
                        return 0;
                    }
                }
                operations.push(&operations, iInput);
            }
            else if (iInput == '(') {
                if (prev >= '0' && prev <= '9') {
                    printf("syntax error");
                    return 0;
                }
                operations.push(&operations, iInput);
                cntBrackets++;
            }
            else if (iInput == ')') {
                if (cntBrackets < 1) {
                    printf("syntax error");
                    return 0;
                }
                if (prev == '(') {
                    printf("syntax error");
                    return 0;
                }
                while (operations.top(&operations) != '(') {
                    calculate(&operands, &operations);
                }
                operations.pop(&operations);
                cntBrackets--;
            }
            else {
                printf("syntax error");
                return 0;
            }
        }
        prev = iInput;
    }
    if (number != -(int)1e9) {
        operands.push(&operands, number);
    }
    if (cntBrackets != 0 || prev == '-' || prev == '+' || prev == '*' || prev == '/' || operands.empty(&operands)) {
        printf("syntax error");
        return 0;
    }
    while (!operations.empty(&operations)) {
        if (!calculate(&operands, &operations)) {
            printf("division by zero");
            return 0;
        }
    }
    printf("%ld", (long)(operands.top(&operands)));
    return 0;
}

void delete_last(stack* st) {
    st->size--;
}
bool checkEmpty(stack* st) {
    return !((bool)(st->size));
}
double lastElement(stack* st) {
    return st->arr[(st->size) - 1];
}
void pushBack(stack* st, double symbol) {
    st->arr[st->size] = symbol;
    st->size += 1;
}

void activateStack(stack* st) {
    st->size = 0;
    st->pop = &delete_last;
    st->empty = &checkEmpty;
    st->top = &lastElement;
    st->push = &pushBack;
}

bool calculate(stack* numbers, stack* op) {
    double b = numbers->top(numbers);
    numbers->pop(numbers);
    double a = numbers->top(numbers);
    numbers->pop(numbers);
    switch ((int)op->top(op)) {
        case '*':
            numbers->push(numbers, a * b);
            break;
        case '/':
            if (b == 0) {
                return 0;
            }
            numbers->push(numbers, a / b);
            break;
        case '+':
            numbers->push(numbers, a + b);
            break;
        default: //case '-':
            numbers->push(numbers, a - b);
            break;
    }
    op->pop(op);
    return 1;
}
