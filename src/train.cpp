// Copyright 2021 NNTU-CS
#include "train.h"
int Train::getLength() {
    if (!first)
        return 0;

    if (first->next == first)
        return 1;

    first->light = true;
    countOp++;

    int length = 0;
    Car* current = first;

    while (true) {
        current = current->next;
        countOp++;
        length++;

        if (current->light)
            break;

        current->light = true;
        countOp++;
    }

    return length;
}

