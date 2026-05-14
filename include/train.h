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
        if (!first) {
            return 0;
        }

        // Выключаем все вагоны, чтобы начать с чистого листа
        Car* current = first;
        do {
            current->light = false;
            current = current->next;
        } while (current != first);

        // Включаем первый вагон как метку
        first->light = true;
        countOp++;

        // Если один вагон — сразу возвращаем
        if (first->next == first) {
            return 1;
        }

        int length = 0;
        current = first;

        while (true) {
            current = current->next;
            countOp++;
            length++;

            if (current->light) {
                break;
            }

            current->light = true;
            countOp++;
        }

        return length;
    }

    int getOpCount() {
        return countOp;
    }
};

#endif  // INCLUDE_TRAIN_H_
