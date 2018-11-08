/*
 * Author       : wqw
 * Date         : 2018/9/28
 * File         : tree_AVL.cpp
 * Description  : AVLTree 基本功能 及 数据成员声明 介绍
 */


#ifndef AVL_TREE_TREE_AVL_H
#define AVL_TREE_TREE_AVL_H

#include <cmath>
#include <time.h>
#include <malloc.h>
#include "data.h"

typedef struct Node* Tree;
typedef struct Node* Next;

struct Node{
    Data* data;     // 节点数据项
    int height;     // 当前节点的高度
    Next left;      // 左儿子
    Next right;     // 右儿子
};

class TreeAVL {
public:
    Tree tree_AVL_root; // 根节点
public:
    // 构造
    TreeAVL() { tree_AVL_root = NULL; }
    // 单旋转，int类型的参数用于判断是左旋哈偶是右旋
    Tree SingleRotation(Tree tree, int);
    Tree DoubleRotation(Tree tree, int);    // 双旋转，int参数同上
    Tree NewNode(Data* data);               // 创建新节点
    Tree Insert(Tree node, Tree tree);      // 插入节点
    Tree Find(Data*, Tree tree);            // 查找节点

    Tree Delete(Data*, Tree tree);          // 删除节点
    Tree FindLeft(Tree);                    // 寻找最左节点
    Tree FindRight(Tree);                   // 寻找最右节点
    Tree DelNodeWithoutSon(Data*, Tree tree);   // 删除一个没有子节点的节点
    Tree Replace(Tree tree);                // 寻找代替节点
    void Print(Tree);                       // 遍历AVL树，打印数据
    static int Height(Tree tree);          // 获取节点高度
    static int Max(int h1, int h2) { return h1 > h2 ? h1 : h2; }    // 比较高度，返回较高的一方
    static bool CmpHeight(int h1, int h2) { return h1 > h2 ? true : false; }    // 比较高度，返回h1是否高于h2
};

#endif //AVL_TREE_TREE_AVL_H
