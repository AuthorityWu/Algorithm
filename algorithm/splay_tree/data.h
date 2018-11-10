//
// Created by 14493 on 2018/10/5.
//

#ifndef SPLAY_TREE_DATA_H
#define SPLAY_TREE_DATA_H

class Data {
private:
    int number;
public:
    Data(int num) { number = num; }
    void SetNum(int num) { number = num; }
    int GetNum() { return number; }
    static int Cmp(Data* x1, Data* x2);
};

int Data::Cmp(Data* x1, Data* x2) {
    if (x1 == NULL)
        return 1;
    else if (x2 == NULL)
        return -1;

    if (x1->number > x2->number)
        return 1;
    else if (x1->number < x2->number)
        return -1;
    else
        return 0;
}

#endif //SPLAY_TREE_DATA_H
