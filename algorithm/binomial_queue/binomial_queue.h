/*
 * Author       : wqw
 * Date         : 2018/9/12
 * File         : binomial_queue.h
 * Description  : 二项树的主要功能的实现。其中部分代码参考了《数据结构与算法分析--C语言描述》。
 *              二项树是用于实现更加高效的 堆或优先队列 的一种数据结构，在 堆或优先队列 的内容中应该可以找到。
 */

#ifndef BINOMIAL_QUEUE_BINOMIAL_QUEUE_H
#define BINOMIAL_QUEUE_BINOMIAL_QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#define MAX_TREES 8
#define INF 100000
int Capacity = (1<<MAX_TREES) - 1;

typedef struct BinNode* Next;       // next指针
typedef struct BinNode* BinTree;    // 二项树的根节点
typedef struct Collection* BinQueue;// 二项树队列的指针
// 二项树节点
struct BinNode {
    int num;            // 数据项
    Next right_child;    // 右儿子
    Next next_brother; // 左兄弟
};
// 二项树队列结构体
struct Collection {
    int current_size;               // 二项树队列的总节点数量
    BinTree the_tree[MAX_TREES];    // 二项树队列
};

// 初始化二项树队列
BinQueue InitializeBinQueue(BinQueue new_bin_queue);
// 合并一个结点到目标二项树队列
BinQueue InsertBinNode(BinQueue H, BinTree bin_node);
// 创建一个新的结点并合并到目标二项树队列
BinQueue NewBinNode(BinQueue H, int num);
// 合并两个二项树
BinTree CombineTree(BinTree T1, BinTree T2);
// 将两个二项树队列进行合并
BinQueue Marge(BinQueue H1, BinQueue H2);
// 删除BinQueue中的一个最小项
int DeleteMin(BinQueue H);

// 初始化二项树队列
BinQueue InitializeBinQueue(BinQueue new_bin_queue) {
    int i;
    new_bin_queue = (BinQueue)malloc(sizeof(struct Collection));
    for (i = 0; i < MAX_TREES; i++)
        new_bin_queue->the_tree[i] = NULL;
    new_bin_queue->current_size = 0;
    return new_bin_queue;
}
// 合并一个结点到目标二项树队列
BinQueue InsertBinNode(BinQueue H, BinTree bin_node) {
    BinQueue temp_bin_queue = NULL;
    // 创建一个只有一个节点的二项树队列
    temp_bin_queue = InitializeBinQueue(temp_bin_queue);
    temp_bin_queue->the_tree[0] = bin_node;
    temp_bin_queue->current_size += 1;
    H = Marge(H, temp_bin_queue);   // 将只有一个二项树队列进行合并

    return H;
}
// 创建一个新的结点并合并到目标二项树队列
BinQueue NewBinNode(BinQueue H, int num) {
    BinTree new_bin_node = NULL;
    // new_bin_node初始化
    new_bin_node = (BinTree)malloc(sizeof(struct BinNode));
    new_bin_node->right_child = new_bin_node->next_brother = NULL;
    new_bin_node->num = num;
    H = InsertBinNode(H, new_bin_node);     // 插入结点到二项树队列

    return H;
}
// 合并两个二项树
BinTree CombineTree(BinTree T1, BinTree T2) {
    BinTree temp;
    if (T1->num > T2->num) {    // 保证T1的根的值的为最小
        temp = T1;
        T1 = T2;
        T2 = temp;
    }
    T2->next_brother = T1->right_child;     // 让T2的左兄弟指向T1的右儿子
    T1->right_child = T2;                    // 让T1的右儿子指向T2

    return T1;
}
// 将两个二项树队列进行合并
BinQueue Marge(BinQueue H1, BinQueue H2) {
    BinTree T1, T2;
    BinTree Carry = NULL;   // 用于存储T1，T2的合并的新二项树
    int i, j;
    // 判断合并后的总节点数量是否会超出二项树队列所能容纳的
    if (H1->current_size + H2->current_size > Capacity) {
        printf("Error! Merge will out of capacity!");
        return H1;
    }
    H1->current_size += H2->current_size;
    for (i = 0, j = 1; j <= H1->current_size; i++, j *= 2) {
        T1 = H1->the_tree[i];
        T2 = H2->the_tree[i];
        // 这里使用二进制的方法表示了所有的情况共8种(非常巧妙)
        switch (!!T1 + !!T2*2 + !!Carry*4) {
            case 0:     // T1, T2, Carry 都为空
            case 1:     // T2, Carry 为空， T1 不为空
                break;
            case 2:     // T1, Carry 为空， T2 不为空
                H1->the_tree[i] = T2;
                H2->the_tree[i] = NULL;
                break;
            case 3:     // Carry NULL， T1， T2 NO NULL
                Carry = CombineTree(T1, T2);
                H1->the_tree[i] = NULL;
                H2->the_tree[i] = NULL;
                break;
            case 4:     // T1,T2 NULL, Carry NO NULL
                H1->the_tree[i] = Carry;
                Carry = NULL;
                break;
            case 5:     // T2 NULL, T1, Carry NO NULL
                Carry = CombineTree(T1, Carry);
                H1->the_tree[i] = NULL;
                break;
            case 6:     // T1 NULL, T2, Carry NO NULL
            case 7:     // T1, T2, Carry NO NULL
                Carry = CombineTree(Carry, T2);
                H2->the_tree[i] = NULL;
                break;
        }
    }
    free(H2);
    return H1;
}
// 删除BinQueue中的一个最小项
int DeleteMin(BinQueue H) {
    BinQueue temp_bin_queue = NULL;
    BinTree del_node = NULL, temp_node;
    int i, del_pos = 0, min = INF, num;

    temp_bin_queue = InitializeBinQueue(temp_bin_queue);
    // 找到拥有最小根节点的二项树的位置
    for (i = 0; i < MAX_TREES; i++) {
        if (H->the_tree[i] != NULL && min > H->the_tree[i]->num) {
            min = H->the_tree[i]->num;
            del_pos = i;
        }
    }
    // 让最小节点的左儿子代替他的位置，释放最小节点
    del_node = H->the_tree[del_pos];
    num = del_node->num;
    H->the_tree[del_pos] = del_node->right_child;
    free(del_node);
    temp_bin_queue->current_size = (1<<del_pos) - 1;    // 删除掉最小根节点的二项树剩余的节点个数
    H->current_size -= (1<<del_pos);                    // 减去掉之前包含最小根节点的二项树的节点个数
    // 将删除掉最小节点的二项树拆开，用于构建临时的二项树队列temp_bin_queue
    for (i = del_pos - 1; i >= 0; i--) {
        temp_node = H->the_tree[del_pos];
        H->the_tree[del_pos] = temp_node->next_brother;
        temp_node->next_brother = NULL;
        temp_bin_queue->the_tree[i] = temp_node;
    }
    H = Marge(H, temp_bin_queue);   // 将temp_bin_queue重新合并回原本的二项树队列
    return num;
}

#endif //BINOMIAL_QUEUE_BINOMIAL_QUEUE_H
