/*
 * Author       : wqw
 * Date         : 2018/9/4
 * File         : Binary_tree.h
 * Description  : 二叉树的基本结构实现
 * e-mail	: AuthorityWu@outlook.com
 */

#ifndef ADT_BINARY_TREE_H
#define ADT_BINARY_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define False  0
#define True   1

typedef struct Node* Next;
typedef struct Node* Tree;

struct Node{
    int number;
    Next left;
    Next right;
};

// 毁灭/放空 二叉树
Tree MakeEmpty(Tree tree) {
    Tree temp = tree;

    if (temp->left != NULL)
        MakeEmpty(temp->left);
    if (temp->right != NULL)
        MakeEmpty(temp->right);
    free(temp);
    return NULL;
}

// 查找二叉树的 最左端/最小值
Tree FindMin(Tree tree) {
    Tree temp = tree;

    if (temp->left == NULL)
        return temp;
    else
        return FindMin(temp->left);
}

// 查找二叉树的 最右端/最大值
Tree FindMax(Tree tree) {
    Tree temp = tree;

    if (temp->right == NULL)
        return temp;
    else
        return FindMax(temp->right);
}

// 查找结点
Tree Find(Tree tree, int number) {
    Tree temp = tree;

    if (temp == NULL)
        return NULL;

    if (temp->number > number )
        return Find(temp->left, number);
    else if (temp->number == number)
        return  temp;
    else
        return Find(temp->right, number);
}

// 插入节点
int Insert(Tree tree, Tree node) {
    int flag = 0;   // 用于判断是否找到插入的位置
    Tree temp = tree;

    while (!flag) {
        if (temp->number > node->number) {  // 判断接下来的走向
            if (temp->left == NULL) {
                flag = 1;
                temp->left = node;
            } else
                temp = temp->left;
        } else if (temp->number == node->number) {  // 若该点已存在则不重复存储
            return False;
        } else {
            if (temp->right == NULL) {
                flag = 1;
                temp->right = node;
            } else
                temp = temp->right;
        }
    }
    return True;
}

// 创建结点并插入结点
Tree NewNode(int number, Tree tree) {
    Tree new_node;

    new_node = (Tree) malloc (sizeof(struct Node));
    new_node->number = number;
    new_node->right = new_node->left = NULL;
    // 如果树为空则创建树的根结点，否则插入新节点
    if (tree == NULL)
        tree = new_node;
    else
        if (!Insert(tree, new_node))
            free(new_node);

    return tree;
}

// 查找某节点的父节点
Tree FindPre(int number, Tree tree) {
    Tree temp = tree;

    if (temp->number == number)     // 若该点正好是tree的根节点，则返回根节点
        return temp;

    if (temp->number > number) {
        if (temp->left->number == number)
            return temp;
        else
            return FindPre(number, temp->left);
    } else {
        if (temp->right->number == number)
            return temp;
        else
            return FindPre(number, temp->right);
    }
}


/* 删除结点:
 * 情况一： 若结点不存在则返回False
 * 情况二： 若结点无子结点则直接删除
 * 情况三：
 * 若存在左子结点则再左子树中寻找max结点作为代替节点
 * 若只存在右子结点则再右子树中寻找min结作为代替节点
 * 需要知道删除结点的父节点 或 代替节点的父节点 pre
 * 先找代替节点的父节点pre
 * 交换代替节点与删除结点的数据
 * 若代替节点存在子节点，则让pre指向代替节点的指针指向其子节点
 * 最后删除代替节点
 */

Tree DelNode(int number, Tree tree) {
    Tree  del_node;
    Tree  replace_node;
    Tree  pre;

    if (!(del_node = Find(tree, number)))
        return tree;

    if (del_node->left != NULL) {       // 情况三
        replace_node = FindMax(del_node->left);         // 查找代替结点
        pre = FindPre(replace_node->number, del_node);  // 查找代替节点的父节点
        del_node->number = replace_node->number;        // 交换代替节点与删除结点的数据
        // 让pre指向代替节点的指针指向其子节点
        if (pre->left != NULL && pre->left->number == replace_node->number)
            pre->left = replace_node->left;
        else
            pre->right = replace_node->left;
    } else if (del_node->right != NULL) {
        replace_node = FindMin(del_node->right);
        pre = FindPre(replace_node->number, del_node);
        del_node->number = replace_node->number;

        if (pre->left != NULL && pre->left->number == replace_node->number)
            pre->left = replace_node->right;
        else
            pre->right = replace_node->right;
    } else {    // 情况二
        if (del_node != tree){
            pre = FindPre(del_node->number, tree);
            if (pre->left != NULL && pre->left->number == del_node->number)
                pre->left = NULL;
            else
                pre->right = NULL;
        } else
            tree = NULL;
        free(del_node);
        return tree;
    }

    free(replace_node);
    return tree;
}

// 中序遍历 打印二叉树
void Print(Tree tree) {
    Tree temp = tree;

    if (temp == NULL)
        return;

    Print(temp->left);
    printf("%d  ", temp->number);
    Print(temp->right);
    return;
}

#endif //ADT_BINARY_TREE_H
