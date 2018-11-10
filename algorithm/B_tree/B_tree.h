/*
 * Author       : wqw
 * Date         : 2018/10/10
 * File         : B_tree.h
 * Description  : B_tree的节点和树 类声明
 * notice	: 以下博客对了 B-Tree 的插入及删除作了具体分析。
 * url: https://blog.csdn.net/qq_40760673/article/details/83247243
 */

#ifndef B_TREE_B_TREE_H
#define B_TREE_B_TREE_H

#include <iostream>
#include <vector>
#include "data.h"

#define NO_LEAF_SIZE 2          // 非叶子节点的数据存储容量
#define LEAF_SIZE 3             // 叶子节点的数据存储容量

using namespace std;

typedef class B_TreeNode* Tree;
typedef class B_TreeNode* Next;

class B_TreeNode {
public:
    int max_size;               // 数据最大存储容量
    vector<Data*> data;         // 数据域
    vector<Next> next;          // 指针域
public:
    B_TreeNode(int max_size) {
        this->max_size = max_size;
    }
    ~B_TreeNode() {
        int data_size = data.size();
        int next_size = data.size();
        for (int i=0; i<data_size; i++)
            data.pop_back();
        for (int i=0; i<next_size; i++)
            next.pop_back();
    };
};

class B_Tree {
public:
    Tree root_pre;          // 根节点的父节点（不是根节点，方便于根节点分裂）
public:
    B_Tree();       // 构造函数
    ~B_Tree();      // 析构函数
    static Tree NewNode(int);               // 创建节点
    static Tree Find(Tree, Data*);          // 查找节点
    // 插入组件
    static void SplitNode(Tree, Tree, int, int);                   // 节点分裂
    static Tree InsertData(Tree, Tree, Data*, int);                 // 插入数据
    static Tree Insert(Tree, Data*);                                // 插入
    // 删除组件
    static void CombineNode(Tree, Tree, Tree, int);                // 合并节点
    static void Move(Tree, Tree, Tree, int, int);                  // 数据移动（三个节点间的数据移动）
    static void MoveData(Tree, Tree, int, int, int);               // 数据移动的分步（2个节点间的数据移动）
    static Tree FindMaxNode(Tree);                                  // 查找含有最大数据的节点
    static void DeleteLeafData(Tree, Tree, int, Data*);             // 删除叶子节点
    static Tree Delete(Tree, Data*);                                // 节点删除
};
#endif //B_TREE_B_TREE_H
