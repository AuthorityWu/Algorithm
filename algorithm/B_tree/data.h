/*
 * Author       : wqw
 * Date         : 2018/10/10
 * File         : data.h
 * Description  : Data 数据类 类声明及定义
 */

#ifndef B_TREE_DATA_H
#define B_TREE_DATA_H

class Data {
private:
    int number;
public:
    Data() { };
    Data(int num) { setNumber(num); }
    void setNumber(int num) { number = num; }
    int getNumber() { return number; }
    static int Cmp(Data* data1, Data* data2) {
        if (data1->number > data2->number)
            return 1;
        else if (data1->number < data2->number)
            return -1;
        else
            return 0;
    }
};

#endif //B_TREE_DATA_H
