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

    int length = 1;
    Car* current = first;
    bool startLight = current->light;

    current->light = false;
    countOp++;

    while (true) {
        for (int i = 0; i < length; i++) {
            current = current->next;
            countOp++;
        }

        if (current->light) {
            current->light = false;
            countOp++;

            for (int i = 0; i < length; i++) {
                current = current->prev;
                countOp++;
            }

            length++;
        } else {
            for (int i = 0; i < length; i++) {
                current = current->prev;
                countOp++;
            }

            if (!current->light) {
                first->light = startLight;
                if (startLight) {
                    countOp++;
                }
                return length;
            }

            length++;
        }
    }
}

int Train::getOpCount() {
    return countOp;
}
