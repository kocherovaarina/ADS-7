// Copyright 2021 NNTU-CS
#include "train.h"
int Train::getLength() {
    if (!first) return 0;

    int length = 0;
    Car* current = first;

    current->light = false;
    countOp++;

    while (true) {
        for (int i = 0; i < length + 1; i++) {
            current = current->next;
            countOp++;
        }

        if (current->light) {
            current->light = false;
            countOp++;

            for (int i = 0; i < length + 1; i++) {
                current = current->prev;
                countOp++;
            }

            length++;
        } else {
            length++;
            break;
        }
    }

    return length;
}
