/*
 * Author       : wqw
 * Date         : 2018/9/4
 * File         : Binary_tree.h
 * Description  : 二叉树的基本功能的测试代码
 * e-mail	: AuthorityWu@outlook.com
 */

#include <time.h>
#include "binary_tree.h"

int num[15];

// 生成一颗随机二叉树
Tree CreateTree(Tree tree) {
    int i;

    srand((int)time(NULL));
    for (i = 0; i < 15; i++) {
        num[i] = rand() % 1000;
        tree = NewNode(num[i], tree);
    }
    return tree;
}

int main() {
    Tree tree = NULL;
    int i=0;

    tree = CreateTree(tree);
    Print(tree);
    printf("\n");

    for (; i < 15; i++) {
        printf("Delete: %d \nTree: ", num[i]);
        tree = DelNode(num[i], tree);
        Print(tree);
        printf("\n");
    }

    tree = CreateTree(tree);
    Print(tree);
    printf("\n");
    MakeEmpty(tree);
    Print(tree);
    printf("\n");

    return 0;
}
