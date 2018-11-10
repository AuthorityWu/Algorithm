/*
 * Author       : wqw
 * Date         : 2018/10/10
 * File         : B_tree_example.cpp
 * Description  : B_tree 的插入删除功能测试、例子
 * notice       : 以下博客对了 B-Tree 的插入及删除作了具体分析。
 * url: https://blog.csdn.net/qq_40760673/article/details/83247243
 */

#include <stdio.h>
#include <time.h>
#include <algorithm>
#include "B_tree.h"

int num[30];

void CreateTree(B_Tree* btree) {
    srand((unsigned int)time(NULL));
    for (int i=0; i<30;  i++) {
        num[i] = rand()%1000;
        Data* data = new Data(num[i]);
        btree->root_pre = B_Tree::Insert(btree->root_pre, data);
        cout<<num[i]<<',';
    }
    cout<<endl;
}

void Print(Tree tree) {
    if (tree->max_size == LEAF_SIZE) {
        for (int i=0; i<tree->data.size(); i++)
            cout<<tree->data[i]->getNumber()<<',';
    }
    else {
        int i=0;
        for (; i<tree->data.size(); i++) {
            Print(tree->next[i]);
            cout<<tree->data[i]->getNumber()<<',';
        }
        Print(tree->next[i]);
    }
}

int main() {
    B_Tree* btree = new B_Tree();
    CreateTree(btree);
    cout<<"Print Tree: "<<endl;
    Print(btree->root_pre->next[0]);
    cout<<endl;

    for (int i=0; i<30; i++) {
        cout<<"Delete: "<<num[i]<<endl;
        btree->root_pre = btree->Delete(btree->root_pre, new Data(num[i]));
        Print(btree->root_pre->next[0]);
        cout<<endl;
    }
    return 0;
}
