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
        
        int length = 1;
        Car* current = first;
        
        // Выключаем свет в первом вагоне
        if (current->light) {
            current->light = false;
            countOp++;
        }
        
        while (true) {
            // Идем на length шагов вперед
            for (int i = 0; i < length; i++) {
                current = current->next;
                countOp++;
            }
            
            if (current->light) {
                // Нашли горящий свет - выключаем
                current->light = false;
                countOp++;
                length++;
            } else {
                // Нашли выключенный свет - включаем
                current->light = true;
                countOp++;
                
                // Возвращаемся на length шагов назад
                for (int i = 0; i < length; i++) {
                    current = current->prev;
                    countOp++;
                }
                
                // Проверяем, вернулись ли в первый вагон
                if (!current->light) {
                    return length;
                }
                length++;
            }
        }
    }

    int getOpCount() {
        return countOp;
    }
};

#endif  // INCLUDE_TRAIN_H_
