// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TRAIN_H_
#define INCLUDE_TRAIN_H_

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
        if (!first) return 0;
        
        // Шаг 1: выключаем свет ВО ВСЕХ вагонах
        Car* current = first;
        do {
            if (current->light) {
                current->light = false;
                countOp++;
            }
            current = current->next;
            countOp++;
        } while (current != first);
        
        // Шаг 2: включаем свет в первом вагоне
        first->light = true;
        countOp++;
        
        // Шаг 3: считаем длину, двигаясь до горящего света
        int length = 1;
        current = first->next;
        countOp++;
        
        while (current != first) {
            if (current->light) {
                break;
            }
            length++;
            current = current->next;
            countOp++;
        }
        
        return length;
    }

    int getOpCount() {
        return countOp;
    }
};

#endif  // INCLUDE_TRAIN_H_
