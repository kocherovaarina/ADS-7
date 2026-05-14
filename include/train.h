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

        int length = 0;
        Car* current = first;

        // Убедимся, что стартовый вагон выключен
        if (current->light) {
            current->light = false;
            countOp++;
        }

        while (true) {
            // Идём вперёд на length + 1 шагов
            for (int i = 0; i < length + 1; i++) {
                current = current->next;
                countOp++;
            }

            if (current->light) {
                // Если горит — тушим
                current->light = false;
                countOp++;

                // Возвращаемся назад
                for (int i = 0; i < length + 1; i++) {
                    current = current->prev;
                    countOp++;
                }

                length++;
            } else {
                // Если не горит — включаем и выходим
                current->light = true;
                countOp++;
                length++;
                break;
            }
        }

        return length;
    }

    int getOpCount() {
        return countOp;
    }
};

#endif  // INCLUDE_TRAIN_H_
