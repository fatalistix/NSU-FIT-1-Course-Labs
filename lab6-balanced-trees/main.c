#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) ((a > b) ? a : b)

typedef struct TAVLTree_t {
    struct TAVLTree_t* Right;
    struct TAVLTree_t* Left;
    int Value;
    unsigned char Height;
} TAVLTree;

int ReadInt();
TAVLTree* AddElement(TAVLTree* t, TAVLTree* Nodes, int Index);
int CalcHeight(TAVLTree* t);

int main() {
    int NumberOfNodes = ReadInt();
    TAVLTree* Nodes = (TAVLTree*) calloc(sizeof(TAVLTree), NumberOfNodes);
    TAVLTree* Tree = NULL;
    for (int i = 0; i < NumberOfNodes; i++) {
        Nodes[i].Value = ReadInt();
        Tree = AddElement(Tree, Nodes, i);
    }
    printf("%d", CalcHeight(Tree));
    free(Nodes);
    return 0;
}

int ReadInt() {
    int Number = fgetc(stdin);
    char Sign = 1;
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

unsigned char Height(TAVLTree* t) {
    return t ? t->Height : 0;
}
int BalanceCode(TAVLTree* t) {
    return Height(t->Right) - Height(t->Left);
}
void FixHeight (TAVLTree* t) {
    unsigned char LeftHeight  = Height(t->Left);
    unsigned char RightHeight = Height(t->Right);
    t->Height = (LeftHeight > RightHeight ? LeftHeight : RightHeight) + 1;
}
TAVLTree* LeftRotation (TAVLTree* t) {
    TAVLTree* Temp = t->Right;
    t->Right = Temp->Left;
    Temp->Left = t;
    FixHeight(t);
    FixHeight(Temp);
    return Temp;
}
TAVLTree* RightRotation(TAVLTree* t) {
    TAVLTree* Temp = t->Left;
    t->Left = Temp->Right;
    Temp->Right = t;
    FixHeight(t);
    FixHeight(Temp);
    return Temp;
}
TAVLTree* BalanceAVLTree(TAVLTree* t) {
    FixHeight(t);
    if (BalanceCode(t) == 2) {
        if (BalanceCode(t->Right) < 0) {
            t->Right = RightRotation(t->Right);
        }
        return LeftRotation(t);
    }
    if (BalanceCode(t) == -2) {
        if (BalanceCode(t->Left) > 0) {
            t->Left = LeftRotation(t->Left);
        }
        return RightRotation(t);
    }
    return t;
}
TAVLTree* AddElement(TAVLTree* t, TAVLTree* Nodes, int Index) {
    if (t == NULL) {
        Nodes[Index].Height = 1;
        return &(Nodes[Index]);
    }
    if (Nodes[Index].Value <= t->Value) {
        t->Left  = AddElement(t->Left,  Nodes, Index);
    }
    else {
        t->Right = AddElement(t->Right, Nodes, Index);
    }
    return BalanceAVLTree(t);
}

int CalcHeight(TAVLTree* t) {
    if (t == NULL) {
        return 0;
    }
    int LeftMax  = CalcHeight(t->Left);
    int RightMax = CalcHeight(t->Right);
    return MAX(LeftMax, RightMax) + 1;
}
