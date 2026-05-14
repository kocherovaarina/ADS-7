//Copyright NNTU-CS
#ifndef TRAIN_H
#define TRAIN_H

struct Car {
    bool light;
    Car* next;
    Car* prev;
};

class Train {
private:
    int countOp;
    Car* first;

public:
    Train() : countOp(0), first(nullptr) {}

    void addCar(bool light) {
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

    int getLength() {
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

    int getOpCount() {
        return countOp;
    }
};

#endif // TRAIN_H
