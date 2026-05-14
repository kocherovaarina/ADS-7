// Copyright 2022 NNTU-CS
#include "train.h"

Train::Train() : countOp(0), first(nullptr) {}

void Train::addCar(bool light) {
    Car* newCar = new Car{light, nullptr, nullptr};

    if (!first) {
        first = newCar;
        first->next = first;
        first->prev = first;
    } else {
        Car* last = first->prev;
        last->next = newCar;
        newCar->prev = last;
        newCar->next = first;
        first->prev = newCar;
    }
}

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

int Train::getOpCount() {
    return countOp;
}
