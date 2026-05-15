// Copyright 2021 NNTU-CS
#include "train.h"

Train::Train() : first(nullptr), countOp(0) {}

void Train::addCar(bool light) {
    Car* newCar = new Car{light, nullptr, nullptr};
    if (!first) {
        first = newCar;
        newCar->next = newCar;
        newCar->prev = newCar;
    } else {
        newCar->next = first;
        newCar->prev = first->prev;
        first->prev->next = newCar;
        first->prev = newCar;
    }
}

int Train::getLength() {
    if (!first) return 0;

    if (!first->light) {
        first->light = true;
    }

    int k = 1;
    Car* current;

    while (true) {
        current = first;
        for (int i = 0; i < k; ++i) {
            current = current->next;
            ++countOp;
        }

        if (current->light) {
            current->light = false;

            for (int i = 0; i < k; ++i) {
                current = current->prev;
                ++countOp;
            }

            if (current == first && !first->light) {
                return k;
            }
        } else {
            for (int i = 0; i < k; ++i) {
                current = current->prev;
                ++countOp;
            }
        }

        ++k;
    }
}

int Train::getOpCount() {
    return countOp;
}

