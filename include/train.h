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
        
        // Шаг 1: выключаем свет во всех вагонах
        // Сначала в первом вагоне
        if (first->light) {
            first->light = false;
            countOp++;
        }
        
        // Затем обходим остальные
        Car* current = first->next;
        countOp++;  // переход к next
        
        while (current != first) {
            if (current->light) {
                current->light = false;
                countOp++;
            }
            current = current->next;
            countOp++;  // переход к следующему
        }
        
        // Шаг 2: включаем свет в первом вагоне
        first->light = true;
        countOp++;
        
        // Шаг 3: считаем длину, двигаясь до горящего света
        int length = 1;
        current = first->next;
        countOp++;  // переход к next
        
        while (current != first) {
            if (current->light) {
                break;
            }
            current = current->next;
            countOp++;  // переход к следующему
            length++;
        }
        
        return length;
    }

    int getOpCount() {
        return countOp;
    }
};

#endif  // INCLUDE_TRAIN_H_
