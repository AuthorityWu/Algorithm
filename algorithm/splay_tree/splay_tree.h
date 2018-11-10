//
// Created by 14493 on 2018/10/5.
//

#ifndef SPLAY_TREE_SPLAY_TREE_H
#define SPLAY_TREE_SPLAY_TREE_H

#include <iostream>
#include <malloc.h>
#include "data.h"

using namespace std;

typedef struct SplayNode* Tree;
typedef struct SplayNode* Next;

struct SplayNode {
    Data* data;         // 数据项
    Next left_son;      // 左儿子
    Next right_son;     // 右儿子
};

class SplayTree {
public:
    Tree root;
public:
    SplayTree() { root = NULL; }                             // 构造函数

    static Tree NewNode(Data*);                             // 新建节点
    static Tree Find(Tree tree, Data* data);                // 查找结点
    static Tree Enquiry(Tree tree, Data*);                  // 查询节点
    static Tree EnquiryMax(Tree, Data*);                    // 查询最右节点
    static Tree EnquiryMin(Tree, Data*);                    // 查询最左节点
    static Tree Insert(Tree tree, Tree);                    // 插入节点
    static Tree Delete(Tree tree, Data*);                   // 删除节点
    static Tree SingleRotation(Tree tree, int direction);   // 单旋转
};
// 单旋转
Tree SplayTree::SingleRotation(Tree tree, int direction) {
    if (direction == 0) {
        Tree temp = tree->left_son;
        tree->left_son = temp->right_son;
        temp->right_son = tree;
        return temp;
    } else {
        Tree temp = tree->right_son;
        tree->right_son = temp->left_son;
        temp->left_son = tree;
        return temp;
    }
}
// 查找结点
Tree SplayTree::Find(Tree tree, Data* data) {
    if (tree == NULL)
        return NULL;
    else if (Data::Cmp(tree->data, data) == 1)
        Find(tree->left_son, data);
    else if (Data::Cmp(tree->data, data) == -1)
        Find(tree->right_son, data);
    else
        return tree;
}
// 新建节点
Tree SplayTree::NewNode(Data* data) {
    Tree new_node = NULL;
    if ((new_node = (Tree)malloc(sizeof(struct SplayNode))) == NULL) {
        cout<<"Out of Space!"<<endl;
        exit(1);
    }
    new_node->data = data;
    new_node->left_son = new_node->right_son = NULL;
    return new_node;
}
// 查询最右节点（给删除节点使用）
Tree SplayTree::EnquiryMax(Tree tree, Data* data) {
    if (tree == NULL)
        return NULL;

    if (tree->right_son == NULL)
        return tree;
    else {
        tree->right_son = EnquiryMax(tree->right_son, data);
        // 通过旋转进行伸展
        tree = SingleRotation(tree, 1);
        return tree;
    }
}
// 查询最左节点（给删除节点使用）
Tree SplayTree::EnquiryMin(Tree tree, Data* data) {
    if (tree == NULL)
        return NULL;

    if (tree->left_son == NULL)
        return tree;
    else {
        tree->left_son = EnquiryMin(tree->left_son,data);
        // 通过旋转进行伸展
        tree = SingleRotation(tree, 0);
        return tree;
    }
}
// 查询节点
Tree SplayTree::Enquiry(Tree tree, Data* data) {
    if (tree == NULL)
        return NULL;
    // 判断是否找到目标节点
    if (Data::Cmp(tree->data, data) == 0)
        return tree;
    // 判断接下来的走向
    else if (Data::Cmp(tree->data,data) == 1) {
        tree->left_son =  Enquiry(tree->left_son, data);
        // 通过旋转进行伸展
        if (Data::Cmp(tree->left_son->data,data) >= 0)
            tree = SingleRotation(tree,0);
        else
            tree = SingleRotation(tree,1);
        return tree;
    }
    else {
        tree->right_son = Enquiry(tree->right_son, data);
        if (Data::Cmp(tree->right_son->data, data) <= 0)
            tree = SingleRotation(tree, 1);
        else
            tree = SingleRotation(tree, 0);
        return tree;
    }
}
// 插入节点
Tree SplayTree::Insert (Tree tree, Tree insert_node) {
    Tree pre, pos;

    if (tree == NULL)
        tree = insert_node;
    pos = pre = tree;
    // 找到可插入位置
    while (pos != NULL) {
        if (Data::Cmp(pos->data, insert_node->data) == 1) {
            pre = pos;
            pos = pos->left_son;
        } else if (Data::Cmp(pos->data, insert_node->data) == -1) {
            pre = pos;
            pos = pos->right_son;
        } else          // 若找到相同的数据节点，则直接结束循环
            break;
    }
    // 若插入的位置非空说明存在相同的数据节点，不重复插入，对该节点进行伸展
    if (pos == NULL) {
        if (Data::Cmp(pre->data, insert_node->data) == 1)
            pre->left_son = insert_node;
        else if (Data::Cmp(pre->data, insert_node->data) == -1)
            pre->right_son = insert_node;
    }
    // 查询刚刚插入的节点，进行节点伸展
    tree = Enquiry(tree, insert_node->data);
    return tree;
}

/* 删除节点
 * 对于要被删除的节点，先进行删除节点的伸展，
 * 找删除节点的左子树的最右节点（或 找删除节点的右子树的最左节点）进行代替（与二叉树删除类似）
 * 再对代替节点进行伸展，
 * 如：1.先对删除节点进行伸展，若伸展树超过1个节点，
 * 2.则寻找代替节点，左子树存在则，
 * 代替节点为删除节点的左子树的最右节点，
 * 3.对代替节点进行伸展，使得左子树的根节点为代替节点，
 * 4.让代替节点得右儿子指针指向删除节点的右子树，则删除完成
 * （由伸展树的性质得，代替节点作为左子树的根节点时，不存在右子树）
 */
Tree SplayTree::Delete(Tree tree, Data* data) {
    Tree del_node = Find(tree, data);

    if (del_node != NULL) {
        // 对要删除的节点进行伸展
        del_node = Enquiry(tree, data);
        // 找左子树的最右节点进行代替
        if (del_node->left_son != NULL) {
            Tree replace_node = EnquiryMax(del_node->left_son, data);
            // 代替节点的右儿子指向删除节点的右儿子
            replace_node->right_son = del_node->right_son;
            free(del_node->data);
            delete del_node;
            tree = replace_node;
        }
        // 找右子树的最左节点进行代替
        else if ((del_node->right_son != NULL)) {
            Tree replace_node = EnquiryMin(del_node->right_son, data);
            replace_node->left_son = del_node->left_son;
            free(del_node->data);
            delete del_node;
            tree = replace_node;
        }
        // 左右子树都为空时，则该节点为该树的最后一个节点,直接删除
        else {
            free(del_node->data);
            delete del_node;
            tree = NULL;
        }
    }
    return tree;
}

#endif //SPLAY_TREE_SPLAY_TREE_H
