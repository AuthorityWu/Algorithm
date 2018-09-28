/*
 * Author       : wqw
 * Date         : 2018/9/28
 * File         : hashing.h
 * Description  : 散列表及再散列的操作
 * 实现《数据结构与算法分析-C语言描述》中有关可扩散的内容中的存储比特的散列结构。
 *
 * Notice(注意): 本代码实现的结构，是由本人进行设计的，可能比较抽象
 * 有兴趣的可以通过以下网址，访问有关该结构的文章：
 *
 */

#ifndef EXTENDIBLE_HASHING_HASHING_H
#define EXTENDIBLE_HASHING_HASHING_H

#include <iostream>

using namespace std;

const int MAX_NUM = 11;
const int MAX_SIZE = 11;

class Hashing {
public:
    int total;              // 散列表中的总数量
    int list[MAX_SIZE];     // 散列表

    Hashing();
    int Find(int key);
    bool Insert(int key);
    friend void ReHashing(Hashing*, Hashing*, Hashing*, int);
};

Hashing::Hashing() {
    total = 0;
    for (int i=0; i<MAX_SIZE; i++ )
        list[i] = -1;
}
// 平方探测法 F(i)=i^2 返回已存在的key或可插入位置
int Hashing::Find(int key) {
    int pos, count=0, sum = 0;
    sum += key;
    pos = sum % MAX_NUM;
    while (list[pos] != -1) {   // 如果找到
        if (list[pos] == key)
            return pos;
        count++;
        sum += count*2 - 1;
        pos = sum % MAX_SIZE;
    }
    return pos;
}
// 插入元素
bool Hashing::Insert(int key) {
    int pos = Find(key);    // 先查找可插入位置
    if (list[pos] != key) {
        list[pos] = key;
        total++;
    } else
        return false;
    return true;
}
// 再散列
void ReHashing(Hashing* h0, Hashing* h1, Hashing* h2, int deep) {
    for (int i=0; i<MAX_SIZE; i++) {
        if (h0->list[i] != -1) {
            // 判断list[i]转化为二进制时前缀中的第 deep 个数为 1 or 0
            if ( h0->list[i] & (1<<(MAX_NUM - deep - 1)) ) {
                h2->Insert(h0->list[i]);
            } else {
                h1->Insert(h0->list[i]);
            }
        }
    }
}

#endif //EXTENDIBLE_HASHING_HASHING_H
