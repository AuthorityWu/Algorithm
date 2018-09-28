/*
 * Author       : wqw
 * Date         : 2018/9/12
 * File         : example_binomial_queue.cpp
 * Description  : 二项树的主要功能的测试代码
 */

#include "binomial_queue.h"
/*
 * 打印中“ 2-- + ”为2->right_child = 3,“ 2 ”为2->next_brother=3
 *              |                          |
 *              3                          3
 */
// 打印二项树
void PrintBinTree(BinTree T, int deep) {
    int i;
    if (deep == 0)
        printf("%3d", T->num);
    else {
        printf("   ");
        for (i = 0 ; i < deep - 1; i++)
            printf("   | ");
        printf("  %3d", T->num);
    }
    if (T->right_child != NULL) {
        printf("-- * \n");
        printf("   ");
        for (i = 0 ; i < deep+1; i++)
            printf("   | ");
        printf("\n");
        PrintBinTree(T->right_child, deep+1);
    } else
        printf("\n");
    if (T->next_brother != NULL) {
        printf("   ");
        for (i = 0 ; i < deep; i++)
            printf("   | ");
        printf("\n");
        PrintBinTree(T->next_brother, deep);
    }
    return ;
}
// 打印二项树队列
void PrintBinQueue(BinQueue H) {
    int i, j;

    for (i = 0, j = 1; j <= H->current_size; i++, j *= 2) {
        if (H->the_tree[i] != NULL) {
            printf("BinQueue->the_tree[%d]: \n", i);
            PrintBinTree(H->the_tree[i], 0);
            printf("\n");
        }
    }
}
// 随机创建二项树队列
BinQueue CreateBinQueue(BinQueue H) {
    int i, count = 16, num;

    srand((int)time(NULL));
    for (i = 0; i < count; i++) {
        num = rand()%1000;
        printf("%d\t", num);
        H = NewBinNode(H, num);
    }
    printf("\n");
    return H;
}

int main() {
    int i = 0, n;

    BinQueue bin_queue = NULL;

    bin_queue = InitializeBinQueue(bin_queue);
    bin_queue = CreateBinQueue(bin_queue);
    PrintBinQueue(bin_queue);

    printf("Delete: %d\n", DeleteMin(bin_queue));
    PrintBinQueue(bin_queue);
    n = bin_queue->current_size;
    // 删除所有节点
    for (; i < n; i++) {
        printf("Delete: %d\n", DeleteMin(bin_queue));
    }
    PrintBinQueue(bin_queue);

    return 0;
}