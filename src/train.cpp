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

    bool anyLightOn = first->light;
    if (!anyLightOn) {
        // cppcheck-suppress constVariablePointer
        Car* temp = first->next;
        while (temp != first) {
            if (temp->light) {
                anyLightOn = true;
                break;
            }
            temp = temp->next;
        }
    }

    if (!anyLightOn) {
        first->light = true;
        int steps = 0;
        // cppcheck-suppress constVariablePointer
        Car* current = first->next;
        steps = 1;
        countOp = 1;

        while (current != first) {
            current = current->next;
            steps++;
            countOp++;
        }

        for (int i = 0; i < steps; i++) {
            current = current->prev;
            countOp++;
        }

        return steps;
    }

    if (!first->light) {
        first->light = true;
    }

    int k = 1;
    Car* current;

    while (true) {
        current = first;
        for (int i = 0; i < k; i++) {
            current = current->next;
            countOp++;
        }

        if (current->light) {
            current->light = false;
            for (int i = 0; i < k; i++) {
                current = current->prev;
                countOp++;
            }
            if (current == first && !first->light) {
                return k;
            }
        } else {
            for (int i = 0; i < k; i++) {
                current = current->prev;
                countOp++;
            }
        }
        k++;
    }
}

int Train::getOpCount() {
    return countOp;
}
