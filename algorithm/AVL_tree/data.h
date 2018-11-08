/*
 * Author       : wqw
 * Date         : 2018/9/28
 * File         : data.cpp
 * Description  : AVLTree 的 数据项
 */

#ifndef AVL_TREE_DATA_H
#define AVL_TREE_DATA_H

#include <iostream>

using namespace std;

class Data {
private:
    int number;
public:
    Data() {}
    Data(int num) { number = num; }
    void set_num(int num) { number = num; }
    int get_num() { return number; }
    void operator= (int num) { number = num; }
    void print() { cout<<number; }
    static int DataCmp(Data* data1, Data* data2) {
        if (data1->number > data2->number)
            return 1;
        else if (data1->number < data2->number)
            return -1;
        else
            return 0;
    }
};

#endif //AVL_TREE_DATA_H
