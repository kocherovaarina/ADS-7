// Copyright 2021 NNTU-CS
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

int Train::getOpCount() {
    return countOp;
}
