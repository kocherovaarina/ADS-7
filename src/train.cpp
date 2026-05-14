// Copyright 2021 NNTU-CS
#include "train.h"

int Train::getLength() {
    if (!first) return 0;

    int length = 0;
    Car* current = first;
    bool savedLight = current->light;

    if (current->light) {
        current->light = false;
        countOp++;
    }

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
            for (int i = 0; i < length + 1; i++) {
                current = current->prev;
                countOp++;
            }

            if (!current->light) {
                first->light = savedLight;
                if (savedLight && !first->light) {
                    countOp++;
                } else if (!savedLight && first->light) {
                    countOp++;
                }
                return length + 1;
            }

            length++;
            for (int i = 0; i < length + 1; i++) {
                current = current->next;
                countOp++;
            }
        }
    }
}
